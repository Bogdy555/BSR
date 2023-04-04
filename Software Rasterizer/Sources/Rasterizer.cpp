#include "..\Headers\Main.hpp"



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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return 0.0f;
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return 0.0f;
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0]) / 255.0f;

		return _R;
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return 0.0f;
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec2f(0.0f, 0.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec2f(0.0f, 0.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0]) / 255.0f;
		float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1]) / 255.0f;

		return Math::Vec2f(_R, _G);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec2f(0.0f, 0.0f);
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec3f(0.0f, 0.0f, 0.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0]) / 255.0f;
		float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1]) / 255.0f;
		float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2]) / 255.0f;

		return Math::Vec3f(_R, _G, _B);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::Image& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0]) / 255.0f;
		float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1]) / 255.0f;
		float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2]) / 255.0f;
		float _A = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3]) / 255.0f;

		return Math::Vec4f(_R, _G, _B, _A);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return 0.0f;
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return 0.0f;
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0];

		return _R;
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return 0.0f;
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec2f(0.0f, 0.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec2f(0.0f, 0.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0];
		float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1];

		return Math::Vec2f(_R, _G);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec2f(0.0f, 0.0f);
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec3f(0.0f, 0.0f, 0.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0];
		float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1];
		float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2];

		return Math::Vec3f(_R, _G, _B);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec3f(0.0f, 0.0f, 0.0f);
	}
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

	Math::Vec2f _NewTextureCoords = _TextureCoords;
	float _NewMipLevel = _MipLevel;

	switch (WrapType)
	{
	case _WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		}
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapClamp:
	{
		_NewTextureCoords.x = Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapRepeat:
	{
		_NewTextureCoords.x = Math::Period(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = Math::Period(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	case _WrapMirror:
	{
		Math::Vec2f _Period(Math::Period(_TextureCoords.x, 0.0f, 1.0f), Math::Period(_TextureCoords.y, 0.0f, 1.0f));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(Textures.size() - 1));

		break;
	}
	default:
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
	}

	switch (LerpType)
	{
	case _LerpNearest:
	{
		_NewMipLevel += 0.5f;

		const Image::ImageFloat& _CurrentTexture = Textures[(size_t)(_NewMipLevel)];

		size_t _ImageX = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Width - 1) + 0.5f, _NewTextureCoords.x) + 0.5f, 0.0f, (float)(_CurrentTexture.Width - 1)));
		size_t _ImageY = (size_t)(Math::Clamp(Math::Mix(-0.5f, (float)(_CurrentTexture.Height - 1) + 0.5f, _NewTextureCoords.y) + 0.5f, 0.0f, (float)(_CurrentTexture.Height - 1)));

		float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0];
		float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1];
		float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2];
		float _A = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3];

		return Math::Vec4f(_R, _G, _B, _A);
	}
	case _LerpLinear:
	{
		break;
	}
	default:
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
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
	Verteces.push_back(_Vertex);
}

void Rasterizer::VertexBuffer::Erase(const size_t _Index)
{
	Verteces.erase(Verteces.begin() + _Index);
}

void Rasterizer::VertexBuffer::Clear()
{
	Verteces.clear();
}

const size_t Rasterizer::VertexBuffer::GetSize() const
{
	return Verteces.size();
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
	Indexes.push_back(_Index);
}

void Rasterizer::IndexBuffer::Erase(const size_t _Index)
{
	Indexes.erase(Indexes.begin() + _Index);
}

void Rasterizer::IndexBuffer::Clear()
{
	Indexes.clear();
}

const size_t Rasterizer::IndexBuffer::GetSize() const
{
	return Indexes.size();
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
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		delete[] Meshes[_Index].Name;
	}
}

bool Rasterizer::Model::Load(const wchar_t* _Path)
{
	struct MeshFileData
	{
		wchar_t* Name = nullptr;
		size_t FacesStart = (size_t)(-1);
		size_t FacesEnd = (size_t)(-1);
	};

	struct FaceVertex
	{
		size_t Position = (size_t)(-1);
		size_t Normal = (size_t)(-1);
		size_t TextureCoords = (size_t)(-1);
	};

	Meshes.clear();

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

	std::vector<MeshFileData> _Meshes;

	std::vector<Math::Vec3f> _Positions;
	std::vector<Math::Vec3f> _Normals;
	std::vector<Math::Vec2f> _TextureCoords;

	std::vector<std::vector<FaceVertex>> _Faces;

	wchar_t _Line[OBJ_MAX_LINE_LEN + 1];
	wchar_t _OriginalLine[OBJ_MAX_LINE_LEN + 1];

	while (!_fIn.eof())
	{
		_fIn.getline(_OriginalLine, OBJ_MAX_LINE_LEN);

		for (size_t _Index = 0; _Index < OBJ_MAX_LINE_LEN + 1; _Index++)
		{
			_Line[_Index] = _OriginalLine[_Index];
		}

		std::vector<const wchar_t*> _Tokens;

		{
			const wchar_t* _Token = String::TokenizeWhiteSpace(_Line);

			while (_Token)
			{
				_Tokens.push_back(_Token);

				_Token = String::TokenizeWhiteSpace(nullptr);
			}
		}

		if (!_Tokens.size())
		{
			continue;
		}

		if (_Tokens[0][0] == L'#')
		{
			continue;
		}

		if (String::TheSame(_Tokens[0], L"o"))
		{
			if (_Tokens.size() != 2)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _Meshes.size(); _Index++)
			{
				if (String::TheSame(_Tokens[1], _Meshes[_Index].Name))
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
			}

			MeshFileData _MeshFileData;

			size_t _NameLen = String::Length(_Tokens[1]);

			_MeshFileData.Name = new wchar_t[_NameLen + 1];

			if (!_MeshFileData.Name)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _NameLen + 1; _Index++)
			{
				_MeshFileData.Name[_Index] = _Tokens[1][_Index];
			}

			_MeshFileData.FacesStart = _Faces.size();

			if (_Meshes.size())
			{
				_Meshes[_Meshes.size() - 1].FacesEnd = _Faces.size();
			}

			_Meshes.push_back(_MeshFileData);

			continue;
		}

		if (String::TheSame(_Tokens[0], L"v"))
		{
			if (_Tokens.size() != 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Positions.push_back(Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3])));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vn"))
		{
			if (_Tokens.size() != 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Normals.push_back((Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vt"))
		{
			if (_Tokens.size() != 3)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_TextureCoords.push_back((Math::Vec2f(std::stof(_Tokens[1]), std::stof(_Tokens[2]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"f"))
		{
			if (_Tokens.size() < 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Faces.push_back(std::vector<FaceVertex>());

			std::vector<FaceVertex>& _CurrentFace = _Faces[_Faces.size() - 1];

			for (size_t _Index = 1; _Index < _Tokens.size(); _Index++)
			{
				size_t _TokenLen = String::Length(_Tokens[_Index]);

				if (_Tokens[_Index][0] == L'/' || _Tokens[_Index][_TokenLen - 1] == L'/')
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}

				FaceVertex _FaceVertex;

				size_t _CountSlash = String::Count(_Tokens[_Index], L'/');

				switch (_CountSlash)
				{
				case 0:
				{
					X64_CALL(_FaceVertex.Position = std::stoull(_Tokens[_Index]) - 1);
					X86_CALL(_FaceVertex.Position = std::stoul(_Tokens[_Index]) - 1);

					if (_FaceVertex.Position >= _Positions.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 1:
				{
					wchar_t _FaceVertexText[OBJ_MAX_LINE_LEN + 1];

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText[_IndexCopy] = _Tokens[_Index][_IndexCopy];
					}

					std::vector<const wchar_t*> _FaceVertexTokens;

					{
						const wchar_t* _Token = String::TokenizeSlashes(_FaceVertexText);

						while (_Token)
						{
							_FaceVertexTokens.push_back(_Token);

							_Token = String::TokenizeSlashes(nullptr);
						}
					}

					X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
					X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

					if (_FaceVertex.Position >= _Positions.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					X64_CALL(_FaceVertex.TextureCoords = std::stoull(_FaceVertexTokens[1]) - 1);
					X86_CALL(_FaceVertex.TextureCoords = std::stoul(_FaceVertexTokens[1]) - 1);

					if (_FaceVertex.TextureCoords >= _TextureCoords.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 2:
				{
					wchar_t _FaceVertexText[OBJ_MAX_LINE_LEN + 1];

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText[_IndexCopy] = _Tokens[_Index][_IndexCopy];
					}

					std::vector<const wchar_t*> _FaceVertexTokens;

					{
						const wchar_t* _Token = String::TokenizeSlashes(_FaceVertexText);

						while (_Token)
						{
							_FaceVertexTokens.push_back(_Token);

							_Token = String::TokenizeSlashes(nullptr);
						}
					}

					if (_FaceVertexTokens.size() == 2)
					{
						X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
						X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

						if (_FaceVertex.Position >= _Positions.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						X64_CALL(_FaceVertex.Normal = std::stoull(_FaceVertexTokens[1]) - 1);
						X86_CALL(_FaceVertex.Normal = std::stoul(_FaceVertexTokens[1]) - 1);

						if (_FaceVertex.Normal >= _Normals.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}
					else
					{
						X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
						X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

						if (_FaceVertex.Position >= _Positions.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						X64_CALL(_FaceVertex.TextureCoords = std::stoull(_FaceVertexTokens[1]) - 1);
						X86_CALL(_FaceVertex.TextureCoords = std::stoul(_FaceVertexTokens[1]) - 1);

						if (_FaceVertex.TextureCoords >= _TextureCoords.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						X64_CALL(_FaceVertex.Normal = std::stoull(_FaceVertexTokens[2]) - 1);
						X86_CALL(_FaceVertex.Normal = std::stoul(_FaceVertexTokens[2]) - 1);

						if (_FaceVertex.Normal >= _Normals.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}

					break;
				}
				default:
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
				}

				_CurrentFace.push_back(_FaceVertex);
			}

			continue;
		}
	}

	if (!_Meshes.size())
	{
		_fIn.close();
		return false;
	}

	_Meshes[_Meshes.size() - 1].FacesEnd = _Faces.size();

	for (size_t _IndexMesh = 0; _IndexMesh < _Meshes.size(); _IndexMesh++)
	{
		MeshFileData& _CurrentMesh = _Meshes[_IndexMesh];

		Mesh _Mesh;

		_Mesh.Name = _CurrentMesh.Name;

		for (size_t _IndexFace = _CurrentMesh.FacesStart; _IndexFace < _CurrentMesh.FacesEnd; _IndexFace++)
		{
			std::vector<FaceVertex>& _CurrentFace = _Faces[_IndexFace];

			for (size_t _IndexFaceVertex = 1; _IndexFaceVertex < _CurrentFace.size() - 1; _IndexFaceVertex++)
			{
				VertexData _VertA;
				VertexData _VertB;
				VertexData _VertC;

				_VertA.Position = _Positions[_CurrentFace[0].Position];
				_VertA.Color = Math::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

				_VertB.Position = _Positions[_CurrentFace[_IndexFaceVertex].Position];
				_VertB.Color = Math::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

				_VertC.Position = _Positions[_CurrentFace[_IndexFaceVertex + 1].Position];
				_VertC.Color = Math::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

				if (_CurrentFace[0].Normal != (size_t)(-1))
				{
					_VertA.Normal = _Normals[_CurrentFace[0].Normal];
				}
				else
				{
					_VertA.Normal = Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[0].TextureCoords != (size_t)(-1))
				{
					_VertA.TextureCoords = _TextureCoords[_CurrentFace[0].TextureCoords];
				}
				else
				{
					_VertA.TextureCoords = Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex].Normal != (size_t)(-1))
				{
					_VertB.Normal = _Normals[_CurrentFace[_IndexFaceVertex].Normal];
				}
				else
				{
					_VertB.Normal = Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1))
				{
					_VertB.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex].TextureCoords];
				}
				else
				{
					_VertB.TextureCoords = Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex + 1].Normal != (size_t)(-1))
				{
					_VertC.Normal = _Normals[_CurrentFace[_IndexFaceVertex + 1].Normal];
				}
				else
				{
					_VertC.Normal = Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					_VertC.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex + 1].TextureCoords];
				}
				else
				{
					_VertC.TextureCoords = Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[0].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					if (_VertA.TextureCoords != _VertB.TextureCoords && _VertB.TextureCoords != _VertC.TextureCoords && _VertC.TextureCoords != _VertA.TextureCoords)
					{
						Math::Vec3f _Edge1 = _VertB.Position - _VertA.Position;
						Math::Vec3f _Edge2 = _VertC.Position - _VertA.Position;

						Math::Vec2f _Delta1 = _VertB.TextureCoords - _VertA.TextureCoords;
						Math::Vec2f _Delta2 = _VertC.TextureCoords - _VertA.TextureCoords;

						float _Factor = 1.0f / (_Delta1.x * _Delta2.y - _Delta2.x * _Delta1.y);

						_VertA.Tangent.x = _Factor * (_Delta2.y * _Edge1.x - _Delta1.y * _Edge2.x);
						_VertA.Tangent.y = _Factor * (_Delta2.y * _Edge1.y - _Delta1.y * _Edge2.y);
						_VertA.Tangent.z = _Factor * (_Delta2.y * _Edge1.z - _Delta1.y * _Edge2.z);

						_VertA.Tangent /= _VertA.Tangent.Magnitude();

						_VertB.Tangent.x = _VertA.Tangent.x;
						_VertB.Tangent.y = _VertA.Tangent.y;
						_VertB.Tangent.z = _VertA.Tangent.z;

						_VertC.Tangent.x = _VertA.Tangent.x;
						_VertC.Tangent.y = _VertA.Tangent.y;
						_VertC.Tangent.z = _VertA.Tangent.z;
					}
					else
					{
						_VertA.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertB.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertC.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					_VertA.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertB.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertC.Tangent = Math::Vec3f(0.0f, 0.0f, 0.0f);
				}

				IndexData _IndexData;

				_IndexData.IndexA = _Mesh.VBO.GetSize();
				_IndexData.IndexB = _Mesh.VBO.GetSize() + 1;
				_IndexData.IndexC = _Mesh.VBO.GetSize() + 2;

				_Mesh.VBO.PushBack(_VertA);
				_Mesh.VBO.PushBack(_VertB);
				_Mesh.VBO.PushBack(_VertC);

				_Mesh.IBO.PushBack(_IndexData);
			}
		}

		Meshes.emplace_back(std::move(_Mesh));
	}

	_fIn.close();

	return true;
}

const bool Rasterizer::Model::Save(const wchar_t* _Path) const
{
	std::wofstream _fOut;

	_fOut.open(_Path);

	if (!_fOut.is_open())
	{
		return false;
	}

	size_t _CurrentSize = 1;

	for (size_t _IndexMesh = 0; _IndexMesh < Meshes.size(); _IndexMesh++)
	{
		const Mesh& _CurrentMesh = Meshes[_IndexMesh];

		if (!_CurrentMesh.Name)
		{
			_fOut.close();
			_wremove(_Path);
			return false;
		}

		_fOut << L'o' << L' ' << _CurrentMesh.Name << L'\n';

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L' ' << _CurrentVertex.Position.x << L' ' << _CurrentVertex.Position.y << L' ' << _CurrentVertex.Position.z << L'\n';
		}

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L't' << L' ' << _CurrentVertex.TextureCoords.x << L' ' << _CurrentVertex.TextureCoords.y << L'\n';
		}

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L'n' << L' ' << _CurrentVertex.Normal.x << L' ' << _CurrentVertex.Normal.y << L' ' << _CurrentVertex.Normal.z << L'\n';
		}

		for (size_t _IndexTriangle = 0; _IndexTriangle < _CurrentMesh.IBO.GetSize(); _IndexTriangle++)
		{
			const IndexData& _CurrentTriangle = _CurrentMesh.IBO[_IndexTriangle];

			_fOut << L'f' << L' ';
			_fOut << _CurrentTriangle.IndexA + _CurrentSize << L'/' << _CurrentTriangle.IndexA + _CurrentSize << L'/' << _CurrentTriangle.IndexA + _CurrentSize << L' ';
			_fOut << _CurrentTriangle.IndexB + _CurrentSize << L'/' << _CurrentTriangle.IndexB + _CurrentSize << L'/' << _CurrentTriangle.IndexB + _CurrentSize << L' ';
			_fOut << _CurrentTriangle.IndexC + _CurrentSize << L'/' << _CurrentTriangle.IndexC + _CurrentSize << L'/' << _CurrentTriangle.IndexC + _CurrentSize << L'\n';
		}

		_CurrentSize += _CurrentMesh.VBO.GetSize();
	}

	_fOut.close();

	return true;
}

void Rasterizer::Model::PushBack(const Mesh& _Mesh)
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		if (String::TheSame(Meshes[_Index].Name, _Mesh.Name))
		{
			DEBUG_BREAK();
			return;
		}
	}

	Meshes.push_back(_Mesh);
}

void Rasterizer::Model::EmplaceBack(Mesh && _Mesh) noexcept
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		if (String::TheSame(Meshes[_Index].Name, _Mesh.Name))
		{
			DEBUG_BREAK();
			return;
		}
	}

	Meshes.emplace_back(std::move(_Mesh));
}

void Rasterizer::Model::Erase(const size_t _Index)
{
	delete[] Meshes[_Index].Name;

	Meshes.erase(Meshes.begin() + _Index);
}

void Rasterizer::Model::Clear()
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		delete[] Meshes[_Index].Name;
	}

	Meshes.clear();
}

const size_t Rasterizer::Model::GetSize() const
{
	return Meshes.size();
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
