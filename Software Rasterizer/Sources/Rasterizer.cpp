#include "..\Headers\Main.hpp"



Rasterizer::FrameBuffer::FrameBuffer() : Width(0), Height(0), Color(nullptr), Depth(nullptr), Stencil(nullptr)
{

}

Rasterizer::FrameBuffer::FrameBuffer(const FrameBuffer& _Other) : Width(0), Height(0), Color(nullptr), Depth(nullptr), Stencil(nullptr)
{
	Color = new uint8_t[_Other.Width * _Other.Height * 3];

	if (!Color)
	{
		return;
	}

	Depth = new float[_Other.Width * _Other.Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return;
	}

	Stencil = new uint64_t[_Other.Width * _Other.Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return;
	}

	for (size_t _Index = 0; _Index < _Other.Width * _Other.Height; _Index++)
	{
		Color[_Index * 3 + 0] = _Other.Color[_Index * 3 + 0];
		Color[_Index * 3 + 1] = _Other.Color[_Index * 3 + 1];
		Color[_Index * 3 + 2] = _Other.Color[_Index * 3 + 2];

		Depth[_Index] = _Other.Depth[_Index];

		Stencil[_Index] = _Other.Stencil[_Index];
	}

	Width = _Other.Width;
	Height = _Other.Height;
}

Rasterizer::FrameBuffer::FrameBuffer(FrameBuffer&& _Other) noexcept : Width(_Other.Width), Height(_Other.Height), Color(_Other.Color), Depth(_Other.Depth), Stencil(_Other.Stencil)
{
	_Other.Width = 0;
	_Other.Height = 0;
	_Other.Color = nullptr;
	_Other.Depth = nullptr;
	_Other.Stencil = nullptr;
}

Rasterizer::FrameBuffer::~FrameBuffer()
{
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;
}

bool Rasterizer::FrameBuffer::Create(const size_t _Width, const size_t _Height)
{
	Width = 0;
	Height = 0;
	delete[] Color;
	Color = nullptr;
	delete[] Depth;
	Depth = nullptr;
	delete[] Stencil;
	Stencil = nullptr;

	Color = new uint8_t[_Width * _Height * 3];

	if (!Color)
	{
		return false;
	}

	Depth = new float[_Width * _Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return false;
	}

	Stencil = new uint64_t[_Width * _Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return false;
	}

	for (size_t _Index = 0; _Index < _Width * _Height; _Index++)
	{
		Color[_Index * 3 + 0] = 0;
		Color[_Index * 3 + 1] = 0;
		Color[_Index * 3 + 2] = 0;

		Depth[_Index] = 1.0f;

		Stencil[_Index] = 0;
	}

	Width = _Width;
	Height = _Height;

	return true;
}

void Rasterizer::FrameBuffer::Destroy()
{
	Width = 0;
	Height = 0;
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;
}

void Rasterizer::FrameBuffer::FillColor(const uint8_t _R, const uint8_t _G, const uint8_t _B)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Color[_Index * 3 + 0] = _R;
		Color[_Index * 3 + 1] = _G;
		Color[_Index * 3 + 2] = _B;
	}
}

void Rasterizer::FrameBuffer::FillDepth(const float _Depth)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Depth[_Index] = _Depth;
	}
}

void Rasterizer::FrameBuffer::FillStencil(const uint64_t _Value)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Stencil[_Index] = _Value;
	}
}

void Rasterizer::FrameBuffer::SetR(const uint8_t _R, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 0] = _R;
}

void Rasterizer::FrameBuffer::SetG(const uint8_t _G, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 1] = _G;
}

void Rasterizer::FrameBuffer::SetB(const uint8_t _B, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 2] = _B;
}

void Rasterizer::FrameBuffer::SetDepth(const float _Depth, const size_t _X, const size_t _Y)
{
	Depth[_X + _Y * Width] = _Depth;
}

void Rasterizer::FrameBuffer::SetStencil(const uint64_t _Value, const size_t _X, const size_t _Y)
{
	Stencil[_X + _Y * Width] = _Value;
}

const size_t Rasterizer::FrameBuffer::GetWidth() const
{
	return Width;
}

const size_t Rasterizer::FrameBuffer::GetHeight() const
{
	return Height;
}

const uint8_t Rasterizer::FrameBuffer::GetR(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 0];
}

const uint8_t Rasterizer::FrameBuffer::GetG(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 1];
}

const uint8_t Rasterizer::FrameBuffer::GetB(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 2];
}

const Math::Vec3f Rasterizer::FrameBuffer::GetColor(const size_t _X, const size_t _Y) const
{
	return Math::Vec3f((float)(GetR(_X, _Y)) / 255.0f, (float)(GetG(_X, _Y)) / 255.0f, (float)(GetB(_X, _Y)) / 255.0f);
}

const float Rasterizer::FrameBuffer::GetDepth(const size_t _X, const size_t _Y) const
{
	return Depth[_X + _Y * Width];
}

const uint64_t Rasterizer::FrameBuffer::GetStencil(const size_t _X, const size_t _Y) const
{
	return Stencil[_X + _Y * Width];
}

void Rasterizer::FrameBuffer::operator= (const FrameBuffer& _Other)
{
	Width = 0;
	Height = 0;
	delete[] Color;
	Color = nullptr;
	delete[] Depth;
	Depth = nullptr;
	delete[] Stencil;
	Stencil = nullptr;

	Color = new uint8_t[_Other.Width * _Other.Height * 3];

	if (!Color)
	{
		return;
	}

	Depth = new float[_Other.Width * _Other.Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return;
	}

	Stencil = new uint64_t[_Other.Width * _Other.Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return;
	}

	for (size_t _Index = 0; _Index < _Other.Width * _Other.Height; _Index++)
	{
		Color[_Index * 3 + 0] = _Other.Color[_Index * 3 + 0];
		Color[_Index * 3 + 1] = _Other.Color[_Index * 3 + 1];
		Color[_Index * 3 + 2] = _Other.Color[_Index * 3 + 2];

		Depth[_Index] = _Other.Depth[_Index];

		Stencil[_Index] = _Other.Stencil[_Index];
	}

	Width = _Other.Width;
	Height = _Other.Height;
}

void Rasterizer::FrameBuffer::operator= (FrameBuffer&& _Other) noexcept
{
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;

	Width = _Other.Width;
	Height = _Other.Height;
	Color = _Other.Color;
	Depth = _Other.Depth;
	Stencil = _Other.Stencil;

	_Other.Width = 0;
	_Other.Height = 0;
	_Other.Color = nullptr;
	_Other.Depth = nullptr;
	_Other.Stencil = nullptr;
}



Rasterizer::FrameBufferFloat::FrameBufferFloat() : Width(0), Height(0), Color(nullptr), Depth(nullptr), Stencil(nullptr)
{

}

Rasterizer::FrameBufferFloat::FrameBufferFloat(const FrameBufferFloat& _Other) : Width(0), Height(0), Color(nullptr), Depth(nullptr), Stencil(nullptr)
{
	Color = new float[_Other.Width * _Other.Height * 3];

	if (!Color)
	{
		return;
	}

	Depth = new float[_Other.Width * _Other.Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return;
	}

	Stencil = new uint64_t[_Other.Width * _Other.Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return;
	}

	for (size_t _Index = 0; _Index < _Other.Width * _Other.Height; _Index++)
	{
		Color[_Index * 3 + 0] = _Other.Color[_Index * 3 + 0];
		Color[_Index * 3 + 1] = _Other.Color[_Index * 3 + 1];
		Color[_Index * 3 + 2] = _Other.Color[_Index * 3 + 2];

		Depth[_Index] = _Other.Depth[_Index];

		Stencil[_Index] = _Other.Stencil[_Index];
	}

	Width = _Other.Width;
	Height = _Other.Height;
}

Rasterizer::FrameBufferFloat::FrameBufferFloat(FrameBufferFloat&& _Other) noexcept : Width(_Other.Width), Height(_Other.Height), Color(_Other.Color), Depth(_Other.Depth), Stencil(_Other.Stencil)
{
	_Other.Width = 0;
	_Other.Height = 0;
	_Other.Color = nullptr;
	_Other.Depth = nullptr;
	_Other.Stencil = nullptr;
}

Rasterizer::FrameBufferFloat::~FrameBufferFloat()
{
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;
}

bool Rasterizer::FrameBufferFloat::Create(const size_t _Width, const size_t _Height)
{
	Width = 0;
	Height = 0;
	delete[] Color;
	Color = nullptr;
	delete[] Depth;
	Depth = nullptr;
	delete[] Stencil;
	Stencil = nullptr;

	Color = new float[_Width * _Height * 3];

	if (!Color)
	{
		return false;
	}

	Depth = new float[_Width * _Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return false;
	}

	Stencil = new uint64_t[_Width * _Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return false;
	}

	for (size_t _Index = 0; _Index < _Width * _Height; _Index++)
	{
		Color[_Index * 3 + 0] = 0.0f;
		Color[_Index * 3 + 1] = 0.0f;
		Color[_Index * 3 + 2] = 0.0f;

		Depth[_Index] = 1.0f;

		Stencil[_Index] = 0;
	}

	Width = _Width;
	Height = _Height;

	return true;
}

void Rasterizer::FrameBufferFloat::Destroy()
{
	Width = 0;
	Height = 0;
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;
}

void Rasterizer::FrameBufferFloat::FillColor(const float _R, const float _G, const float _B)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Color[_Index * 3 + 0] = _R;
		Color[_Index * 3 + 1] = _G;
		Color[_Index * 3 + 2] = _B;
	}
}

void Rasterizer::FrameBufferFloat::FillDepth(const float _Depth)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Depth[_Index] = _Depth;
	}
}

void Rasterizer::FrameBufferFloat::FillStencil(const uint64_t _Value)
{
	for (size_t _Index = 0; _Index < Width * Height; _Index++)
	{
		Stencil[_Index] = _Value;
	}
}

void Rasterizer::FrameBufferFloat::SetR(const float _R, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 0] = _R;
}

void Rasterizer::FrameBufferFloat::SetG(const float _G, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 1] = _G;
}

void Rasterizer::FrameBufferFloat::SetB(const float _B, const size_t _X, const size_t _Y)
{
	Color[(_X + _Y * Width) * 3 + 2] = _B;
}

void Rasterizer::FrameBufferFloat::SetDepth(const float _Depth, const size_t _X, const size_t _Y)
{
	Depth[_X + _Y * Width] = _Depth;
}

void Rasterizer::FrameBufferFloat::SetStencil(const uint64_t _Value, const size_t _X, const size_t _Y)
{
	Stencil[_X + _Y * Width] = _Value;
}

const size_t Rasterizer::FrameBufferFloat::GetWidth() const
{
	return Width;
}

const size_t Rasterizer::FrameBufferFloat::GetHeight() const
{
	return Height;
}

const float Rasterizer::FrameBufferFloat::GetR(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 0];
}

const float Rasterizer::FrameBufferFloat::GetG(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 1];
}

const float Rasterizer::FrameBufferFloat::GetB(const size_t _X, const size_t _Y) const
{
	return Color[(_X + _Y * Width) * 3 + 2];
}

const Math::Vec3f Rasterizer::FrameBufferFloat::GetColor(const size_t _X, const size_t _Y) const
{
	return Math::Vec3f(GetR(_X, _Y), GetG(_X, _Y), GetB(_X, _Y));
}

const float Rasterizer::FrameBufferFloat::GetDepth(const size_t _X, const size_t _Y) const
{
	return Depth[_X + _Y * Width];
}

const uint64_t Rasterizer::FrameBufferFloat::GetStencil(const size_t _X, const size_t _Y) const
{
	return Stencil[_X + _Y * Width];
}

void Rasterizer::FrameBufferFloat::operator= (const FrameBufferFloat& _Other)
{
	Width = 0;
	Height = 0;
	delete[] Color;
	Color = nullptr;
	delete[] Depth;
	Depth = nullptr;
	delete[] Stencil;
	Stencil = nullptr;

	Color = new float[_Other.Width * _Other.Height * 3];

	if (!Color)
	{
		return;
	}

	Depth = new float[_Other.Width * _Other.Height];

	if (!Depth)
	{
		delete[] Color;
		Color = nullptr;
		return;
	}

	Stencil = new uint64_t[_Other.Width * _Other.Height];

	if (!Stencil)
	{
		delete[] Color;
		Color = nullptr;
		delete[] Depth;
		Depth = nullptr;
		return;
	}

	for (size_t _Index = 0; _Index < _Other.Width * _Other.Height; _Index++)
	{
		Color[_Index * 3 + 0] = _Other.Color[_Index * 3 + 0];
		Color[_Index * 3 + 1] = _Other.Color[_Index * 3 + 1];
		Color[_Index * 3 + 2] = _Other.Color[_Index * 3 + 2];

		Depth[_Index] = _Other.Depth[_Index];

		Stencil[_Index] = _Other.Stencil[_Index];
	}

	Width = _Other.Width;
	Height = _Other.Height;
}

void Rasterizer::FrameBufferFloat::operator= (FrameBufferFloat&& _Other) noexcept
{
	delete[] Color;
	delete[] Depth;
	delete[] Stencil;

	Width = _Other.Width;
	Height = _Other.Height;
	Color = _Other.Color;
	Depth = _Other.Depth;
	Stencil = _Other.Stencil;

	_Other.Width = 0;
	_Other.Height = 0;
	_Other.Color = nullptr;
	_Other.Depth = nullptr;
	_Other.Stencil = nullptr;
}



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



const Math::Mat4f Rasterizer::Transform::GetModelMatrix() const
{
	return
		Math::Mat4f::GetTranslation(Position) *
		Math::Mat4f::GetRotation(AngleFlat, Math::Vec3f(0.0f, -1.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleVertical, Math::Vec3f(-1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetScale(Scale.x, Scale.y, Scale.z, 1.0f) *
		Math::Mat4f::GetShear(ShearXByY, 0, 1) *
		Math::Mat4f::GetShear(ShearXByZ, 0, 2) *
		Math::Mat4f::GetShear(ShearYByZ, 1, 2) *
		Math::Mat4f::GetShear(ShearYByX, 1, 0) *
		Math::Mat4f::GetShear(ShearZByX, 2, 0) *
		Math::Mat4f::GetShear(ShearZByY, 2, 1);
}
