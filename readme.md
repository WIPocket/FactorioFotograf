# Factorio Fotograf
Minimalistic Factorio map generator for Linux.  
I assume that your Factorio folder is in `~/.factorio`

## Usage
Just run:  
`./run savename`  
If you want to optimize the generated map with `optipng`:  
`./optimize webfolder`  
usually saves around 20 %

## Deps
- Non-headless Factorio runnable by `factorio` (`yay -S factorio` or [download here](https://factorio.com/download) and then add the binary to PATH)
- `optipng` and `parallel` for optimizing images

## Task list
- [x] Lua mod
- [x] Image merge tool
- [x] Web
- [ ] Generate lower zoom levels
- [ ] Smarter mod injection (Right now disables all other mods)
- [ ] Compatible with other mods
