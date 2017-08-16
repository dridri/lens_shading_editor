# LensShadingEditor

This is a very simple GUI tool to generate lens shading table for Raspberry Pi camera v1/v2.

See https://github.com/6by9/lens_shading on how to apply the generated table.


## Building

The program is entirely Qt5 based, and should be able to be built on any platform supporting it.

To build it, just do the following :
```
mkdir build && cd build
cmake ..
make
[make install] or just run it
```


![screenshot](https://raw.githubusercontent.com/dridri/lens_shading_editor/master/screenshot.png)
