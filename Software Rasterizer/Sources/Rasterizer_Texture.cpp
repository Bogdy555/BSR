#include "..\Headers\BSR.hpp"



static bool ComputeWrap(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel, const uint8_t _WrapType, const size_t _TexturesSize, BSR::Math::Vec2f& _NewTextureCoords, float& _NewMipLevel)
{
	switch (_WrapType)
	{
	case BSR::Rasterizer::_WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x > 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y > 1.0f)
		{
			return false;
		}
		_NewMipLevel = BSR::Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case BSR::Rasterizer::_WrapClamp:
	{
		_NewTextureCoords.x = BSR::Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = BSR::Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);
		_NewMipLevel = BSR::Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case BSR::Rasterizer::_WrapRepeat:
	{
		_NewTextureCoords.x = _TextureCoords.x - floorf(_TextureCoords.x);
		_NewTextureCoords.y = _TextureCoords.y - floorf(_TextureCoords.y);
		_NewMipLevel = BSR::Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	case BSR::Rasterizer::_WrapMirror:
	{
		BSR::Math::Vec2f _Period(_TextureCoords.x - floorf(_TextureCoords.x), _TextureCoords.y - floorf(_TextureCoords.y));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);
		_NewMipLevel = BSR::Math::Clamp(_MipLevel, 0.0f, (float)(_TexturesSize - 1));

		break;
	}
	default:
	{
		return false;
	}
	}

	return true;
}



static const BSR::Math::Vec2f ComputeWrap(const BSR::Math::Vec2f& _TextureCoords, const uint8_t _WrapType)
{
	BSR::Math::Vec2f _NewTextureCoords;

	switch (_WrapType)
	{
	case BSR::Rasterizer::_WrapBlack:
	{
		_NewTextureCoords.x = BSR::Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = BSR::Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);

		break;
	}
	case BSR::Rasterizer::_WrapClamp:
	{
		_NewTextureCoords.x = BSR::Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = BSR::Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);

		break;
	}
	case BSR::Rasterizer::_WrapRepeat:
	{
		_NewTextureCoords.x = _TextureCoords.x - floorf(_TextureCoords.x);
		_NewTextureCoords.y = _TextureCoords.y - floorf(_TextureCoords.y);

		break;
	}
	case BSR::Rasterizer::_WrapMirror:
	{
		BSR::Math::Vec2f _Period(_TextureCoords.x - floorf(_TextureCoords.x), _TextureCoords.y - floorf(_TextureCoords.y));
		_NewTextureCoords.x = (_Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)(abs((int32_t)(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)(abs((int32_t)(floorf(_TextureCoords.y))) % 2 == 1);

		break;
	}
	default:
	{
		_NewTextureCoords.x = BSR::Math::Clamp(_TextureCoords.x, 0.0f, 1.0f);
		_NewTextureCoords.y = BSR::Math::Clamp(_TextureCoords.y, 0.0f, 1.0f);

		BSR_DEBUG_BREAK();

		break;
	}
	}

	return _NewTextureCoords;
}



BSR::Rasterizer::Texture::Texture() : LerpType(_LerpLinear), WrapType(_WrapRepeat)
{

}

BSR::Rasterizer::Texture::Texture(Texture&& _Other) noexcept : LerpType(_Other.LerpType), WrapType(_Other.WrapType)
{
	_Other.LerpType = _LerpLinear;
	_Other.WrapType = _WrapRepeat;
}

BSR::Rasterizer::Texture::~Texture()
{

}

void BSR::Rasterizer::Texture::SetLerpType(const uint8_t _LerpType)
{
	LerpType = _LerpType;
}

void BSR::Rasterizer::Texture::SetWrapType(const uint8_t _WrapType)
{
	WrapType = _WrapType;
}

const uint8_t BSR::Rasterizer::Texture::GetLerpType() const
{
	return LerpType;
}

const uint8_t BSR::Rasterizer::Texture::GetWrapType() const
{
	return WrapType;
}

void BSR::Rasterizer::Texture::operator= (Texture&& _Other) noexcept
{
	LerpType = _Other.LerpType;
	WrapType = _Other.WrapType;

	_Other.LerpType = _LerpLinear;
	_Other.WrapType = _WrapRepeat;
}



BSR::Rasterizer::Texture_R::Texture_R() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_R::Texture_R(Texture_R&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_R::~Texture_R()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const float BSR::Rasterizer::Texture_R::SampleR(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return 0.0f;
	}

	BSR::Math::Vec2f _NewTextureCoords;
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
		BSR_DEBUG_BREAK();
		return 0.0f;
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::Image& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::Image& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	float _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	float _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	float _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	float _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	float _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	float _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	float _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	float _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	float _ResultHighResDown = BSR::Math::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	float _ResultHighResUp = BSR::Math::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	float _ResultLowResDown = BSR::Math::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	float _ResultLowResUp = BSR::Math::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	float _ResultHighRes = BSR::Math::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	float _ResultLowRes = BSR::Math::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_R::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _R = SampleR(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_R, _R, _R, 1.0f);
}

bool BSR::Rasterizer::Texture_R::AddMip(const BSR::Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 1];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 1 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
		}
	}

	Textures.push_back(BSR::Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_R::AddDirectMip(BSR::Image::Image&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_R::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_R::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_R::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::Image& BSR::Rasterizer::Texture_R::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::Image& BSR::Rasterizer::Texture_R::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_R::operator= (Texture_R&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const float BSR::Rasterizer::Texture_R::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::Image& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0]) / 255.0f;

	return _R;
}



BSR::Rasterizer::Texture_RG::Texture_RG() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_RG::Texture_RG(Texture_RG&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_RG::~Texture_RG()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec2f BSR::Rasterizer::Texture_RG::SampleRG(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::Image& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::Image& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec2f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec2f _ResultHighResDown = BSR::Math::Vec2f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec2f _ResultHighResUp = BSR::Math::Vec2f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec2f _ResultLowResDown = BSR::Math::Vec2f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec2f _ResultLowResUp = BSR::Math::Vec2f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec2f _ResultHighRes = BSR::Math::Vec2f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec2f _ResultLowRes = BSR::Math::Vec2f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec2f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_RG::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const BSR::Math::Vec2f _RG = SampleRG(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_RG, 0.0f, 1.0f);
}

bool BSR::Rasterizer::Texture_RG::AddMip(const BSR::Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 2];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
		}
	}

	Textures.push_back(BSR::Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_RG::AddDirectMip(BSR::Image::Image&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_RG::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_RG::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_RG::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::Image& BSR::Rasterizer::Texture_RG::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::Image& BSR::Rasterizer::Texture_RG::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_RG::operator= (Texture_RG&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec2f BSR::Rasterizer::Texture_RG::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::Image& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1]) / 255.0f;

	return BSR::Math::Vec2f(_R, _G);
}



BSR::Rasterizer::Texture_RGB::Texture_RGB() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_RGB::Texture_RGB(Texture_RGB&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_RGB::~Texture_RGB()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec3f BSR::Rasterizer::Texture_RGB::SampleRGB(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::Image& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::Image& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec3f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec3f _ResultHighResDown = BSR::Math::Vec3f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec3f _ResultHighResUp = BSR::Math::Vec3f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec3f _ResultLowResDown = BSR::Math::Vec3f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec3f _ResultLowResUp = BSR::Math::Vec3f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec3f _ResultHighRes = BSR::Math::Vec3f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec3f _ResultLowRes = BSR::Math::Vec3f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec3f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_RGB::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const BSR::Math::Vec3f _RGB = SampleRGB(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_RGB, 1.0f);
}

bool BSR::Rasterizer::Texture_RGB::AddMip(const BSR::Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 3];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
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

	Textures.push_back(BSR::Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_RGB::AddDirectMip(BSR::Image::Image&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_RGB::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_RGB::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_RGB::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::Image& BSR::Rasterizer::Texture_RGB::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::Image& BSR::Rasterizer::Texture_RGB::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_RGB::operator= (Texture_RGB&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec3f BSR::Rasterizer::Texture_RGB::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::Image& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1]) / 255.0f;
	float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2]) / 255.0f;

	return BSR::Math::Vec3f(_R, _G, _B);
}



BSR::Rasterizer::Texture_RGBA::Texture_RGBA() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_RGBA::Texture_RGBA(Texture_RGBA&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_RGBA::~Texture_RGBA()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_RGBA::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::Image& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::Image& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec4f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec4f _ResultHighResDown = BSR::Math::Vec4f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec4f _ResultHighResUp = BSR::Math::Vec4f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec4f _ResultLowResDown = BSR::Math::Vec4f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec4f _ResultLowResUp = BSR::Math::Vec4f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec4f _ResultHighRes = BSR::Math::Vec4f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec4f _ResultLowRes = BSR::Math::Vec4f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec4f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

bool BSR::Rasterizer::Texture_RGBA::AddMip(const BSR::Image::Image& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::Image _NewImage;

	_NewImage.Data = new uint8_t[_Image.Width * _Image.Height * 4];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
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

	Textures.push_back(BSR::Image::Image());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_RGBA::AddDirectMip(BSR::Image::Image&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_RGBA::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_RGBA::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_RGBA::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::Image& BSR::Rasterizer::Texture_RGBA::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::Image& BSR::Rasterizer::Texture_RGBA::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_RGBA::operator= (Texture_RGBA&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_RGBA::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::Image& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0]) / 255.0f;
	float _G = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1]) / 255.0f;
	float _B = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2]) / 255.0f;
	float _A = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3]) / 255.0f;

	return BSR::Math::Vec4f(_R, _G, _B, _A);
}



BSR::Rasterizer::Texture_Float_R::Texture_Float_R() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_Float_R::Texture_Float_R(Texture_Float_R&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_Float_R::~Texture_Float_R()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const float BSR::Rasterizer::Texture_Float_R::SampleR(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return 0.0f;
	}

	BSR::Math::Vec2f _NewTextureCoords;
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
		BSR_DEBUG_BREAK();
		return 0.0f;
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::ImageFloat& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::ImageFloat& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	float _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	float _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	float _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	float _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	float _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	float _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	float _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	float _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	float _ResultHighResDown = BSR::Math::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	float _ResultHighResUp = BSR::Math::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	float _ResultLowResDown = BSR::Math::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	float _ResultLowResUp = BSR::Math::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	float _ResultHighRes = BSR::Math::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	float _ResultLowRes = BSR::Math::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_Float_R::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _R = SampleR(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_R, _R, _R, 1.0f);
}

bool BSR::Rasterizer::Texture_Float_R::AddMip(const BSR::Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 1];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 1 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
		}
	}

	Textures.push_back(BSR::Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_Float_R::AddDirectMip(BSR::Image::ImageFloat&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_Float_R::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_Float_R::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_Float_R::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_R::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_R::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_Float_R::operator= (Texture_Float_R&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const float BSR::Rasterizer::Texture_Float_R::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::ImageFloat& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 1 + 0];

	return _R;
}



BSR::Rasterizer::Texture_Float_RG::Texture_Float_RG() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_Float_RG::Texture_Float_RG(Texture_Float_RG&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_Float_RG::~Texture_Float_RG()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec2f BSR::Rasterizer::Texture_Float_RG::SampleRG(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec2f(0.0f, 0.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::ImageFloat& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::ImageFloat& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec2f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec2f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec2f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec2f _ResultHighResDown = BSR::Math::Vec2f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec2f _ResultHighResUp = BSR::Math::Vec2f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec2f _ResultLowResDown = BSR::Math::Vec2f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec2f _ResultLowResUp = BSR::Math::Vec2f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec2f _ResultHighRes = BSR::Math::Vec2f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec2f _ResultLowRes = BSR::Math::Vec2f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec2f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_Float_RG::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const BSR::Math::Vec2f _RG = SampleRG(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_RG, 0.0f, 1.0f);
}

bool BSR::Rasterizer::Texture_Float_RG::AddMip(const BSR::Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 2];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
	}

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			_NewImage.Data[(_X + _Y * _Image.Width) * 2 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
		}
	}

	Textures.push_back(BSR::Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_Float_RG::AddDirectMip(BSR::Image::ImageFloat&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_Float_RG::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_Float_RG::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_Float_RG::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RG::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RG::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_Float_RG::operator= (Texture_Float_RG&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec2f BSR::Rasterizer::Texture_Float_RG::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::ImageFloat& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 2 + 1];

	return BSR::Math::Vec2f(_R, _G);
}



BSR::Rasterizer::Texture_Float_RGB::Texture_Float_RGB() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_Float_RGB::Texture_Float_RGB(Texture_Float_RGB&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_Float_RGB::~Texture_Float_RGB()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec3f BSR::Rasterizer::Texture_Float_RGB::SampleRGB(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::ImageFloat& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::ImageFloat& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec3f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec3f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec3f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec3f _ResultHighResDown = BSR::Math::Vec3f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec3f _ResultHighResUp = BSR::Math::Vec3f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec3f _ResultLowResDown = BSR::Math::Vec3f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec3f _ResultLowResUp = BSR::Math::Vec3f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec3f _ResultHighRes = BSR::Math::Vec3f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec3f _ResultLowRes = BSR::Math::Vec3f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec3f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_Float_RGB::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const BSR::Math::Vec3f _RGB = SampleRGB(_TextureCoords, _MipLevel);

	return BSR::Math::Vec4f(_RGB, 1.0f);
}

bool BSR::Rasterizer::Texture_Float_RGB::AddMip(const BSR::Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 3];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
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

	Textures.push_back(BSR::Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_Float_RGB::AddDirectMip(BSR::Image::ImageFloat&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_Float_RGB::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_Float_RGB::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_Float_RGB::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RGB::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RGB::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_Float_RGB::operator= (Texture_Float_RGB&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec3f BSR::Rasterizer::Texture_Float_RGB::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::ImageFloat& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 1];
	float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 3 + 2];

	return BSR::Math::Vec3f(_R, _G, _B);
}



BSR::Rasterizer::Texture_Float_RGBA::Texture_Float_RGBA() : Texture(), Textures()
{

}

BSR::Rasterizer::Texture_Float_RGBA::Texture_Float_RGBA(Texture_Float_RGBA&& _Other) noexcept : Texture(std::move(_Other)), Textures(std::move(_Other.Textures))
{

}

BSR::Rasterizer::Texture_Float_RGBA::~Texture_Float_RGBA()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_Float_RGBA::SampleRGBA(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!Textures.size())
	{
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	BSR::Math::Vec2f _NewTextureCoords;
	float _NewMipLevel;

	if (!ComputeWrap(_TextureCoords, _MipLevel, WrapType, Textures.size(), _NewTextureCoords, _NewMipLevel))
	{
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(_NewTextureCoords, _NewMipLevel);
	}

	if (LerpType != _LerpLinear)
	{
		BSR_DEBUG_BREAK();
		return BSR::Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	const float _HighResMipLevel = floorf(_NewMipLevel);
	const float _LowResMipLevel = BSR::Math::Clamp(floorf(_NewMipLevel) + 1.0f, 0.0f, (float)(Textures.size() - 1));

	const BSR::Image::ImageFloat& _HighResTexture = Textures[(size_t)(_HighResMipLevel)];
	const BSR::Image::ImageFloat& _LowResTexture = Textures[(size_t)(_LowResMipLevel)];

	BSR::Math::Vec2f _PointHighResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 0.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 0.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointHighResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_HighResTexture.Width) - 0.5f) + 1.5f) / (float)(_HighResTexture.Width), (floorf(_TextureCoords.y * (float)(_HighResTexture.Height) - 0.5f) + 1.5f) / (float)(_HighResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResDownRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 0.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpLeft = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 0.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));
	BSR::Math::Vec2f _PointLowResUpRight = BSR::Math::Vec2f((floorf(_TextureCoords.x * (float)(_LowResTexture.Width) - 0.5f) + 1.5f) / (float)(_LowResTexture.Width), (floorf(_TextureCoords.y * (float)(_LowResTexture.Height) - 0.5f) + 1.5f) / (float)(_LowResTexture.Height));

	BSR::Math::Vec4f _ColorHighResDownLeft = SampleNearest(ComputeWrap(_PointHighResDownLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResDownRight = SampleNearest(ComputeWrap(_PointHighResDownRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResUpLeft = SampleNearest(ComputeWrap(_PointHighResUpLeft, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorHighResUpRight = SampleNearest(ComputeWrap(_PointHighResUpRight, WrapType), _HighResMipLevel);
	BSR::Math::Vec4f _ColorLowResDownLeft = SampleNearest(ComputeWrap(_PointLowResDownLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResDownRight = SampleNearest(ComputeWrap(_PointLowResDownRight, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResUpLeft = SampleNearest(ComputeWrap(_PointLowResUpLeft, WrapType), _LowResMipLevel);
	BSR::Math::Vec4f _ColorLowResUpRight = SampleNearest(ComputeWrap(_PointLowResUpRight, WrapType), _LowResMipLevel);

	BSR::Math::Vec4f _ResultHighResDown = BSR::Math::Vec4f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec4f _ResultHighResUp = BSR::Math::Vec4f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	BSR::Math::Vec4f _ResultLowResDown = BSR::Math::Vec4f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	BSR::Math::Vec4f _ResultLowResUp = BSR::Math::Vec4f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	BSR::Math::Vec4f _ResultHighRes = BSR::Math::Vec4f::Mix(_ResultHighResDown, _ResultHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	BSR::Math::Vec4f _ResultLowRes = BSR::Math::Vec4f::Mix(_ResultLowResDown, _ResultLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	return BSR::Math::Vec4f::Mix(_ResultHighRes, _ResultLowRes, _NewMipLevel - floorf(_NewMipLevel));
}

bool BSR::Rasterizer::Texture_Float_RGBA::AddMip(const BSR::Image::ImageFloat& _Image)
{
	if (!_Image.Data || !_Image.Width || !_Image.Height)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));
		return false;
	}

	BSR::Image::ImageFloat _NewImage;

	_NewImage.Data = new float[_Image.Width * _Image.Height * 4];
	_NewImage.Width = _Image.Width;
	_NewImage.Height = _Image.Height;

	if (!_NewImage.Data)
	{
		BSR_DEBUG_BREAK();
		return false;
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

	Textures.push_back(BSR::Image::ImageFloat());

	Textures[Textures.size() - 1].Data = _NewImage.Data;
	Textures[Textures.size() - 1].Width = _NewImage.Width;
	Textures[Textures.size() - 1].Height = _NewImage.Height;

	return true;
}

void BSR::Rasterizer::Texture_Float_RGBA::AddDirectMip(BSR::Image::ImageFloat&& _Image)
{
	BSR_ASSERT_MSG(_Image.Data && _Image.Width && _Image.Height, BSR_STRING_TYPE("Can not add an empty image to the texture mip chain."));

	Textures.emplace_back(std::move(_Image));
}

void BSR::Rasterizer::Texture_Float_RGBA::RemoveMip(const size_t _Index)
{
	delete[] Textures[_Index].Data;
	Textures.erase(Textures.begin() + _Index);
}

void BSR::Rasterizer::Texture_Float_RGBA::RemoveAllMips()
{
	for (size_t _Index = 0; _Index < Textures.size(); _Index++)
	{
		delete[] Textures[_Index].Data;
	}
	Textures.clear();
}

const size_t BSR::Rasterizer::Texture_Float_RGBA::GetMipsCount() const
{
	return Textures.size();
}

BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RGBA::operator[] (const size_t _Index)
{
	return Textures[_Index];
}

const BSR::Image::ImageFloat& BSR::Rasterizer::Texture_Float_RGBA::operator[] (const size_t _Index) const
{
	return Textures[_Index];
}

void BSR::Rasterizer::Texture_Float_RGBA::operator= (Texture_Float_RGBA&& _Other) noexcept
{
	RemoveAllMips();

	(Texture&)(*this) = std::move((Texture&)(_Other));

	Textures = std::move(_Other.Textures);
}

const BSR::Math::Vec4f BSR::Rasterizer::Texture_Float_RGBA::SampleNearest(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const BSR::Image::ImageFloat& _CurrentTexture = Textures[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(BSR::Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(BSR::Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	float _R = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 0];
	float _G = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 1];
	float _B = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 2];
	float _A = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * 4 + 3];

	return BSR::Math::Vec4f(_R, _G, _B, _A);
}
