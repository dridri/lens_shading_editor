# LensShadingEditor

This is a very simple GUI tool to generate lens shading table for Raspberry Pi camera v1/v2.

See https://github.com/6by9/lens_shading on how to apply the generated table.


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


![screenshot](https://raw.githubusercontent.com/dridri/lens_shading_editor/master/screenshot.png)
