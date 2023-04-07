#include "..\Headers\Main.hpp"



static bool ComputeWrap(const Math::Vec2f& _TextureCoords, const float _MipLevel, const uint8_t _WrapType, const size_t _TexturesSize, Math::Vec2f& _NewTextureCoords, float& _NewMipLevel)
{
	switch (_WrapType)
	{
	case Rasterizer::_WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return false;
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case Rasterizer::_WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case Rasterizer::_WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case Rasterizer::_WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	default:
	{
		return false;
	}
	}

	return true;
}



Rasterizer::Texture::Texture() : LerpType(_LerpLinear), WrapType(_WrapRepeat)
{

}

Rasterizer::Texture::Texture(Texture&& _Other) noexcept : LerpType(_Other.LerpType), WrapType(_Other.WrapType)
{
	_Other.LerpType = _LerpLinear;
	_Other.WrapType = _WrapRepeat;
}

Rasterizer::Texture::~Texture()
{

}

void Rasterizer::Texture::SetLerpType(const uint8_t _LerpType)
{
	LerpType = _LerpType;
}

void Rasterizer::Texture::SetWrapType(const uint8_t _WrapType)
{
	WrapType = _WrapType;
}

const uint8_t Rasterizer::Texture::GetLerpType() const
{
	return LerpType;
}

const uint8_t Rasterizer::Texture::GetWrapType() const
{
	return WrapType;
}

void Rasterizer::Texture::operator= (Texture&& _Other) noexcept
{
	LerpType = _Other.LerpType;
	WrapType = _Other.WrapType;

	_Other.LerpType = _LerpLinear;
	_Other.WrapType = _WrapRepeat;
}



Rasterizer::Texture_R::Texture_R() : Texture(), Textures()
{

}

Rasterizer::Texture_R::Texture_R(Texture_R&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_R::~Texture_R()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const float Rasterizer::Texture_R::SampleR(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return 0.0f;
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return 0.0f;
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return 0.0f;
	}

	return 0.0f;
}

const Math::Vec4f Rasterizer::Texture_R::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _R = SampleR(_TextureCoords, _MipLevel);

	return Math::Vec4f(_R, _R, _R, 1.0f);
}

void Rasterizer::Texture_R::AddMip(const Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 1];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 1 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
		}
	}

	Textures.push_back(Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_R::AddDirectMip(Image::Image&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_R::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_R::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_R::GetMipsCount() const
{
	return Textures.size();
}

Image::Image& Rasterizer::Texture_R::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::Image& Rasterizer::Texture_R::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_R::operator= (Texture_R&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const float Rasterizer::Texture_R::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0]) / 255.0f;

	return _R;
}



Rasterizer::Texture_RG::Texture_RG() : Texture(), Textures()
{

}

Rasterizer::Texture_RG::Texture_RG(Texture_RG&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_RG::~Texture_RG()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec2f Rasterizer::Texture_RG::SampleRG(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	return Math::Vec2f(0.0f, 0.0f);
}

const Math::Vec4f Rasterizer::Texture_RG::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const Math::Vec2f _RG = SampleRG(_TextureCoords, _MipLevel);

	return Math::Vec4f(_RG, 0.0f, 1.0f);
}

void Rasterizer::Texture_RG::AddMip(const Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 2];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
		}
	}

	Textures.push_back(Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_RG::AddDirectMip(Image::Image&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_RG::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_RG::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_RG::GetMipsCount() const
{
	return Textures.size();
}

Image::Image& Rasterizer::Texture_RG::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::Image& Rasterizer::Texture_RG::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_RG::operator= (Texture_RG&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec2f Rasterizer::Texture_RG::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1]) / 255.0f;

	return Math::Vec2f(_R, _G);
}



Rasterizer::Texture_RGB::Texture_RGB() : Texture(), Textures()
{

}

Rasterizer::Texture_RGB::Texture_RGB(Texture_RGB&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_RGB::~Texture_RGB()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec3f Rasterizer::Texture_RGB::SampleRGB(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

const Math::Vec4f Rasterizer::Texture_RGB::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const Math::Vec3f _RGB = SampleRGB(_TextureCoords, _MipLevel);

	return Math::Vec4f(_RGB, 1.0f);
}

void Rasterizer::Texture_RGB::AddMip(const Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 3];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 2] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 2];
		}
	}

	Textures.push_back(Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_RGB::AddDirectMip(Image::Image&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_RGB::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_RGB::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_RGB::GetMipsCount() const
{
	return Textures.size();
}

Image::Image& Rasterizer::Texture_RGB::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::Image& Rasterizer::Texture_RGB::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_RGB::operator= (Texture_RGB&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec3f Rasterizer::Texture_RGB::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1]) / 255.0f;
	float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2]) / 255.0f;

	return Math::Vec3f(_R, _G, _B);
}



Rasterizer::Texture_RGBA::Texture_RGBA() : Texture(), Textures()
{

}

Rasterizer::Texture_RGBA::Texture_RGBA(Texture_RGBA&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_RGBA::~Texture_RGBA()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec4f Rasterizer::Texture_RGBA::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

void Rasterizer::Texture_RGBA::AddMip(const Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 4];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 2] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 2];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 3] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 3];
		}
	}

	Textures.push_back(Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_RGBA::AddDirectMip(Image::Image&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_RGBA::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_RGBA::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_RGBA::GetMipsCount() const
{
	return Textures.size();
}

Image::Image& Rasterizer::Texture_RGBA::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::Image& Rasterizer::Texture_RGBA::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_RGBA::operator= (Texture_RGBA&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec4f Rasterizer::Texture_RGBA::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1]) / 255.0f;
	float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2]) / 255.0f;
	float _A = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3]) / 255.0f;

	return Math::Vec4f(_R, _G, _B, _A);
}



Rasterizer::Texture_Float_R::Texture_Float_R() : Texture(), Textures()
{

}

Rasterizer::Texture_Float_R::Texture_Float_R(Texture_Float_R&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_Float_R::~Texture_Float_R()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const float Rasterizer::Texture_Float_R::SampleR(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return 0.0f;
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return 0.0f;
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return 0.0f;
	}

	return 0.0f;
}

const Math::Vec4f Rasterizer::Texture_Float_R::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _R = SampleR(_TextureCoords, _MipLevel);

	return Math::Vec4f(_R, _R, _R, 1.0f);
}

void Rasterizer::Texture_Float_R::AddMip(const Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 1];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 1 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
		}
	}

	Textures.push_back(Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_Float_R::AddDirectMip(Image::ImageFloat&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_Float_R::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_Float_R::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_Float_R::GetMipsCount() const
{
	return Textures.size();
}

Image::ImageFloat& Rasterizer::Texture_Float_R::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::ImageFloat& Rasterizer::Texture_Float_R::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_Float_R::operator= (Texture_Float_R&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const float Rasterizer::Texture_Float_R::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0];

	return _R;
}



Rasterizer::Texture_Float_RG::Texture_Float_RG() : Texture(), Textures()
{

}

Rasterizer::Texture_Float_RG::Texture_Float_RG(Texture_Float_RG&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_Float_RG::~Texture_Float_RG()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec2f Rasterizer::Texture_Float_RG::SampleRG(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec2f(0.0f, 0.0f);
	}

	return Math::Vec2f(0.0f, 0.0f);
}

const Math::Vec4f Rasterizer::Texture_Float_RG::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const Math::Vec2f _RG = SampleRG(_TextureCoords, _MipLevel);

	return Math::Vec4f(_RG, 0.0f, 1.0f);
}

void Rasterizer::Texture_Float_RG::AddMip(const Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 2];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
		}
	}

	Textures.push_back(Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_Float_RG::AddDirectMip(Image::ImageFloat&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_Float_RG::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_Float_RG::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_Float_RG::GetMipsCount() const
{
	return Textures.size();
}

Image::ImageFloat& Rasterizer::Texture_Float_RG::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::ImageFloat& Rasterizer::Texture_Float_RG::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_Float_RG::operator= (Texture_Float_RG&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec2f Rasterizer::Texture_Float_RG::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1];

	return Math::Vec2f(_R, _G);
}



Rasterizer::Texture_Float_RGB::Texture_Float_RGB() : Texture(), Textures()
{

}

Rasterizer::Texture_Float_RGB::Texture_Float_RGB(Texture_Float_RGB&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_Float_RGB::~Texture_Float_RGB()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec3f Rasterizer::Texture_Float_RGB::SampleRGB(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

const Math::Vec4f Rasterizer::Texture_Float_RGB::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const Math::Vec3f _RGB = SampleRGB(_TextureCoords, _MipLevel);

	return Math::Vec4f(_RGB, 1.0f);
}

void Rasterizer::Texture_Float_RGB::AddMip(const Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 3];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
			_NewImage.Data[(_X + _Y * _Image.Width) * 3 + 2] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 2];
		}
	}

	Textures.push_back(Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_Float_RGB::AddDirectMip(Image::ImageFloat&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_Float_RGB::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_Float_RGB::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_Float_RGB::GetMipsCount() const
{
	return Textures.size();
}

Image::ImageFloat& Rasterizer::Texture_Float_RGB::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::ImageFloat& Rasterizer::Texture_Float_RGB::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_Float_RGB::operator= (Texture_Float_RGB&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec3f Rasterizer::Texture_Float_RGB::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1];
	float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2];

	return Math::Vec3f(_R, _G, _B);
}



Rasterizer::Texture_Float_RGBA::Texture_Float_RGBA() : Texture(), Textures()
{

}

Rasterizer::Texture_Float_RGBA::Texture_Float_RGBA(Texture_Float_RGBA&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

Rasterizer::Texture_Float_RGBA::~Texture_Float_RGBA()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const Math::Vec4f Rasterizer::Texture_Float_RGBA::SampleRGBA(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

void Rasterizer::Texture_Float_RGBA::AddMip(const Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		DEBUG_BREAK_MSG(STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return;
	}

	Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 4];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		DEBUG_BREAK();
		return;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 2] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 2];
			_NewImage.Data[(_X + _Y * _Image.Width) * 4 + 3] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 3];
		}
	}

	Textures.push_back(Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;
}

void Rasterizer::Texture_Float_RGBA::AddDirectMip(Image::ImageFloat&& _Image)
{
	ASSERT_MSG(_Image.Data, STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void Rasterizer::Texture_Float_RGBA::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void Rasterizer::Texture_Float_RGBA::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t Rasterizer::Texture_Float_RGBA::GetMipsCount() const
{
	return Textures.size();
}

Image::ImageFloat& Rasterizer::Texture_Float_RGBA::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const Image::ImageFloat& Rasterizer::Texture_Float_RGBA::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void Rasterizer::Texture_Float_RGBA::operator= (Texture_Float_RGBA&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const Math::Vec4f Rasterizer::Texture_Float_RGBA::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1];
	float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2];
	float _A = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3];

	return Math::Vec4f(_R, _G, _B, _A);
}
