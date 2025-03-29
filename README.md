# Bogdan Software Rasterizer (BSR)



This project is a software rasterizer able to render 3D graphics in real time and save the rendered results to disk.



## Requirements

The only supported platform is Windows (7 all the way up to 11 on both 32 and 64 bit architectures).

It is necessary to be compiled with MSVC. The C++ version used is C++20.



## Getting started

It works out of the box since I only use relative paths and the project does not have external dependencies.



## Controls

W, A, S, D, Shift and Space move the camera.

The arrow keys, F7 and F8 rotate your head around.

F5 renders the scene with PBR and saves it to disk.

F6 swaps projection type.

F3 decreases the FOV.

F4 increases the FOV.

Escape closes the app.

F11 toggles full screen on or off.



## Copy right

I am not the author of the amazing PBR materials, nor the HDR sIBL environment maps. I got them from https://freepbr.com and http://www.hdrlabs.com/sibl/archive.html. I only use them to test my software and I do not own any ownership over them.

The actual software is free for anyone as long as it is not used commercially, but as an educational resource or an external dependency for personal projects. With that said, I do not assume responsibility for anything that might happen because of using this software. All rights reserved.



## Features

* Rasterizer (fully featured)

![Render example](./ReadMe%20Resources/Render.bmp)

* Deferred renderer with PBR pipeline

![Deferred example](./ReadMe%20Resources/Deferred.bmp)

* Real time Phong

![Phong example](./ReadMe%20Resources/Phong.bmp)

* Texture sampling with wrapping (repeat, mirror, clamp and black) and filtering (nearest and bi-linear)

![Texture sampling example](./ReadMe%20Resources/TextureSampling.bmp)



## What I would like to add (if I have enough patience)

* Cascaded shadow maps with PCSS and PCF

* SSR

* SSAO

* Allow the fog to have the color of the env map

* Allow texture wrapping and lerping on individual component

* Depth of field

* Orbit camera

* Focal length

* Fish eye lens

* MSAA

* Scene Hierarchy

* Improve Model loader (optimize vertex pool)

* Load mtllib as well

* Save compressed Model

* Add Transform and Material on Mesh

* Add Transform on Model

* Generate iradiance map on startup

* Generate BRDF LUT on startup

* Generate prefiltered convoluted maps on startup

* Check for bugs in the composit fragment shader, PBR related math functions and light calculations
