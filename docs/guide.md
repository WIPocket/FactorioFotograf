# Instructions

- [Download the latest release binary](https://github.com/ProkopRandacek/FactorioFotograf/releases/) (alternatively you can build it from source)
- Make sure that you have factorio (not headless)
- Manually open Factorio
- Make sure that all mods that are in the target save are enabled (none if none)
- Close the game
- Run the FF binary that you downloaded
  - `./ff -j12` will spawn 12 worker threads
  - `./ff --help` shows all available arguments
- Factorio will open
- Manually open the save you want to make a map from
- FactorioFotograf will start capturing screenshots when the save loads (The game will be frozen)
- When capturing is done, Factorio will be closed and FF will start zooming out the images
- The final web is in `~/.factorio/script-output/FF`

# Steam version of Factorio

If you want to use the steam version of Factorio, you have to specify the path of the steam Factorio executable.
For example, if your steam is installed into the default folder, the call would look like this:
```sh
./ff --fac-bin ~/.steam/steam/steamapps/common/Factorio/bin/x64/factorio
```
notes:
- Your path might be different if you installed steam form flatpak.
- You might have to manually confirm that you want to start Factorio with the command line arguments from FactorioFotograf.
- FactorioFotograf will probably fail to kill Factorio after the capture is done which is not a big deal. Just close the game manually (make sure not to save the game).

