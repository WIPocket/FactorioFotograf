# Factorio Fotograf

[![demo image](./demo.png)](https://randacek.dev/m/f/protab/)

Minimalistic and easy to customize Factorio map generator for Linux in under 300 loc.  
I assume that your Factorio folder is in `~/.factorio`

## Usage
`./run savename`  
Where `savename` is the save name. If you mistype the savename, just load the game manually when Factorio opens.  
If you are using mods make sure that you can load the save without syncing the mods. The same mods you have enabled right now + Fotograf will be used when loading the save.

## Example
[Protab](https://randacek.dev/m/f/protab/)  
Taking the screenshots took ~1.5 minutes. Creating lower zoom levels took another 20 minutes. After that the map was 15 GB in size. Converting to jpg took 4 minutes and the final map has 3.5 GB.  
[Space Exploration map](https://randacek.dev/m/f/se/)  
Compatible with mods

## Image optimization
Script      | Description               | %save | Time
------------|---------------------------|-------|-----
`minifypng` | Optimizes the pngs        | ~30 % | slow
`2jpg`      | Converts the pngs to jpgs | ~75 % | fast

`yay -S parallel optipng imagemagick --needed`

## Deps
Non-headless Factorio runnable by `factorio` (`yay -S factorio` or [download here](https://factorio.com/download) and then add the binary to PATH)  
`jq`, `convert`, `gcc`

## TODO
- [x] Lua mod
- [x] Image merge tool
- [x] Web
- [x] Generate lower zoom levels
- [ ] Include world download in the web
- [ ] Zooming in parallel
- [ ] Position in the url
- [ ] Map timelapse capturing
- [ ] Multiple surfaces
- [ ] Optionally capture in jpg right away instead of converting later
- [x] Compatible with other mods
- [ ] Achieve optimal C code
  - [ ] Own image resize
  - [ ] Import image bitmap straight into the final position in ram to avoid copying

