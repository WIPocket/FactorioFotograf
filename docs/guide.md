If you find something unclear, feel free to message me on discord (`Proicop#3264`), create issue or send a pull request.

# Dependencies

- Factorio (`/usr/bin/factorio` or specify with `--fac-bin PATH`)
- `make`
- `gcc`
- `python`
- Python pillow (`pip install pillow`)
- You have to clone the repo with the `--recursive` flag.

# Usage

- Open the game
- Make sure that all mods that you have in the save are enabled
- Close the game
- Run the `run` script like this: `./run --save factorio-save-name --output output-directory`
  - If Factorio can't find the save, you can load any save manually when Factorio is launched
  - Run `./run --help` for all available arguments
- Factorio will open, load the save and start taking screenshots.
- After capturing, FactorioFotograf will start generating lower zoom levels

# Steam version of Factorio

To run a steam version of Factorio, overwrite the the default factorio path by using the `--fac-bin BINARY` flag.
In case of steam Factorio, that would be `--fac-bin ~/.steam/steam/steamapps/common/Factorio/bin/x64/factorio`.

notes:
- Your path might be different if you installed steam form flatpak.
- You might have to manually confirm that you want to start Factorio with the command line arguments from FactorioFotograf.
- FactorioFotograf will fail to kill Factorio after the capture is done which is not a big deal. Just close the game manually (make sure not to save the game).

# `No rule to make target 'C-Thread-Pool/thpool.o', needed by 'imgmerge'`

You did not clone the repository with the `--recursive` flag  
You can fix that by running:
```sh
git submodule init
git submodule update
```
somewhere in the repository directory.

