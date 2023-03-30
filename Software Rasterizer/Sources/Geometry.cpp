#include "..\Headers\Main.hpp"



Geometry::VertexBuffer::VertexBuffer() : Verteces()
{

}

Geometry::VertexBuffer::VertexBuffer(const VertexBuffer& _Other) : Verteces(_Other.Verteces)
{

}

Geometry::VertexBuffer::~VertexBuffer()
{

}

void Geometry::VertexBuffer::PushBack(const VertexData& _Vertex)
{
	Verteces.PushBack(_Vertex);
}

void Geometry::VertexBuffer::Erase(const size_t _Index)
{
	Verteces.Erase(_Index);
}

void Geometry::VertexBuffer::Clear()
{
	Verteces.Clear();
}

const size_t Geometry::VertexBuffer::GetSize() const
{
	return Verteces.GetSize();
}

Geometry::VertexData& Geometry::VertexBuffer::operator[] (const size_t _Index)
{
	return Verteces[_Index];
}

const Geometry::VertexData& Geometry::VertexBuffer::operator[] (const size_t _Index) const
{
	return Verteces[_Index];
}

void Geometry::VertexBuffer::operator= (const VertexBuffer& _Other)
{
	Verteces = _Other.Verteces;
}



Geometry::IndexBuffer::IndexBuffer() : Indexes()
{

}

Geometry::IndexBuffer::IndexBuffer(const IndexBuffer& _Other) : Indexes(_Other.Indexes)
{

}

Geometry::IndexBuffer::~IndexBuffer()
{

}

void Geometry::IndexBuffer::PushBack(const IndexData& _Index)
{
	Indexes.PushBack(_Index);
}

void Geometry::IndexBuffer::Erase(const size_t _Index)
{
	Indexes.Erase(_Index);
}

void Geometry::IndexBuffer::Clear()
{
	Indexes.Clear();
}

const size_t Geometry::IndexBuffer::GetSize() const
{
	return Indexes.GetSize();
}

Geometry::IndexData& Geometry::IndexBuffer::operator[] (const size_t _Index)
{
	return Indexes[_Index];
}

const Geometry::IndexData& Geometry::IndexBuffer::operator[] (const size_t _Index) const
{
	return Indexes[_Index];
}

void Geometry::IndexBuffer::operator= (const IndexBuffer& _Other)
{
	Indexes = _Other.Indexes;
}



Geometry::Model::Model() : Meshes()
{

}

Geometry::Model::Model(const Model& _Other) : Meshes(_Other.Meshes)
{

}

Geometry::Model::~Model()
{

}

void Geometry::Model::PushBack(const Mesh& _Mesh)
{
	Meshes.PushBack(_Mesh);
}

void Geometry::Model::Erase(const size_t _Index)
{
	Meshes.Erase(_Index);
}

void Geometry::Model::Clear()
{
	Meshes.Clear();
}

const size_t Geometry::Model::GetSize() const
{
	return Meshes.GetSize();
}

Geometry::Mesh& Geometry::Model::operator[] (const size_t _Index)
{
	return Meshes[_Index];
}

const Geometry::Mesh& Geometry::Model::operator[] (const size_t _Index) const
{
	return Meshes[_Index];
}

void Geometry::Model::operator= (const Model& _Other)
{
	Meshes = _Other.Meshes;
}
