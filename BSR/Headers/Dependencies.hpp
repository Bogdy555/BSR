#ifndef BSR_Dependencies_hpp

#define BSR_Dependencies_hpp



#pragma comment(lib, "WinMM.lib")



#pragma pack(pop)



#include "WinTargetVer.hpp"
#include <Windows.h>



#include <iostream>
#include <fstream>

#include <cmath>

#include <vector>
#include <sstream>
#include <memory>

#include <chrono>

#include <thread>
#include <mutex>
#include <future>



#pragma pack(push, 1)



#include "Macros.hpp"



namespace BSR
{

	enum States : const size_t
	{
		_Previous = 0,
		_Current = 1
	};

	template <typename T> class Vector;

	namespace Time
	{

		class Timer;

	}

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

	struct WindowCreationDescriptor;

	class Window;

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

		enum CullingTypes : const uint8_t;

		enum DepthTestingTypes : const uint8_t;

		enum BlendingTypes : const uint8_t;

		typedef const Math::Vec4f (*VertexShaderFnc)(const void* _Vertex, const void* _Uniforms, float* _OutLerpers);

		typedef const bool (*GeometryShaderFnc)(const Math::Vec4f& _APosition, const Math::Vec4f& _BPosition, const Math::Vec4f& _CPosition, const float* _ALerpers, const float* _BLerpers, const float* _CLerpers, const void* _Uniforms, std::vector<Math::Vec4f>& _PositionsOut, std::vector<float*>& _LerpersOut);

		typedef void (*FragmentShaderFnc)(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType);

		class Context;

	}

	namespace Renderer
	{

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

		class Instance;

	}

	namespace MultiProcessing
	{

		enum ReturnValues : const int32_t;

		class SharedMemory;

		class SharedMutex;

		class Process;

	}

	namespace RunTime
	{

		class Application;

		enum MenuTypes : const uint64_t;

		class Menu;

	}

}



#include "Templates.hpp"



#endif
