# Factorio Fotograf

[![demo image](./demo.png)](https://rdck.dev/m/f/protab/)

Minimalistic and easy to customize Factorio map generator for Linux in 398 lines of code.  
Generating requires almost no additional ram. If you can load the save, you can create a map from it.  

## Examples

### [Protab](https://rdck.dev/m/f/protab/)
- Larger vanilla map
- 2 minutes to do everything (including opening factorio & loading the save) (12 thread AMD Ryzen 5)
- (I manually removed 2 most-zoomed-in layers to save space)

### [Space Exploration map](https://rdck.dev/m/f/se/)
- Modded save example
- 1 minutes to do everything (including opening factorio & loading the save) (12 thread AMD Ryzen 5)
- (I manually removed 2 most-zoomed-in layers to save space)

## Usage
`./run save-name [--help]`  
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

