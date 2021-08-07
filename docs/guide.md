# Common instructions:

- Download the code
  - `git clone --recursive https://github.com/ProkopRandacek/FactorioFotograf.git`
  - Note: You cannot just download the zip from Github because that won't download the submodules
- Make sure that you have:
  - `make`
  - `gcc`
  - `jq`
  - `convert` (imagemagick)
  - Archlinux: `pacman -S jq gcc make imagemagick --needed`
  - Fedora: `dnf install jq gcc make imagemagick`
  - Ubuntu: `apt-get install jq gcc make imagemagick`
- Go to the repository root folder
- From here you can run the `run.sh` script


# Usage 1: Vanilla save, PNG

- Open the game
- Make sure that all mods are disables
- Close the game
- Run the `run.sh` script like this: `./run.sh save-name-that-you-want-a-map-for`
  - Note: This argument is also used as a folder name for the web that is going to be generated
  - If this folder already exists, it's going to be deleted
  - If Factorio doesn't find the save, you can load any save manually
- Factorio will open and load the save
- As soon as the save loads, FactorioFotograf mod will freeze the game and start capturing the map
- You can see the progress in in the `run.sh` script output
- When the capturing finishes, Factorio will close
  - Note: Game is **not** saved.
- After capturing, FactorioFotograf will start generating lower zoom levels
  - Note: You can track the progress by the number in the `run.sh` script's output `.../image/0/X...`
  - X goes from 8 to 1 and every number is ~4x faster than the previous one
- The final web is in your current directory (It's called the same as the save name)

# Converting from PNG to JPEG

- Go to the repo root
- `./util/2jpg /path/to/your/generated/web/dir/`

# Optimizing PNGS

- Go to the repo root
- `./util/minifypng /path/to/your/generated/web/dir/`

# Optimizing PNGS more

- Go to the repo root
- `./util/minifypng2 /path/to/your/generated/web/dir/`
