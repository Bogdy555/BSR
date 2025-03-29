#ifndef BSR_Rasterizer_hpp

#define BSR_Rasterizer_hpp



#include "BSR.hpp"



#define BSR_OBJ_MAX_LINE_LEN 200



namespace BSR
{

	namespace Rasterizer
	{

		enum LerpTypes : const uint8_t
		{
			_LerpNearest = 0,
			_LerpLinear = 1
		};

		enum WrapTypes : const uint8_t
		{
			_WrapBlack = 0,
			_WrapClamp = 1,
			_WrapRepeat = 2,
			_WrapMirror = 3
		};

		class Texture
		{

		public:

			Texture();
			Texture(const Texture& _Other) = delete;
			Texture(Texture&& _Other) noexcept;
			virtual ~Texture();

			virtual const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const = 0;

			void SetLerpType(const uint8_t _LerpType);
			void SetWrapType(const uint8_t _WrapType);

			const uint8_t GetLerpType() const;
			const uint8_t GetWrapType() const;

			void operator= (const Texture& _Other) = delete;
			void operator= (Texture&& _Other) noexcept;

		protected:

			uint8_t LerpType;
			uint8_t WrapType;

		};

		class Texture_R : public Texture
		{

		public:

			Texture_R();
			Texture_R(const Texture_R& _Other) = delete;
			Texture_R(Texture_R&& _Other) noexcept;
			~Texture_R();

			const float SampleR(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::Image& _Image);
			void AddDirectMip(Image::Image&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::Image& operator[] (const size_t _Index);
			const Image::Image& operator[] (const size_t _Index) const;

			void operator= (const Texture_R& _Other) = delete;
			void operator= (Texture_R&& _Other) noexcept;

		private:

			const float SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::Image> Textures;

		};

		class Texture_RG : public Texture
		{

		public:

			Texture_RG();
			Texture_RG(const Texture_RG& _Other) = delete;
			Texture_RG(Texture_RG&& _Other) noexcept;
			~Texture_RG();

			const Math::Vec2f SampleRG(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::Image& _Image);
			void AddDirectMip(Image::Image&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::Image& operator[] (const size_t _Index);
			const Image::Image& operator[] (const size_t _Index) const;

			void operator= (const Texture_RG& _Other) = delete;
			void operator= (Texture_RG&& _Other) noexcept;

		private:

			const Math::Vec2f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::Image> Textures;

		};

		class Texture_RGB : public Texture
		{

		public:

			Texture_RGB();
			Texture_RGB(const Texture_RGB& _Other) = delete;
			Texture_RGB(Texture_RGB&& _Other) noexcept;
			~Texture_RGB();

			const Math::Vec3f SampleRGB(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::Image& _Image);
			void AddDirectMip(Image::Image&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::Image& operator[] (const size_t _Index);
			const Image::Image& operator[] (const size_t _Index) const;

			void operator= (const Texture_RGB& _Other) = delete;
			void operator= (Texture_RGB&& _Other) noexcept;

		private:

			const Math::Vec3f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::Image> Textures;

		};

		class Texture_RGBA : public Texture
		{

		public:

			Texture_RGBA();
			Texture_RGBA(const Texture_RGBA& _Other) = delete;
			Texture_RGBA(Texture_RGBA&& _Other) noexcept;
			~Texture_RGBA();

			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::Image& _Image);
			void AddDirectMip(Image::Image&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::Image& operator[] (const size_t _Index);
			const Image::Image& operator[] (const size_t _Index) const;

			void operator= (const Texture_RGBA& _Other) = delete;
			void operator= (Texture_RGBA&& _Other) noexcept;

		private:

			const Math::Vec4f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::Image> Textures;

		};

		class Texture_Float_R : public Texture
		{

		public:

			Texture_Float_R();
			Texture_Float_R(const Texture_Float_R& _Other) = delete;
			Texture_Float_R(Texture_Float_R&& _Other) noexcept;
			~Texture_Float_R();

			const float SampleR(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::ImageFloat& _Image);
			void AddDirectMip(Image::ImageFloat&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::ImageFloat& operator[] (const size_t _Index);
			const Image::ImageFloat& operator[] (const size_t _Index) const;

			void operator= (const Texture_Float_R& _Other) = delete;
			void operator= (Texture_Float_R&& _Other) noexcept;

		private:

			const float SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::ImageFloat> Textures;

		};

		class Texture_Float_RG : public Texture
		{

		public:

			Texture_Float_RG();
			Texture_Float_RG(const Texture_Float_RG& _Other) = delete;
			Texture_Float_RG(Texture_Float_RG&& _Other) noexcept;
			~Texture_Float_RG();

			const Math::Vec2f SampleRG(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::ImageFloat& _Image);
			void AddDirectMip(Image::ImageFloat&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::ImageFloat& operator[] (const size_t _Index);
			const Image::ImageFloat& operator[] (const size_t _Index) const;

			void operator= (const Texture_Float_RG& _Other) = delete;
			void operator= (Texture_Float_RG&& _Other) noexcept;

		private:

			const Math::Vec2f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::ImageFloat> Textures;

		};

		class Texture_Float_RGB : public Texture
		{

		public:

			Texture_Float_RGB();
			Texture_Float_RGB(const Texture_Float_RGB& _Other) = delete;
			Texture_Float_RGB(Texture_Float_RGB&& _Other) noexcept;
			~Texture_Float_RGB();

			const Math::Vec3f SampleRGB(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const;
			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::ImageFloat& _Image);
			void AddDirectMip(Image::ImageFloat&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::ImageFloat& operator[] (const size_t _Index);
			const Image::ImageFloat& operator[] (const size_t _Index) const;

			void operator= (const Texture_Float_RGB& _Other) = delete;
			void operator= (Texture_Float_RGB&& _Other) noexcept;

		private:

			const Math::Vec3f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::ImageFloat> Textures;

		};

		class Texture_Float_RGBA : public Texture
		{

		public:

			Texture_Float_RGBA();
			Texture_Float_RGBA(const Texture_Float_RGBA& _Other) = delete;
			Texture_Float_RGBA(Texture_Float_RGBA&& _Other) noexcept;
			~Texture_Float_RGBA();

			const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel = 0.0f) const override;

			bool AddMip(const Image::ImageFloat& _Image);
			void AddDirectMip(Image::ImageFloat&& _Image);
			void RemoveMip(const size_t _Index);
			void RemoveAllMips();

			const size_t GetMipsCount() const;

			Image::ImageFloat& operator[] (const size_t _Index);
			const Image::ImageFloat& operator[] (const size_t _Index) const;

			void operator= (const Texture_Float_RGBA& _Other) = delete;
			void operator= (Texture_Float_RGBA&& _Other) noexcept;

		private:

			const Math::Vec4f SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const;

			std::vector<Image::ImageFloat> Textures;

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

		typedef const bool (*GeometryShaderFnc)(const Math::Vec4f& _APosition, const Math::Vec4f& _BPosition, const Math::Vec4f& _CPosition, const float* _ALerpers, const float* _BLerpers, const float* _CLerpers, const void* _Uniforms, std::vector<Math::Vec4f>& _PositionsOut, std::vector<float*>& _LerpersOut);

		typedef void (*FragmentShaderFnc)(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType);

		class Context
		{

		public:

			Context();
			Context(const Context& _Other);
			Context(Context&& _Other) noexcept;
			~Context();

			const bool RenderMesh(const void* _VBO, const size_t _VBOSize, const size_t _VBOStride, const size_t* _IBO, const size_t _IBOBegin, const size_t _IBOEnd, const void* _Uniforms, const size_t _LerpersCountVertToGeom, const size_t _LerpersCountGeomToFrag, const VertexShaderFnc _VertexShader, const GeometryShaderFnc _GeometryShader, const FragmentShaderFnc _FragmentShader, void* _FrameBuffer) const;

			void SetViewPort(const size_t _ViewPortX, const size_t _ViewPortY, const size_t _ViewPortWidth, const size_t _ViewPortHeight);
			void SetCullingType(const uint8_t _CullingType);
			void SetDepthTestingType(const uint8_t _DepthTestingType);
			void SetBlendingType(const uint8_t _BlendingType);

			void GetViewPort(size_t& _ViewPortX, size_t& _ViewPortY, size_t& _ViewPortWidth, size_t& _ViewPortHeight) const;
			const uint8_t GetCullingType() const;
			const uint8_t GetDepthTestingType() const;
			const uint8_t GetBlendingType() const;

			void operator= (const Context& _Other);
			void operator= (Context&& _Other) noexcept;

			static const bool DepthTest(const float _NewDepth, const float _OldDepth, const uint8_t _DepthTestingType);
			static const Math::Vec3f Blend(const Math::Vec3f& _OldColor, const Math::Vec4f& _Color, const uint8_t _BlendingType, const bool _Clamp = true);

		private:

			size_t ViewPortX;
			size_t ViewPortY;
			size_t ViewPortWidth;
			size_t ViewPortHeight;

			uint8_t CullingType;
			uint8_t DepthTestingType;
			uint8_t BlendingType;

		};

	}

}



#endif
