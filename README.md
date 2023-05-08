# Software Rasterizer



This project is a software rasterizer able to render 3D graphics in *real time (hopefully)* and save the rendered results to disk.



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

* Texture sampling with wrapping (repeat, mirror, clamp and black) and filtering (nearest and bi-linear)

![Texture sampling example](./ReadMe%20Resources/TextureSampling.bmp)



## Classes and structs

Here I will list all of the classes and structures found in the project and a short description of them. This software was designed in a modular way so I will also provide the module name before the class name.

* class BSR::Vector

This class is a container that has exactly the same functionality as the std::vector from STL. It has support for move semantics and it is templated.

* class BSR::Time::Timer

Used for elapsed time.

* struct BSR::Math::Vec2f

A simple 2 component point struct. It has operator overloading and helpers for convenience.

* struct BSR::Math::Vec3f

A simple 3 component point struct. It has operator overloading and helpers for convenience.

* struct BSR::Math::Vec4f

A simple 4 component point struct. It has operator overloading and helpers for convenience.

* class BSR::Math::Mat2f

This is a class for 2x2 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* class BSR::Math::Mat3f

This is a class for 3x3 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* class BSR::Math::Mat4f

This is a class for 4x4 matrices with helper functions to retrieve transformation matrices. A lot of operators were overloaded for convenience.

* struct BSR::Asset

Used to store an address to an asset and it's name.

* class BSR::AssetManager

Used to handle assets in order to avoid copying data all around the place.

* struct BSR::Image::Image

Data about an image.

* struct BSR::Image::ImageFloat

Stores an image with floating point components.

* struct BSR::WindowCreationDescriptor

A structure that packs all of the parameteres required for creating a window.

* class BSR::Window

A class that manages the threads for the grapthic windows used.

* class BSR::Rasterizer::Texture

Base class for any texture.

* class BSR::Rasterizer::Texture_R : public BSR::Rasterizer::Texture

A texture with 1 component.

* class BSR::Rasterizer::Texture_RG : public BSR::Rasterizer::Texture

A texture with 2 components.

* class BSR::Rasterizer::Texture_RGB : public BSR::Rasterizer::Texture

A texture with 3 components.

* class BSR::Rasterizer::Texture_RGBA : public BSR::Rasterizer::Texture

A texture with 4 components.

* class BSR::Rasterizer::Texture_Float_R : public BSR::Rasterizer::Texture

A texture with 1 floating point component.

* class BSR::Rasterizer::Texture_Float_RG : public BSR::Rasterizer::Texture

A texture with 2 floating point components.

* class BSR::Rasterizer::Texture_Float_RGB : public BSR::Rasterizer::Texture

A texture with 3 floating point components.

* class BSR::Rasterizer::Texture_Float_RGBA : public BSR::Rasterizer::Texture

A texture with 4 floating point components.

* class BSR::Rasterizer::Context

The thing that actually renders.

* struct BSR::Renderer::Material

Data structure that holds the material properties of a 3D model.

* struct BSR::Renderer::Light

The data of a light source.

* struct BSR::Renderer::Camera

Used to store and manage the position and orientation data from which to render the scene.

* struct BSR::Renderer::Transform

It is used to store the position and orientation of a 3D model.

* struct BSR::Renderer::VertexData

The properties of a point on the 3D model.

* class BSR::Renderer::VertexBuffer

A pool of verteces.

* struct BSR::Renderer::IndexData

A set of 3 indexs into the vertex buffer. Such a triplet forms a triangle.

* class BSR::Renderer::IndexBuffer

A pool of indexed triangles.

* struct BSR::Renderer::Mesh

Quick and dirty hack to pair up a VBO and an IBO.

* class BSR::Renderer::Model

A set of meshes that describe a 3D model.

* class BSR::Renderer::Instance

An instance of the renderer.

* class BSR::MultiProcessing::SharedMemory

Used to comunicate between processes.

* class BSR::MultiProcessing::SharedMutex

Used to comunicate between processes as well.

* class BSR::MultiProcessing::Process

Used to create and manage processes.

* class BSR::RunTime::Application

A class made for managing the runtime.

* class BSR::RunTime::Menu

For the runtime as well.

* struct BSR_APP::WindowData

Data used for the main window.

* class BSR_APP::RunTime::Application : public BSR::RunTime::Application

A class made for managing the runtime.

* class BSR_APP::RunTime::MainMenu : public BSR::RunTime::Menu

For the runtime as well.
