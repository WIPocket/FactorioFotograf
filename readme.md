# Factorio Fotograf

[![demo image](./demo.png)](https://rdck.dev/m/f/protab/)

Minimalistic Factorio map generator for Linux.

## Examples

### [Protab](https://rdck.dev/m/f/protab/)
- Larger vanilla map
- 2 minutes to do everything (including opening factorio & loading the save) (12 thread AMD Ryzen 5)
- (I manually removed 2 most-zoomed-in layers to save space)

### [Space Exploration map](https://rdck.dev/m/f/se/)
- Modded save example
- 1 minute to do everything (including opening factorio & loading the save) (12 thread AMD Ryzen 5)
- (I manually removed 2 most-zoomed-in layers to save space)

## Usage
`./ff [--help] -j$(nproc)`  
[Detailed usage guide](./docs/guide.md)

## Deps
Non-headless Factorio

(Arch: `yay -S factorio --needed`)  

## Features
- [x] Web
- [x] Compatible with other mods
- [x] Zooming in parallel
- [x] Position in the url
- [x] PNG & JPEG export
- [ ] Map time lapse capturing
- [ ] Multiple surfaces
- [ ] Custom png reader & writter for maximum performance

