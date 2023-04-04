#ifndef Rasterizer_hpp

#define Rasterizer_hpp



#include "Main.hpp"



#define OBJ_MAX_LINE_LEN 200



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

		virtual const Math::Vec4f SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const = 0;

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

		void AddMip(const Image::Image& _Image);
		void AddDirectMip(Image::Image&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::Image& operator[] (const size_t _Index);
		const Image::Image& operator[] (const size_t _Index) const;

		void operator= (const Texture_R& _Other) = delete;
		void operator= (Texture_R&& _Other) noexcept;

	private:

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

		void AddMip(const Image::Image& _Image);
		void AddDirectMip(Image::Image&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::Image& operator[] (const size_t _Index);
		const Image::Image& operator[] (const size_t _Index) const;

		void operator= (const Texture_RG& _Other) = delete;
		void operator= (Texture_RG&& _Other) noexcept;

	private:

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

		void AddMip(const Image::Image& _Image);
		void AddDirectMip(Image::Image&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::Image& operator[] (const size_t _Index);
		const Image::Image& operator[] (const size_t _Index) const;

		void operator= (const Texture_RGB& _Other) = delete;
		void operator= (Texture_RGB&& _Other) noexcept;

	private:

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

		void AddMip(const Image::Image& _Image);
		void AddDirectMip(Image::Image&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::Image& operator[] (const size_t _Index);
		const Image::Image& operator[] (const size_t _Index) const;

		void operator= (const Texture_RGBA& _Other) = delete;
		void operator= (Texture_RGBA&& _Other) noexcept;

	private:

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

		void AddMip(const Image::ImageFloat& _Image);
		void AddDirectMip(Image::ImageFloat&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::ImageFloat& operator[] (const size_t _Index);
		const Image::ImageFloat& operator[] (const size_t _Index) const;

		void operator= (const Texture_Float_R& _Other) = delete;
		void operator= (Texture_Float_R&& _Other) noexcept;

	private:

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

		void AddMip(const Image::ImageFloat& _Image);
		void AddDirectMip(Image::ImageFloat&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::ImageFloat& operator[] (const size_t _Index);
		const Image::ImageFloat& operator[] (const size_t _Index) const;

		void operator= (const Texture_Float_RG& _Other) = delete;
		void operator= (Texture_Float_RG&& _Other) noexcept;

	private:

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

		void AddMip(const Image::ImageFloat& _Image);
		void AddDirectMip(Image::ImageFloat&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::ImageFloat& operator[] (const size_t _Index);
		const Image::ImageFloat& operator[] (const size_t _Index) const;

		void operator= (const Texture_Float_RGB& _Other) = delete;
		void operator= (Texture_Float_RGB&& _Other) noexcept;

	private:

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

		void AddMip(const Image::ImageFloat& _Image);
		void AddDirectMip(Image::ImageFloat&& _Image);
		void RemoveMip(const size_t _Index);
		void RemoveAllMips();

		const size_t GetMipsCount() const;

		Image::ImageFloat& operator[] (const size_t _Index);
		const Image::ImageFloat& operator[] (const size_t _Index) const;

		void operator= (const Texture_Float_RGBA& _Other) = delete;
		void operator= (Texture_Float_RGBA&& _Other) noexcept;

	private:

		std::vector<Image::ImageFloat> Textures;

	};

	struct Camera
	{

		Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);

		float AngleFlat = 0.0f;
		float AngleVertical = 0.0f;
		float AngleTilt = 0.0f;

		bool Perspective = true;

		float FieldOfView = 120.0f;
		float NearPlane = 0.001f;
		float FarPlane = 1000.0f;

		const Math::Mat4f GetViewMatrix() const;
		const Math::Mat4f GetProjectionMatrix(const float _AspectRatio) const;

	};

	struct Transform
	{

		Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);

		float AngleFlat = 0.0f;
		float AngleVertical = 0.0f;
		float AngleTilt = 0.0f;

		Math::Vec3f Scale = Math::Vec3f(1.0f, 1.0f, 1.0f);

		float ShearXByY = 0.0f;
		float ShearXByZ = 0.0f;

		float ShearYByZ = 0.0f;
		float ShearYByX = 0.0f;

		float ShearZByX = 0.0f;
		float ShearZByY = 0.0f;

		const Math::Mat4f GetModelMatrix() const;

	};

	struct VertexData
	{
		Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
		Math::Vec4f Color = Math::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		Math::Vec3f Normal = Math::Vec3f(0.0f, 0.0f, 1.0f);
		Math::Vec3f Tangent = Math::Vec3f(1.0f, 0.0f, 0.0f);
		Math::Vec2f TextureCoords = Math::Vec2f(0.0f, 0.0f);
	};

	class VertexBuffer
	{

	public:

		VertexBuffer();
		VertexBuffer(const VertexBuffer& _Other);
		VertexBuffer(VertexBuffer&& _Other) noexcept;
		~VertexBuffer();

		void PushBack(const VertexData& _Vertex);
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		VertexData& operator[] (const size_t _Index);
		const VertexData& operator[] (const size_t _Index) const;

		void operator= (const VertexBuffer& _Other);
		void operator= (VertexBuffer&& _Other) noexcept;

	private:

		std::vector<VertexData> Verteces;

	};

	struct IndexData
	{
		size_t IndexA = 0;
		size_t IndexB = 0;
		size_t IndexC = 0;
	};

	class IndexBuffer
	{

	public:

		IndexBuffer();
		IndexBuffer(const IndexBuffer& _Other);
		IndexBuffer(IndexBuffer&& _Other) noexcept;
		~IndexBuffer();

		void PushBack(const IndexData& _Index);
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		IndexData& operator[] (const size_t _Index);
		const IndexData& operator[] (const size_t _Index) const;

		void operator= (const IndexBuffer& _Other);
		void operator= (IndexBuffer&& _Other) noexcept;

	private:

		std::vector<IndexData> Indexes;

	};

	struct Mesh
	{
		wchar_t* Name = nullptr;
		VertexBuffer VBO;
		IndexBuffer IBO;
	};

	class Model
	{

	public:

		Model();
		Model(const Model& _Other);
		Model(Model&& _Other) noexcept;
		~Model();

		bool Load(const wchar_t* _Path);
		const bool Save(const wchar_t* _Path) const;

		void PushBack(const Mesh& _Mesh);
		void EmplaceBack(Mesh&& _Mesh) noexcept;
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		Mesh& operator[] (const size_t _Index);
		const Mesh& operator[] (const size_t _Index) const;

		void operator= (const Model& _Other);
		void operator= (Model&& _Other) noexcept;

	private:

		std::vector<Mesh> Meshes;

	};

}



#endif
