# Software Rasterizer (Florescu Bogdan - 143)



This project is a software rasterizer able to render 3D graphics in *real time (hopefully)* and save the rendered results to disk.



## Getting started

To compile the project use the script provided in the root folder called "C++98.bat". In order for it to work **GCC has to be added to the path and the line that calls "gccvarsx64.bat" should be removed**.

I usually develop in VS, but it does not have support for C++ 98 so I had to resume to this method so it is what it is.

It should work out of the box since I only use relative paths and the project does not have external dependencies like 3-rd party libraries.



## Classes and structs

Here I will list all of the classes and structures found in the project and a short description of them. This software was designed in a modular way so I will also provide the module name before the class name.

* class Vector

This class is a container that has exactly the same functionality as the std::vector from STL.

* struct Math::Vec2f

A simple 2 component point struct. It has operator overloading and helpers for convenience.

* struct Math::Vec3f

A simple 3 component point struct. It has operator overloading and helpers for convenience.

* struct Math::Vec4f

A simple 4 component point struct. It has operator overloading and helpers for convenience.

* class Math::Mat2f

This is a class for 2x2 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* class Math::Mat3f

This is a class for 3x3 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* class Math::Mat4f

This is a class for 4x4 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* struct Asset

Used to store an address to an asset and it's name.

* class AssetManager

Used to handle assets in order to avoid copying data all around the place.

* struct Image::ImageSRGBA

Data about an image on the sRGB color space with alpha channel.

* struct Image::ImageHDR

Stores a high dynamic range image.

* struct Geometry::VertexData

The properties of a point on the 3D model.

* class Geometry::VertexBuffer

A pool of verteces.

* struct Geometry::IndexData

A set of 3 indexs into the vertex buffer. Such a triplet forms a triangle.

* class Geometry::IndexBuffer

A pool of indexed triangles.

* struct Geometry::Mesh

Quick and dirty hack to pair up a VBO and an IBO.

* class Geometry::Model

A set of meshes that describe a 3D model.



## Features

Currently the features that are available are the math lib and the asset manager and loader that can load 3D models and textures.
