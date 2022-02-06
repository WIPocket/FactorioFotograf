If you find something unclear, feel free to message me on discord (`Proicop#3264`), create issue or send a pull request.

# Instruction

- [Download the latest release binary](https://github.com/ProkopRandacek/FactorioFotograf/releases/)
- Make sure that you have factorio (not headless)
- Manually open Factorio
- Make sure that all mods that are in the target save are enabled
- Close the game
- Run the `ff` binary that you downloaded
  - Note: run `./ff --help` for all available arguments
- Factorio will open
- Manutall open the save you want to make a map from
- FactorioFotograf will start capturing screenshots
- After capturing, FactorioFotograf will start generating lower zoom levels
- The final web is in `.factorio/script-output/FF`

# Steam version of Factorio
If you wish to use the steam version of Factorio, append the `run` script call with the path of the steam factorio executable.
For example, if your steam is installed into the default folder, the call would look like this:
```sh
./ff --fac-bin ~/.steam/steam/steamapps/common/Factorio/bin/x64/factorio
```
notes:
- Your steam folder might be on a different place if you installed steam form flatpak.
- You might have to manually confirm that you want to start Factorio with the command line arguments from FactorioFotograf.
- FactorioFotograf will fail to kill Factorio after the capture is done which is not a big deal. Just close the game manually (make sure not to save the game).

