#ifndef Dependencies_hpp

#define Dependencies_hpp



#pragma pack(pop)



#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif

#ifndef _UNICODE

#define _UNICODE

#endif

#ifndef UNICODE

#define UNICODE

#endif

#define WINVER _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <winsdkver.h>
#include <sdkddkver.h>

#include <Windows.h>



#include <iostream>
#include <fstream>
#include <cstdio>

#include <cassert>

#include <cmath>

#include <cstring>
#include <sstream>



#pragma pack(push, 1)



#include "Macros.hpp"



template <typename T> class Vector;

namespace Math
{

	struct Vec2f;

	struct Vec3f;

	struct Vec4f;

	class Mat2f;

	class Mat3f;

	class Mat4f;

}

struct Asset;

class AssetManager;

namespace Image
{

	struct ImageSRGBA;

	struct ImageHDR;

}

namespace Geometry
{

	struct VertexData;

	class VertexBuffer;

	struct IndexData;

	class IndexBuffer;

	struct Mesh;

	class Model;

}



#include "Templates.hpp"



#endif
