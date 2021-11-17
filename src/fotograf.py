#!/usr/bin/python3
import os, argparse, time, shutil, json, subprocess, webbrowser
from datetime import date
from sys import platform
from PIL import Image

if platform != "linux":
    msg("Factorio Fotograf was not tested outside of Linux.")

start_time = time.time()
HOME = os.path.expandvars("${HOME}")
PWD = os.path.dirname(__file__)
modlist_path = f"{HOME}/.factorio/mods/mod-list.json"

def msg(text: str) -> None:
    start = f"FF {(time.time() - start_time):.2f}".ljust(11)
    print(f"{start} {text}")

def die(text: str) -> None:
    msg(f"ERROR: {text}")
    exit(1)

parser = argparse.ArgumentParser()
parser.add_argument("--save-name", help="The Factorio save to load", type=str)
parser.add_argument("--map-name",  help="How to name the map. Same as SAVE_NAME if not provided", type=str)
parser.add_argument("--png",       help="Use the PNG format instead of JPEG", action="store_true")
parser.add_argument("--fac-bin",   help="Specify factorio binary", type=str, default="factorio")
parser.add_argument("--overwrite", help="Overwrites existing files if necessary", action="store_true")
parser.add_argument("--view",      help="Opens the map in the default web browser", action="store_true")
args = parser.parse_args()

if args.save_name == None:
    msg("No save name provided. You will have to load the save manually when Factorio starts.")

if args.map_name == None:
    if args.save_name == None:
        timestring = date.today().strftime("%d%m%Y%H%M%S")
        args.map_name = f"unnamed_map_{timestring}"
        msg(f"No map or save name provided. Using '{args.map_name}' as map name")
    else:
        msg("No map name provided. Using save name as map name.")
        args.map_name = args.save_name

if os.path.isdir(PWD + "/" + args.map_name):
    if not args.overwrite:
        die(f"Error: the output directory '{args.map_name}' already exists. Use --overwrite argument to override existing directories")
    else:
        msg(f"Deleting directory '{PWD}/{args.map_name}'")
        shutil.rmtree(PWD + "/" + args.map_name)

def edit_modlist(newval: bool) -> None:
    with open(modlist_path, 'r+') as file:
        modlist = json.loads(file.read())

        for mod in modlist["mods"]:
            if mod["name"] == "fotograf":
                mod["enabled"] = newval
                break
        else:
            modlist["mods"].append({"name": "fotograf", "enabled": newval})

        file.seek(0)
        file.write(json.dumps(modlist))
        file.truncate()

msg("Start")

with open(f"{PWD}/c/settings.h", "w") as file:
    file.write("#define " + ("PNG" if args.png else "JPG"))

with open(f"{PWD}/fotograf_1.0.0/control.lua", "r+") as file:
    lines = file.readlines()
    lines[0] = "ext = \"" + (".png" if args.png else ".jpg") + "\"\n"
    file.seek(0)
    file.writelines(lines)
    file.truncate()

msg("Injecting FactorioFotograf into the game")
link_target = f"{PWD}/fotograf_1.0.0"
link_name = f"{HOME}/.factorio/mods/fotograf_1.0.0"
if os.path.exists(link_name): os.remove(link_name)
os.symlink(link_target, link_name, target_is_directory=True)

msg("Enabling FactorioFotograf mod") 
edit_modlist(True)

msg("Clearing script output")
for d in [f"{HOME}/.factorio/script-output/images/", f"{PWD}/{args.map_name}"]:
    if os.path.exists(d):
        shutil.rmtree(d)
for f in [f"{HOME}/.factorio/script-output/mapInfo.json", f"{HOME}/.factorio/script-output/done"]:
    if os.path.exists(f):
        os.remove(f)
shutil.copytree(f"{PWD}/web/", args.map_name)

msg("Starting Factorio")
factorio_command = ["factorio", "--load-game", args.save_name] if args.save_name != None else ["factorio"]
factorio_process = subprocess.Popen(factorio_command, stdout=subprocess.PIPE)

msg("Waiting for Factorio")
while not os.path.exists(f"{HOME}/.factorio/script-output/done"):
    if os.path.exists(f"{HOME}/.factorio/script-output/images/0"):
        count = 0
        for _, _, files in os.walk(f"{HOME}/.factorio/script-output/images/0"):
            count += len(files)
        msg(f"{count} images taken")
    time.sleep(1)
msg("Capturing done. Terminating Factorio")
factorio_process.terminate()

msg("Moving images")
shutil.move(f"{HOME}/.factorio/script-output/images/", f"{args.map_name}/")
with open(f"{HOME}/.factorio/script-output/mapInfo.json") as file:
    map_info_text = file.read()
map_info = json.loads(map_info_text)
image_resolution = map_info["image_resolution"]
Image.new(mode="RGBA", size=(image_resolution, image_resolution), color=(0, 0, 0, 0)).save(f"{args.map_name}/images/blank.png")

msg("Creating web files")
with open(f"{args.map_name}/mapInfo.js", "w") as file:
    file.write(f"mapInfo = '{map_info_text}'\n")
with open(f"{args.map_name}/script.js", "r+") as file: # replace the first line of script.js with the correct file suffix
    lines = file.readlines()
    lines[0] = "ext = \"" + (".png" if args.png else ".jpg") + "\"\n"
    file.seek(0)
    file.writelines(lines)
    file.truncate()

msg("Restoring mod settings")
edit_modlist(False)

msg("Compiling image merge tool")
back = os.getcwd()
os.chdir(PWD + "/c/")
make_process = subprocess.run(["make"], stdout=subprocess.PIPE)
os.chdir(back)

msg("Zooming")
maxx, maxy, minx, miny = map_info["maxx"], map_info["maxy"], map_info["minx"], map_info["miny"]
for i in range(8, 0, -1):
    os.makedirs(f"{args.map_name}/images/0/{i -1}/", exist_ok=True)
    subprocess.run(map(str, ["./imgmerge", args.map_name, i, i - 1, maxx, maxy, minx, miny]))

msg(f"Done. Output written into {PWD}/{args.map_name}/")
if args.view: webbrowser.open(f"file://{PWD}/{args.map_name}/index.html")

