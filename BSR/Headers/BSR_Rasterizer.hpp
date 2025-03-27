#ifndef BSR_Rasterizer_hpp

#define BSR_Rasterizer_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace Rasterizer
	{

		enum LerpingTypes : const uint8_t
		{
			_LerpNearest = 0,
			_LerpLinear = 1
		};

		enum WrappingTypes : const uint8_t
		{
			_WrapBlack = 0,
			_WrapClamp = 1,
			_WrapRepeat = 2,
			_WrapMirror = 3
		};

		class TextureSDR
		{

		public:

			Vector<Image::SDR> TextureChain;
			size_t ChannelsCount = 0;
			uint8_t LerpType = _LerpNearest;
			uint8_t WrapType = _WrapBlack;

			const Math::Vec4f Sample(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;

		private:

			const Math::Vec4f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

		};

		class TextureHDR
		{

		public:

			Vector<Image::HDR> TextureChain;
			size_t ChannelsCount = 0;
			uint8_t LerpType = _LerpNearest;
			uint8_t WrapType = _WrapBlack;

			const Math::Vec4f Sample(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;

		private:

			const Math::Vec4f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

		};

		enum CullingTypes : const uint8_t
		{
			_NoCulling = 0,
			_ClockWiseCulling = 1,
			_CounterClockWiseCulling = 2
		};

		enum DepthTestingTypes : const uint8_t
		{
			_NoDepthTesting = 0,
			_LowerDepthTesting = 1,
			_HigherDepthTesting = 2,
			_LowerOrEqualDepthTesting = 3,
			_HigherOrEqualDepthTesting = 4
		};

		enum BlendingTypes : const uint8_t
		{
			_NoBlending = 0,
			_AditiveBlending = 1,
			_AlphaBlending = 2
		};

		typedef const Math::Vec4f (*VertexShaderFnc)(const void* _Vertex, const void* _Uniforms, float* _OutLerpers);

		typedef void (*GeometryShaderFnc)(const Math::Vec4f& _APosition, const Math::Vec4f& _BPosition, const Math::Vec4f& _CPosition, const float* _ALerpers, const float* _BLerpers, const float* _CLerpers, const void* _Uniforms, Vector<Math::Vec4f>& _PositionsOut, Vector<float*>& _LerpersOut);

		typedef void (*FragmentShaderFnc)(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType);

		class Context
		{

		public:

			size_t ViewPortX = 0;
			size_t ViewPortY = 0;
			size_t ViewPortWidth = 0;
			size_t ViewPortHeight = 0;
			uint8_t CullingType = _NoCulling;
			uint8_t DepthTestingType = _NoDepthTesting;
			uint8_t BlendingType = _NoBlending;

			const void* VBO = nullptr;
			size_t VBOSize = 0;
			size_t VBOStride = 0;

			const size_t* IBO = nullptr;
			size_t IBOBegin = 0;
			size_t IBOEnd = 0;

			const void* Uniforms = nullptr;
			VertexShaderFnc VertexShader = nullptr;
			size_t LerpersCountVertToGeom = 0;
			GeometryShaderFnc GeometryShader = nullptr;
			size_t LerpersCountGeomToFrag = 0;
			FragmentShaderFnc FragmentShader = nullptr;

			void* FrameBuffer = nullptr;

			void DrawCall();

			static const bool DepthTest(const float _NewDepth, const float _OldDepth, const uint8_t _DepthTestingType);
			static const Math::Vec3f Blend(const Math::Vec3f& _OldColor, const Math::Vec4f& _Color, const uint8_t _BlendingType, const bool _Clamp = true);

		private:

			struct PointData
			{
				Math::Vec4f Position = Math::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
				float* Lerpers = nullptr;
			};

			struct Shape
			{
				bool Render = true;
				PointData A;
				PointData B;
				PointData C;
			};

			void VertexPass(Vector<PointData> &_Output);
			void GeometryPass(const Vector<PointData>& _Input, Vector<Shape>& _Output);
			void ClippingPass(Vector<Shape>& _Shapes);
			void CullingPass(Vector<Shape>& _Shapes);
			void RasterizingPass(Vector<Shape>& _Shapes);

			static const bool InsideNearPlane(const float _Z, const float _W);
			static const float GetTNearPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn);
			static const bool InsideFarPlane(const float _Z, const float _W);
			static const float GetTFarPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn);
			static void LerpAll(const float* _A, const float* _B, const size_t _LerpersCount, const float _Percentage, float* _Out);
			static void CopyAll(const float* _Src, float* _Dest, const size_t _LerpersCount);
			static void MultiplyAll(float* _Out, const size_t _LerpersCount, const float _Value);
			static const bool PointInside(const Math::Vec2f& _P, const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C);
			static const float GetT1(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P);
			static const float GetT2(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P, const float _T1);

		};

	}

}



#endif
