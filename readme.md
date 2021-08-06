# Factorio Fotograf

[![demo image](./demo.png)](https://randacek.dev/m/f/protab/)

Minimalistic and easy to customize Factorio map generator for Linux in 364 lines of code.  
I assume that your Factorio folder is in `~/.factorio`

## Usage
`./run savename`  
Where `savename` is the save name. If you mistype the savename, just load the game manually when Factorio opens.  
If you are using mods make sure that you can load the save without syncing the mods. The same mods you have enabled right now + Fotograf will be used when loading the save.

## Example
[Protab](https://randacek.dev/m/f/protab/)  
State | Time | Ram usage peak (including OS)
------|------|-------------------------------
Capturing screenshots with Factorio (20k images) | 2 Minutes | 4 Gb
Creating zoom levels | 3 Minutes | 1 Gb
Optional: Converting to jpeg | 3.5 Minutes | 2 Gb

Raw png map size: 19 Gb.  
Jpeg map size 0.8 Gb.  

[Space Exploration map](https://randacek.dev/m/f/se/)  
Compatible with mods

## Image optimization
Script       | Description                       | %save | Time
-------------|-----------------------------------|-------|----------
`minifypng`  | PNG optimization                  | ~30 % | slow
`minifypng2` | PNG quantization and optimization | ~70 % | slower
`2jpg`       | Converts the PNGs to JPEGs        | ~75 % | fast

`yay -S parallel optipng imagemagick pngnq-s9 --needed`

## Deps
Non-headless Factorio runnable by `factorio` (`yay -S factorio` or [download here](https://factorio.com/download) and then add the binary to PATH)  
`jq`, `convert` (imagemagick), `gcc`

## TODO
- [x] Lua mod
- [x] Image merge tool
- [x] Web
- [x] Generate lower zoom levels
- [x] Compatible with other mods
- [x] Zooming in parallel
- [ ] Position in the url
- [ ] Include world download in the web
- [ ] Map time lapse capturing
- [ ] Multiple surfaces
- [ ] Optionally capture in jpg right away instead of converting later
- [ ] Achieve optimal C code
  - [ ] Own image resize
  - [ ] Import image bitmap straight into the final position in ram to avoid copying

