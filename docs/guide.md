If you find something unclear, feel free to message me on discord (`Proicop#3264`), create issue or send a pull request.

# Setup instruction

- Download the code
  - `git clone --recursive https://github.com/ProkopRandacek/FactorioFotograf.git`
  - Note: You cannot just download the zip from Github because that won't download the submodules
- Make sure that you have:
  - `standalone factorio`
  - `make`
  - `gcc`
  - `jq`
  - `convert` (imagemagick)
- The `run.sh` script is in the repository root

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

