# Factorio Fotograf
Minimalistic Factorio map generator for Linux.  
I assume that your Factorio folder is in `~/.factorio`

## Usage
Just run:  
`./run savename`  
If you want to optimize the generated map with `optipng`:  
`./optimize webfolder`  
usually saves around 15 %

## Deps
- Nonheadless Factorio runnable by `factorio`
- `optipng` and `parallel` for optimizing images

## Task list
- [x] Lua mod
- [x] Image merge tool
- [x] Web
- [ ] Generate lower zoom levels
- [ ] Smarter mod injection (Right now disables all other mods)
