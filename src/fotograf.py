#!/usr/bin/python3
import os, time, argparse, shutil, json, subprocess
from datetime import date
import datetime
from sys import platform

from log import msg, die

try:
    from PIL import Image
except ImportError:
    print("PIL Python package not found!\nrun `pip install Pillow`")
    exit(1)

HOME = os.path.expandvars("${HOME}")
PWD = os.path.dirname(__file__)

parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("--save",      help="The Factorio save to load" , type=str)
parser.add_argument("--output",    help="Output directory"          , type=str)
parser.add_argument("--fac-base",  help="Specify Factorio base path", type=str, default=f"{HOME}/.factorio")
parser.add_argument("--fac-bin",   help="Specify Factorio binary"   , type=str, default="/usr/bin/factorio")
parser.add_argument("--png",       help="Use PNGs instead of JPEGs" , action="store_true")
parser.add_argument("--overwrite", help="Overwrite output directory", action="store_true")
parser.add_argument("--wrap",      help="A wrapper, like xvfb-run"  , type=str)
args = parser.parse_args()

""" ARGUMENT PARSING LOGIC """

if args.save == None:
    msg("No save name provided. You will have to load the save manually when Factorio starts.")

if args.output == None:
    if args.save == None:
        timestring = date.today().strftime("%d%m%Y%H%M%S")
        args.output = f"unnamed_map_{timestring}"
        msg(f"No output or save name provided. Using '{args.output}' as output directory")
    else:
        msg("No map name provided. Using save name as map name.")
        args.output = args.save

if os.path.isdir(PWD + "/" + args.output):
    if not args.overwrite:
        die(f"Error: the output directory '{args.output}' already exists. Use --overwrite argument to override existing directories")
    else:
        msg(f"Overwriting directory '{PWD}/{args.output}'")
        shutil.rmtree(PWD + "/" + args.output)

""" END OF ARGUMENT PARSING LOGIC """

# Enables/Disables our mod in the modlist
def edit_modlist(newval: bool) -> None:
    with open(f"{args.fac_base}/mods/mod-list.json", 'r+') as file:
        modlist = json.loads(file.read())
        for mod in modlist["mods"]:
            if mod["name"] == "fotograf":
                mod["enabled"] = newval
                break
        else:
            modlist["mods"].append({"name": "fotograf", "enabled": newval})
        file.seek(0)
        file.truncate(0)
        file.write(json.dumps(modlist))
        file.truncate()

msg("Start")

# The C code can work with both PNGs and JPEGs but it needs this flag to be set before compiling
with open(f"{PWD}/c/settings.h", "w") as file:
    file.write("#define " + ("PNG" if args.png else "JPG"))

# The Lua code also needs to have a PNG/JPEG flag
with open(f"{PWD}/fotograf_1.0.0/control.lua", "r+") as file:
    lines = file.readlines()
    lines[0] = "ext = \"" + (".png" if args.png else ".jpg") + "\"\n"
    file.seek(0)
    file.truncate(0)
    file.writelines(lines)
    file.truncate()



msg("Injecting FactorioFotograf into the game")
link_target = f"{PWD}/fotograf_1.0.0"
link_name = f"{args.fac_base}/mods/fotograf_1.0.0"
if os.path.exists(link_name): os.remove(link_name)
os.symlink(link_target, link_name, target_is_directory=True)

msg("Enabling FactorioFotograf mod") 
edit_modlist(True)

msg("Clearing script output")
for d in [f"{args.fac_base}/script-output/images/", f"{PWD}/{args.output}"]:
    if os.path.exists(d):
        shutil.rmtree(d)
for f in [f"{args.fac_base}/script-output/mapInfo.json", f"{args.fac_base}/script-output/done"]:
    if os.path.exists(f):
        os.remove(f)
shutil.copytree(f"{PWD}/web/", args.output) # copy the web template into the output directory
with open(f"{args.output}/.gitignore", "w") as file:
    file.write("*")

msg("Starting Factorio")
factorio_command = [args.fac_bin, "--load-game", args.save] if args.save != None else [args.fac_bin]
if args.wrap is not None:
  factorio_command.insert(0, args.wrap)
factorio_process = subprocess.Popen(factorio_command, stdout=subprocess.PIPE)

msg("Waiting for Factorio")
while not os.path.exists(f"{args.fac_base}/script-output/done"):
    if os.path.exists(f"{args.fac_base}/script-output/images/0"):
        count = 0
        for _, _, files in os.walk(f"{args.fac_base}/script-output/images/0"):
            count += len(files)
        msg(f"{count} images taken")
    time.sleep(1)
msg("Capturing done. Terminating Factorio")
factorio_process.terminate()

msg("Moving images")
shutil.move(f"{args.fac_base}/script-output/images/", f"{args.output}/")
with open(f"{args.fac_base}/script-output/mapInfo.json") as file:
    map_info_text = file.read() # we read map info because we need the map bounds ({max,min}{x,y}) for the zooming program
map_info = json.loads(map_info_text) 
image_resolution = map_info["image_resolution"] # creating the blank tile image
if args.png:
  Image.new(mode="RGBA", size=(image_resolution, image_resolution), color=(0, 0, 0, 0)).save(f"{args.output}/images/blank.png")
else:
  Image.new(mode="RGB", size=(image_resolution, image_resolution), color=(0, 0, 0)).save(f"{args.output}/images/blank.jpg")

msg("Creating web files")
with open(f"{args.output}/mapInfo.js", "w") as file:
    file.write(f"mapInfo = '{map_info_text}'\n")
with open(f"{args.output}/script.js", "r+") as file: # the script.js again need a PNG/JPEG flag set
    lines = file.readlines()
    lines[0] = "ext = \"" + (".png" if args.png else ".jpg") + "\"\n"
    lines[1] = "time = \"" + (datetime.datetime.now().strftime("%d.%m.%Y %H:%M:%S")) + "\"\n"
    file.seek(0)
    file.writelines(lines)
    file.truncate()

msg("Restoring mod settings")
edit_modlist(False)

msg("Compiling image merge tool") # TODO: it would be nice not to need gcc and make installed to run this script
make_process = subprocess.run(["make", "-C", PWD+"/c/"], stdout=subprocess.PIPE)

msg("Zooming")
maxx, maxy, minx, miny = map_info["maxx"], map_info["maxy"], map_info["minx"], map_info["miny"]
for i in range(8, 0, -1):
    os.makedirs(f"{args.output}/images/0/{i -1}/", exist_ok=True)
    subprocess.run(map(str, ["./imgmerge", args.output, i, i - 1, maxx, maxy, minx, miny]))

msg(f"Done. Output written into {PWD}/{args.output}/")

