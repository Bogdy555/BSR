#ifndef Rasterizer_hpp

#define Rasterizer_hpp



#include "Main.hpp"



#define OBJ_MAX_LINE_LEN 200



namespace Rasterizer
{

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

		Vector<VertexData> Verteces;

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

		Vector<IndexData> Indexes;

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

		Vector<Mesh> Meshes;

	};

}



#endif
