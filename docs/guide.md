If you find something unclear, feel free to message me on discord (`Proicop#3264`), create issue or send a pull request.

# Setup instruction

- [Download the latest release](https://github.com/ProkopRandacek/FactorioFotograf/releases/)
- Extract the zip into a folder
- Make sure that you have:
  - `standalone factorio`
  - `make`
  - `gcc`
  - `jq`
  - `convert` (imagemagick)
- From this folder you can run the `run.sh` script

# Vanilla save

- Open the game
- Make sure that all mods are disables
- Close the game
- Run the `run.sh` script like this: `./run.sh save-name-that-you-want-a-map-for`
  - Note: This argument is also used as a folder name for the web that is going to be generated
  - If this folder already exists, it's going to be deleted
  - If Factorio doesn't find the save, you can load any save manually
  - Append `--png` if you want to use pngs instead of jpegs. (generating pngs is ~25% slower and results in ~4x larger images)
- Factorio will open, load the save and start taking screenshots.
- After capturing, FactorioFotograf will start generating lower zoom levels
- The final web is in your current directory (It's called the same as the save name)

# Modded save

- Open the game
- Make sure that all mods that you have in the save are enabled
- Close the game
- Continue as if this was a Vanilla save (From step 4)

# Steam version of Factorio
If you wish to use the steam version of Factorio, append the `run.sh` script call with the path of the steam factorio executable.  
For example, if your steam is installed into the default folder, the call would look like this:
```sh
./run save-name ~/.steam/steam/steamapps/common/Factorio/bin/x64/factorio
```
notes:
- Your steam folder might be on a different place if you installed steam form flatpak.
- You have to manually confirm that you want to start factorio with the command line arguments from FactorioFotograf.
- FactorioFotograf will fail to kill Factorio after the capture is done which is not a big deal. Just close the game manually (make sure not to save the game).

# `make: *** No rule to make target 'C-Thread-Pool/thpool.o', needed by 'imgmerge'.  Stop.`
# `./fotograf.sh: line 56: ./imgmerge: No such file or directory`
You did not clone the repository with the `--recursive` flag  
You can fix that by running:
```sh
git submodule init
git submodule update
```

