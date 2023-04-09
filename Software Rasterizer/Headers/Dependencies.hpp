#ifndef Dependencies_hpp

#define Dependencies_hpp



#pragma pack(pop)



#include "WinTargetVer.hpp"
#include <Windows.h>



#include <iostream>
#include <fstream>

#include <cmath>

#include <vector>
#include <sstream>



#pragma pack(push, 1)



#include "Macros.hpp"



template <typename T> class Vector;

namespace String
{



}

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

	struct Image;

	struct ImageFloat;

}

namespace Rasterizer
{

	enum LerpTypes : const uint8_t;

	enum WrapTypes : const uint8_t;

	class Texture;

	class Texture_R;

	class Texture_RG;

	class Texture_RGB;

	class Texture_RGBA;

	class Texture_Float_R;

	class Texture_Float_RG;

	class Texture_Float_RGB;

	class Texture_Float_RGBA;

	struct Material;

	enum LightTypes : const uint8_t;

	struct Light;

	struct Camera;

	struct Transform;

	struct VertexData;

	class VertexBuffer;

	struct IndexData;

	class IndexBuffer;

	struct Mesh;

	class Model;

	enum CullingTypes : uint8_t;

	enum DepthTestingTypes : uint8_t;

	enum BlendingTypes : uint8_t;

	typedef const Math::Vec4f (*VertexShaderFnc)(const VertexData& _Vertex, const void* _Uniforms, float* _OutLerpers);

	class Context;

}



#include "Templates.hpp"



#endif
