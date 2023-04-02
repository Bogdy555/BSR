#include "..\Headers\Main.hpp"



const Math::Mat4f Rasterizer::Camera::GetViewMatrix() const
{
	return
		Math::Mat4f::GetRotation(AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetRotation(AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) *
		Math::Mat4f::GetTranslation(-Position);
}

const Math::Mat4f Rasterizer::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	if (Perspective)
	{
		return Math::Mat4f::GetPerspective(FieldOfView, _AspectRatio, NearPlane, FarPlane);
	}

	return Math::Mat4f::GetOrtho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f, -FarPlane, -NearPlane);
}



Rasterizer::VertexBuffer::VertexBuffer() : Verteces()
{

}

Rasterizer::VertexBuffer::VertexBuffer(const VertexBuffer& _Other) : Verteces(_Other.Verteces)
{

}

Rasterizer::VertexBuffer::VertexBuffer(VertexBuffer&& _Other) noexcept : Verteces(std::move(_Other.Verteces))
{

}

Rasterizer::VertexBuffer::~VertexBuffer()
{

}

void Rasterizer::VertexBuffer::PushBack(const VertexData& _Vertex)
{
	Verteces.PushBack(_Vertex);
}

void Rasterizer::VertexBuffer::Erase(const size_t _Index)
{
	Verteces.Erase(_Index);
}

void Rasterizer::VertexBuffer::Clear()
{
	Verteces.Clear();
}

const size_t Rasterizer::VertexBuffer::GetSize() const
{
	return Verteces.GetSize();
}

Rasterizer::VertexData& Rasterizer::VertexBuffer::operator[] (const size_t _Index)
{
	return Verteces[_Index];
}

const Rasterizer::VertexData& Rasterizer::VertexBuffer::operator[] (const size_t _Index) const
{
	return Verteces[_Index];
}

void Rasterizer::VertexBuffer::operator= (const VertexBuffer& _Other)
{
	Verteces = _Other.Verteces;
}

void Rasterizer::VertexBuffer::operator= (VertexBuffer&& _Other) noexcept
{
	Verteces = std::move(_Other.Verteces);
}



Rasterizer::IndexBuffer::IndexBuffer() : Indexes()
{

}

Rasterizer::IndexBuffer::IndexBuffer(const IndexBuffer& _Other) : Indexes(_Other.Indexes)
{

}

Rasterizer::IndexBuffer::IndexBuffer(IndexBuffer&& _Other) noexcept : Indexes(std::move(_Other.Indexes))
{

}

Rasterizer::IndexBuffer::~IndexBuffer()
{

}

void Rasterizer::IndexBuffer::PushBack(const IndexData& _Index)
{
	Indexes.PushBack(_Index);
}

void Rasterizer::IndexBuffer::Erase(const size_t _Index)
{
	Indexes.Erase(_Index);
}

void Rasterizer::IndexBuffer::Clear()
{
	Indexes.Clear();
}

const size_t Rasterizer::IndexBuffer::GetSize() const
{
	return Indexes.GetSize();
}

Rasterizer::IndexData& Rasterizer::IndexBuffer::operator[] (const size_t _Index)
{
	return Indexes[_Index];
}

const Rasterizer::IndexData& Rasterizer::IndexBuffer::operator[] (const size_t _Index) const
{
	return Indexes[_Index];
}

void Rasterizer::IndexBuffer::operator= (const IndexBuffer& _Other)
{
	Indexes = _Other.Indexes;
}

void Rasterizer::IndexBuffer::operator= (IndexBuffer&& _Other) noexcept
{
	Indexes = std::move(_Other.Indexes);
}



Rasterizer::Model::Model() : Meshes()
{

}

Rasterizer::Model::Model(const Model& _Other) : Meshes(_Other.Meshes)
{

}

Rasterizer::Model::Model(Model&& _Other) noexcept : Meshes(std::move(_Other.Meshes))
{

}

Rasterizer::Model::~Model()
{

}

bool Rasterizer::Model::Load(const wchar_t* _Path)
{
	Meshes.Clear();

	if (!_Path)
	{
		return false;
	}

	std::wifstream _fIn;

	_fIn.open(_Path);

	if (!_fIn.is_open())
	{
		return false;
	}

	Vector<Math::Vec3f> _Positions;
	Vector<Math::Vec3f> _Normals;
	Vector<Math::Vec2f> _TextureCoords;

	wchar_t _Line[OBJ_MAX_LINE_LEN + 1];
	wchar_t _OriginalLine[OBJ_MAX_LINE_LEN + 1];

	while (!_fIn.eof())
	{
		_fIn.getline(_OriginalLine, OBJ_MAX_LINE_LEN);

		for (size_t _Index = 0; _Index < OBJ_MAX_LINE_LEN + 1; _Index++)
		{
			_Line[_Index] = _OriginalLine[_Index];
		}

		Vector<const wchar_t*> _Tokens;

		{
			const wchar_t* _Token = String::TokenizeWhiteSpace(_Line);

			while (_Token)
			{
				_Tokens.PushBack(_Token);

				_Token = String::TokenizeWhiteSpace(nullptr);
			}
		}

		if (!_Tokens.GetSize())
		{
			continue;
		}

		if (_Tokens[0][0] == L'#')
		{
			continue;
		}

		if (String::TheSame(_Tokens[0], L"v"))
		{
			if (_Tokens.GetSize() != 4)
			{
				_fIn.close();
				return false;
			}

			_Positions.PushBack(Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3])));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vn"))
		{
			if (_Tokens.GetSize() != 4)
			{
				_fIn.close();
				return false;
			}

			_Normals.PushBack((Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vt"))
		{
			if (_Tokens.GetSize() != 3)
			{
				_fIn.close();
				return false;
			}

			_TextureCoords.PushBack((Math::Vec2f(std::stof(_Tokens[1]), std::stof(_Tokens[2]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"o"))
		{
			continue;
		}

		if (String::TheSame(_Tokens[0], L"f"))
		{
			continue;
		}
	}

	_fIn.close();

	return true;
}

void Rasterizer::Model::PushBack(const Mesh& _Mesh)
{
	Meshes.PushBack(_Mesh);
}

void Rasterizer::Model::Erase(const size_t _Index)
{
	Meshes.Erase(_Index);
}

void Rasterizer::Model::Clear()
{
	Meshes.Clear();
}

const size_t Rasterizer::Model::GetSize() const
{
	return Meshes.GetSize();
}

Rasterizer::Mesh& Rasterizer::Model::operator[] (const size_t _Index)
{
	return Meshes[_Index];
}

const Rasterizer::Mesh& Rasterizer::Model::operator[] (const size_t _Index) const
{
	return Meshes[_Index];
}

void Rasterizer::Model::operator= (const Model& _Other)
{
	Meshes = _Other.Meshes;
}

void Rasterizer::Model::operator= (Model&& _Other) noexcept
{
	Meshes = std::move(_Other.Meshes);
}
