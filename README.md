# LensShadingEditor

This is a very simple GUI tool to import and generate lens shading table for Raspberry Pi camera v1/v2.

See https://github.com/6by9/lens_shading on how to apply the generated table.

This program can import 6by9's lens_shading ch[1-4].bin files, and is able to export as C header or LUA table (python comming soon) for OV5647 (V1 camera), IMX219 (V2 camera) but also custom table size


## Dependencies

* Qt5
* OpenGL

For debian-based distributions (and probably other Linux distros) just install packages `cmake g++ qtbase5-dev libqt5opengl5-dev`

On Windows, installing Qt-Creator is the easiest way.

On OSX.. hmm Qt-Creator should be enough.


## Building

The program is entirely Qt5 based, and should be able to be built on any platform supporting it.

To build it, just do the following :
```
qmake
make
then run it using ./lensshading
```


## Prebuilt binaries

Comming soon...


## Screenshots

![screenshot](https://raw.githubusercontent.com/dridri/lens_shading_editor/master/screenshot.png)

![screenshot_load](https://raw.githubusercontent.com/dridri/lens_shading_editor/master/screenshot_load.png) ![screenshot_export](https://raw.githubusercontent.com/dridri/lens_shading_editor/master/screenshot_export.png)
