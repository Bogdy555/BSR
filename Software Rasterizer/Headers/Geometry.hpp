#ifndef Model_hpp

#define Model_hpp



#include "Main.hpp"



namespace Geometry
{

	struct VertexData
	{
		Math::Vec4f Position;
		Math::Vec4f Color;
		Math::Vec3f Normal;
		Math::Vec3f Tangent;
		Math::Vec2f TextureCoords;
	};

	class VertexBuffer
	{

	public:

		VertexBuffer();
		VertexBuffer(const VertexBuffer& _Other);
		~VertexBuffer();

		void PushBack(const VertexData& _Vertex);
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		VertexData& operator[] (const size_t _Index);
		const VertexData& operator[] (const size_t _Index) const;

		void operator= (const VertexBuffer& _Other);

	private:

		Vector<VertexData> Verteces;

	};

	struct IndexData
	{
		size_t IndexA;
		size_t IndexB;
		size_t IndexC;
	};

	class IndexBuffer
	{

	public:

		IndexBuffer();
		IndexBuffer(const IndexBuffer& _Other);
		~IndexBuffer();

		void PushBack(const IndexData& _Index);
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		IndexData& operator[] (const size_t _Index);
		const IndexData& operator[] (const size_t _Index) const;

		void operator= (const IndexBuffer& _Other);

	private:

		Vector<IndexData> Indexes;

	};

	struct Mesh
	{
		VertexBuffer VBO;
		IndexBuffer IBO;
	};

	class Model
	{

	public:

		Model();
		Model(const Model& _Other);
		~Model();

		void PushBack(const Mesh& _Mesh);
		void Erase(const size_t _Index);
		void Clear();

		const size_t GetSize() const;

		Mesh& operator[] (const size_t _Index);
		const Mesh& operator[] (const size_t _Index) const;

		void operator= (const Model& _Other);

	private:

		Vector<Mesh> Meshes;

	};

}



#endif
