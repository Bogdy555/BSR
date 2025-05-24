#ifndef BSR_Dependencies_hpp

#define BSR_Dependencies_hpp



#ifdef _WIN32

#include <cstdint>

#include <cmath>

#include <cstring>

#endif



#ifdef __linux__

#include <cstdint>

#include <cmath>

#include <cstring>

#endif



#ifdef __AVR__

#include <stdint.h>

#include <math.h>

#include <string.h>

#endif



#ifdef ESP32

#include <stdint.h>

#include <math.h>

#include <string.h>

#endif



namespace BSR
{

	template <typename T> class Vector;

	namespace Math
	{

		class Vec2f;

		class Vec3f;

		class Vec4f;

		class Mat2f;

		class Mat3f;

		class Mat4f;

	}

	namespace Image
	{

		struct SDR;

		struct HDR;

	}

	namespace Rasterizer
	{

		enum LerpingTypes : const uint8_t;

		enum WrappingTypes : const uint8_t;

		class TextureSDR;

		class TextureHDR;

		enum CullingTypes : const uint8_t;

		enum DepthTestingTypes : const uint8_t;

		enum BlendingTypes : const uint8_t;

		typedef const Math::Vec4f (*VertexShaderFnc)(const void* _Vertex, const void* _Uniforms, float* _OutLerpers);

		typedef void (*GeometryShaderFnc)(const Math::Vec4f& _APosition, const Math::Vec4f& _BPosition, const Math::Vec4f& _CPosition, const float* _ALerpers, const float* _BLerpers, const float* _CLerpers, const void* _Uniforms, Vector<Math::Vec4f>& _PositionsOut, Vector<float*>& _LerpersOut);

		typedef void (*FragmentShaderFnc)(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType);

		class Context;

	}

	namespace Renderer
	{

		struct VertexData;

		typedef Vector<VertexData> VertexBuffer;

		struct IndexData;

		typedef Vector<IndexData> IndexBuffer;

		struct Mesh;

		typedef Vector<Mesh> Model;

		struct Camera;

		struct Transform;

		enum LightTypes : const uint8_t;

		struct Light;

		enum FogTypes : const uint8_t;

		struct PBRMaterial;

		struct PBRFrameBuffer;

		class PBRContext;

		struct BlinnPhongMaterial;

		struct BlinnPhongFrameBuffer;

		class BlinnPhongContext;

	}

}



#include "BSR_Templates.hpp"



#endif
