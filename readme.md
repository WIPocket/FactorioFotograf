# Factorio Fotograf

[![demo image](./demo.png)](https://rdck.dev/m/f/protab/)

Minimalistic Factorio map generator for Linux.
Generating requires almost no additional ram. If you can load the save, you can create a map from it.  

## Examples

### [Protab](https://rdck.dev/m/f/protab/)
- Larger vanilla map
- 1+3 minutes to generate and zoomout on 12 thread 2Ghz cpu
- Heavy jpg compression (took 2 minutes) using [`util/2jpg`](util/2jpg) (1.1 Gb - 12000 1024x1024 images)

### [Space Exploration map](https://rdck.dev/m/f/se/)
- Modded save example
- 0.5 + 1 minutes to generate and zoomout on 12 thread 2Ghz cpu
- Heavy jpg compression (took 40 seconds) using [`util/2jpg`](util/2jpg) (336 Mb - 3390 1024x1024 images)

## Usage
`./run [--save FACTORIO-SAVE-NAME] [--output OUTPUT-DIRECTORY] [--help]`  
[Detailed usage guide](./docs/guide.md)

## Deps
Non-headless Factorio, C compiler, Python3, Python Pillow.

(Arch: `yay -S factorio python gcc --needed`)  

## Features
- [x] Web
- [x] Compatible with other mods
- [x] Zooming in parallel
- [x] Position in the url
- [x] PNG & JPEG export
- [ ] Map time lapse capturing
- [ ] Multiple surfaces
- [ ] Read image bitmap straight into the final position in ram to avoid copying

