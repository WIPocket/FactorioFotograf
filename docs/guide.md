# Common instructions:

- Download the code
  - `git clone --recursive https://github.com/ProkopRandacek/FactorioFotograf.git`
  - Note: You cannot just download the zip from Github because that won't download the submodules
- Make sure that you have:
  - `make`
  - `gcc`
  - `jq`
  - `convert` (imagemagick)
- Go to the repository root folder
- From here you can run the `run.sh` script


# Usage 1: Vanilla save, PNG

- Open the game
- Make sure that all mods are disables
- Close the game
- Get the source
- Run the `run.sh` script. As argument, use the save name you want to create a map of
  - Note: This argument is also used as a folder name for the web that is going to be generated
  - If this folder already exists, it's going to be deleted
  - If Factorio doesn't find the save, you can load any save manually
- Factorio will open and load the save
- As soon as the save loads, FactorioFotograf mod will trigger, freeze the game and start capturing the map
- You can see a progress bar in in the `run.sh` script output
- When the capturing finishes, Factorio will be killed.
  - Note: Game is **not** saved.
- After capturing, FactorioFotograf will compile the custom image processing tool and use it to generate zoom levels for your map
  - Note: You can track the progress by the number in the `run.sh` script's output `.../image/0/X...`
  - Number go from 8 to 1 and every number is ~4x faster than the previous one
- After the zoom levels generations, the map is done
- Output directory location is written by the `run.sh` script

# Converting from PNG to JPEG

- Go to the repo root
- `./util/2jpg /path/to/your/generated/web/dir/`

# Optimizing PNGS

- Go to the repo root
- `./util/minifypng /path/to/your/generated/web/dir/`

# Optimizing PNGS more

- Go to the repo root
- `./util/minifypng2 /path/to/your/generated/web/dir/`
