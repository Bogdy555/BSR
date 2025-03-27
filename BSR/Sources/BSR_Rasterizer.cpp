#include "../Headers/BSR.hpp"



static const bool OutOfBounds(const BSR::Math::Vec2f& _TextureCoords, const float _MipLevel, const uint8_t _WrapType, const size_t _TextureChainSize)
{
	switch (_WrapType)
	{
	case BSR::Rasterizer::_WrapBlack:
	{
		if (_TextureCoords.x < 0.0f || _TextureCoords.x >= 1.0f || _TextureCoords.y < 0.0f || _TextureCoords.y >= 1.0f)
		{
			return true;
		}

		break;
	}
	case BSR::Rasterizer::_WrapClamp:
	{
		break;
	}
	case BSR::Rasterizer::_WrapRepeat:
	{
		break;
	}
	case BSR::Rasterizer::_WrapMirror:
	{
		break;
	}
	default:
	{
		return true;
	}
	}

	return false;
}

static const BSR::Math::Vec2f ComputeWrapTextureCoords(const BSR::Math::Vec2f& _TextureCoords, const uint8_t _WrapType)
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

		_NewTextureCoords.x = (_Period.x) * (float)((int32_t)(fabs(floorf(_TextureCoords.x))) % 2 == 0) + (1.0f - _Period.x) * (float)((int32_t)(fabs(floorf(_TextureCoords.x))) % 2 == 1);
		_NewTextureCoords.y = (_Period.y) * (float)((int32_t)(fabs(floorf(_TextureCoords.y))) % 2 == 0) + (1.0f - _Period.y) * (float)((int32_t)(fabs(floorf(_TextureCoords.y))) % 2 == 1);

		break;
	}
	}

	return _NewTextureCoords;
}



const BSR::Math::Vec4f BSR::Rasterizer::TextureSDR::Sample(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!TextureChain.GetSize() || !ChannelsCount || (LerpType != _LerpNearest && LerpType != _LerpLinear) || (WrapType != _WrapBlack && WrapType != _WrapClamp && WrapType != _WrapRepeat && WrapType != _WrapMirror))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (OutOfBounds(_TextureCoords, _MipLevel, WrapType, TextureChain.GetSize()))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(ComputeWrapTextureCoords(_TextureCoords, WrapType), Math::Clamp(_MipLevel, 0.0f, (float)(TextureChain.GetSize() - 1)));
	}

	float _NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(TextureChain.GetSize() - 1));
	float _MipLevelHighRes = floorf(_NewMipLevel);
	float _MipLevelLowRes = Math::Clamp(_MipLevelHighRes + 1.0f, 0.0f, (float)(TextureChain.GetSize() - 1));

	const Image::SDR& _ImageHighRes = TextureChain[(size_t)(_MipLevelHighRes)];
	const Image::SDR& _ImageLowRes = TextureChain[(size_t)(_MipLevelLowRes)];

	Math::Vec2f _PointHighResDownLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResDownRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResUpLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResUpRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointLowResDownLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResDownRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResUpLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResUpRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Height));

	Math::Vec4f _ColorHighResDownLeft = SampleNearest(ComputeWrapTextureCoords(_PointHighResDownLeft, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResDownRight = SampleNearest(ComputeWrapTextureCoords(_PointHighResDownRight, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResUpLeft = SampleNearest(ComputeWrapTextureCoords(_PointHighResUpLeft, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResUpRight = SampleNearest(ComputeWrapTextureCoords(_PointHighResUpRight, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorLowResDownLeft = SampleNearest(ComputeWrapTextureCoords(_PointLowResDownLeft, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResDownRight = SampleNearest(ComputeWrapTextureCoords(_PointLowResDownRight, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResUpLeft = SampleNearest(ComputeWrapTextureCoords(_PointLowResUpLeft, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResUpRight = SampleNearest(ComputeWrapTextureCoords(_PointLowResUpRight, WrapType), _MipLevelLowRes);

	Math::Vec4f _ColorHighResDown = Math::Vec4f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	Math::Vec4f _ColorHighResUp = Math::Vec4f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	Math::Vec4f _ColorLowResDown = Math::Vec4f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	Math::Vec4f _ColorLowResUp = Math::Vec4f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	Math::Vec4f _ColorHighRes = Math::Vec4f::Mix(_ColorHighResDown, _ColorHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	Math::Vec4f _ColorLowRes = Math::Vec4f::Mix(_ColorLowResDown, _ColorLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	Math::Vec4f _Color = Math::Vec4f::Mix(_ColorHighRes, _ColorLowRes, _NewMipLevel - _MipLevelHighRes);

	return _Color;
}

const BSR::Math::Vec4f BSR::Rasterizer::TextureSDR::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::SDR& _CurrentTexture = TextureChain[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	Math::Vec4f _Result(0.0f, 0.0f, 0.0f, 1.0f);

	for (size_t _Index = 0; _Index < ChannelsCount; _Index++)
	{
		_Result[_Index] = (float)(_CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * ChannelsCount + _Index]) / 255.0f;
	}

	return _Result;
}



const BSR::Math::Vec4f BSR::Rasterizer::TextureHDR::Sample(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	if (!TextureChain.GetSize() || !ChannelsCount || (LerpType != _LerpNearest && LerpType != _LerpLinear) || (WrapType != _WrapBlack && WrapType != _WrapClamp && WrapType != _WrapRepeat && WrapType != _WrapMirror))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (OutOfBounds(_TextureCoords, _MipLevel, WrapType, TextureChain.GetSize()))
	{
		return Math::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (LerpType == _LerpNearest)
	{
		return SampleNearest(ComputeWrapTextureCoords(_TextureCoords, WrapType), Math::Clamp(_MipLevel, 0.0f, (float)(TextureChain.GetSize() - 1)));
	}

	float _NewMipLevel = Math::Clamp(_MipLevel, 0.0f, (float)(TextureChain.GetSize() - 1));
	float _MipLevelHighRes = floorf(_NewMipLevel);
	float _MipLevelLowRes = Math::Clamp(_MipLevelHighRes + 1.0f, 0.0f, (float)(TextureChain.GetSize() - 1));

	const Image::HDR& _ImageHighRes = TextureChain[(size_t)(_MipLevelHighRes)];
	const Image::HDR& _ImageLowRes = TextureChain[(size_t)(_MipLevelLowRes)];

	Math::Vec2f _PointHighResDownLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResDownRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResUpLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 0.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointHighResUpRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageHighRes.Width) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Width), (floorf(_TextureCoords.y * (float)(_ImageHighRes.Height) - 0.5f) + 1.5f) / (float)(_ImageHighRes.Height));
	Math::Vec2f _PointLowResDownLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResDownRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResUpLeft = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 0.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Height));
	Math::Vec2f _PointLowResUpRight = Math::Vec2f((floorf(_TextureCoords.x * (float)(_ImageLowRes.Width) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Width), (floorf(_TextureCoords.y * (float)(_ImageLowRes.Height) - 0.5f) + 1.5f) / (float)(_ImageLowRes.Height));

	Math::Vec4f _ColorHighResDownLeft = SampleNearest(ComputeWrapTextureCoords(_PointHighResDownLeft, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResDownRight = SampleNearest(ComputeWrapTextureCoords(_PointHighResDownRight, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResUpLeft = SampleNearest(ComputeWrapTextureCoords(_PointHighResUpLeft, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorHighResUpRight = SampleNearest(ComputeWrapTextureCoords(_PointHighResUpRight, WrapType), _MipLevelHighRes);
	Math::Vec4f _ColorLowResDownLeft = SampleNearest(ComputeWrapTextureCoords(_PointLowResDownLeft, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResDownRight = SampleNearest(ComputeWrapTextureCoords(_PointLowResDownRight, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResUpLeft = SampleNearest(ComputeWrapTextureCoords(_PointLowResUpLeft, WrapType), _MipLevelLowRes);
	Math::Vec4f _ColorLowResUpRight = SampleNearest(ComputeWrapTextureCoords(_PointLowResUpRight, WrapType), _MipLevelLowRes);

	Math::Vec4f _ColorHighResDown = Math::Vec4f::Mix(_ColorHighResDownLeft, _ColorHighResDownRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	Math::Vec4f _ColorHighResUp = Math::Vec4f::Mix(_ColorHighResUpLeft, _ColorHighResUpRight, (_TextureCoords.x - _PointHighResDownLeft.x) / (_PointHighResDownRight.x - _PointHighResDownLeft.x));
	Math::Vec4f _ColorLowResDown = Math::Vec4f::Mix(_ColorLowResDownLeft, _ColorLowResDownRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));
	Math::Vec4f _ColorLowResUp = Math::Vec4f::Mix(_ColorLowResUpLeft, _ColorLowResUpRight, (_TextureCoords.x - _PointLowResDownLeft.x) / (_PointLowResDownRight.x - _PointLowResDownLeft.x));

	Math::Vec4f _ColorHighRes = Math::Vec4f::Mix(_ColorHighResDown, _ColorHighResUp, (_TextureCoords.y - _PointHighResDownLeft.y) / (_PointHighResUpLeft.y - _PointHighResDownLeft.y));
	Math::Vec4f _ColorLowRes = Math::Vec4f::Mix(_ColorLowResDown, _ColorLowResUp, (_TextureCoords.y - _PointLowResDownLeft.y) / (_PointLowResUpLeft.y - _PointLowResDownLeft.y));

	Math::Vec4f _Color = Math::Vec4f::Mix(_ColorHighRes, _ColorLowRes, _NewMipLevel - _MipLevelHighRes);

	return _Color;
}

const BSR::Math::Vec4f BSR::Rasterizer::TextureHDR::SampleNearest(const Math::Vec2f& _TextureCoords, const float _MipLevel) const
{
	const float _NewMipLevel = _MipLevel + 0.5f;

	const Image::HDR& _CurrentTexture = TextureChain[(size_t)(floorf(_NewMipLevel))];

	size_t _ImageX = (size_t)(Math::Clamp(_TextureCoords.x * (float)(_CurrentTexture.Width), 0.0f, (float)(_CurrentTexture.Width - 1)));
	size_t _ImageY = (size_t)(Math::Clamp(_TextureCoords.y * (float)(_CurrentTexture.Height), 0.0f, (float)(_CurrentTexture.Height - 1)));

	Math::Vec4f _Result(0.0f, 0.0f, 0.0f, 1.0f);

	for (size_t _Index = 0; _Index < ChannelsCount; _Index++)
	{
		_Result[_Index] = _CurrentTexture.Data[(_ImageX + _ImageY * _CurrentTexture.Width) * ChannelsCount + _Index];
	}

	return _Result;
}



void BSR::Rasterizer::Context::DrawCall()
{
	Vector<PointData> _PointsData;
	VertexPass(_PointsData);
	Vector<Shape> _Shapes;
	GeometryPass(_PointsData, _Shapes);
	for (size_t _Index = 0; _Index < _PointsData.GetSize(); _Index++)
	{
		delete[] _PointsData[_Index].Lerpers;
	}
	_PointsData.Clear();
	ClippingPass(_Shapes);
	CullingPass(_Shapes);
	RasterizingPass(_Shapes);
	for (size_t _Index = 0; _Index < _Shapes.GetSize(); _Index++)
	{
		delete[] _Shapes[_Index].A.Lerpers;
		delete[] _Shapes[_Index].B.Lerpers;
		delete[] _Shapes[_Index].C.Lerpers;
	}
	_Shapes.Clear();
}

const bool BSR::Rasterizer::Context::DepthTest(const float _NewDepth, const float _OldDepth, const uint8_t _DepthTestingType)
{
	switch (_DepthTestingType)
	{
	case _NoDepthTesting:
	{
		return true;
	}
	case _LowerDepthTesting:
	{
		if (_NewDepth >= _OldDepth)
		{
			return false;
		}

		return true;
	}
	case _HigherDepthTesting:
	{
		if (_NewDepth <= _OldDepth)
		{
			return false;
		}

		return true;
	}
	case _LowerOrEqualDepthTesting:
	{
		if (_NewDepth > _OldDepth)
		{
			return false;
		}

		return true;
	}
	case _HigherOrEqualDepthTesting:
	{
		if (_NewDepth < _OldDepth)
		{
			return false;
		}

		return true;
	}
	default:
	{
		break;
	}
	}

	return false;
}

const BSR::Math::Vec3f BSR::Rasterizer::Context::Blend(const Math::Vec3f& _OldColor, const Math::Vec4f& _Color, const uint8_t _BlendingType, const bool _Clamp)
{
	switch (_BlendingType)
	{
	case _NoBlending:
	{
		if (_Clamp)
		{
			return Math::Vec3f::Clamp(Math::Vec3f(_Color), Math::Vec3f(0.0f, 0.0f, 0.0f), Math::Vec3f(1.0f, 1.0f, 1.0f));
		}

		return Math::Vec3f(_Color);
	}
	case _AditiveBlending:
	{
		if (_Clamp)
		{
			return Math::Vec3f::Clamp(_OldColor + Math::Vec3f(_Color) * _Color.w, Math::Vec3f(0.0f, 0.0f, 0.0f), Math::Vec3f(1.0f, 1.0f, 1.0f));
		}

		return _OldColor + Math::Vec3f(_Color) * _Color.w;
	}
	case _AlphaBlending:
	{
		if (_Clamp)
		{
			return Math::Vec3f::Clamp(_OldColor * (1.0f - _Color.w) + Math::Vec3f(_Color) * _Color.w, Math::Vec3f(0.0f, 0.0f, 0.0f), Math::Vec3f(1.0f, 1.0f, 1.0f));
		}

		return _OldColor * (1.0f - _Color.w) + Math::Vec3f(_Color) * _Color.w;
	}
	default:
	{
		break;
	}
	}

	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

void BSR::Rasterizer::Context::VertexPass(Vector<PointData>& _Output)
{
	for (size_t _IndexVBO = 0; _IndexVBO < VBOSize; _IndexVBO++)
	{
		PointData _Data;

		if (LerpersCountVertToGeom)
		{
			_Data.Lerpers = new float[LerpersCountVertToGeom];
		}

		_Data.Position = VertexShader((void*)((uint8_t*)(VBO) + _IndexVBO * VBOStride), Uniforms, _Data.Lerpers);

		_Output.PushBack(_Data);
	}
}

void BSR::Rasterizer::Context::GeometryPass(const Vector<PointData>& _Input, Vector<Shape>& _Output)
{
	if (GeometryShader)
	{
		for (size_t _IndexIBO = IBOBegin; _IndexIBO < IBOEnd; _IndexIBO += 3)
		{
			Vector<Math::Vec4f> _Positions;
			Vector<float*> _Lerpers;

			GeometryShader(_Input[IBO[_IndexIBO + 0]].Position, _Input[IBO[_IndexIBO + 1]].Position, _Input[IBO[_IndexIBO + 2]].Position, _Input[IBO[_IndexIBO + 0]].Lerpers, _Input[IBO[_IndexIBO + 1]].Lerpers, _Input[IBO[_IndexIBO + 2]].Lerpers, Uniforms, _Positions, _Lerpers);

			for (size_t _IndexTriangle = 0; _IndexTriangle < _Positions.GetSize(); _IndexTriangle += 3)
			{
				Shape _Data;

				_Data.A.Position = _Positions[_IndexTriangle + 0];
				_Data.B.Position = _Positions[_IndexTriangle + 1];
				_Data.C.Position = _Positions[_IndexTriangle + 2];

				_Data.A.Lerpers = _Lerpers[_IndexTriangle + 0];
				_Data.B.Lerpers = _Lerpers[_IndexTriangle + 1];
				_Data.C.Lerpers = _Lerpers[_IndexTriangle + 2];

				_Output.PushBack(_Data);
			}
		}
	}
	else
	{
		for (size_t _IndexIBO = IBOBegin; _IndexIBO < IBOEnd; _IndexIBO += 3)
		{
			Shape _Data;

			_Data.A.Position = _Input[IBO[_IndexIBO + 0]].Position;
			_Data.B.Position = _Input[IBO[_IndexIBO + 1]].Position;
			_Data.C.Position = _Input[IBO[_IndexIBO + 2]].Position;

			if (LerpersCountGeomToFrag)
			{
				_Data.A.Lerpers = new float[LerpersCountGeomToFrag];
				_Data.B.Lerpers = new float[LerpersCountGeomToFrag];
				_Data.C.Lerpers = new float[LerpersCountGeomToFrag];

				for (size_t _IndexCopy = 0; _IndexCopy < LerpersCountGeomToFrag; _IndexCopy++)
				{
					_Data.A.Lerpers[_IndexCopy] = _Input[IBO[_IndexIBO + 0]].Lerpers[_IndexCopy];
					_Data.B.Lerpers[_IndexCopy] = _Input[IBO[_IndexIBO + 1]].Lerpers[_IndexCopy];
					_Data.C.Lerpers[_IndexCopy] = _Input[IBO[_IndexIBO + 2]].Lerpers[_IndexCopy];
				}
			}

			_Output.PushBack(_Data);
		}
	}
}

void BSR::Rasterizer::Context::ClippingPass(Vector<Shape>& _Shapes)
{
	for (size_t _IndexTriangle = 0; _IndexTriangle < _Shapes.GetSize(); _IndexTriangle++)
	{
		if (!_Shapes[_IndexTriangle].Render)
		{
			continue;
		}

		bool _AInside = InsideNearPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);
		bool _BInside = InsideNearPlane(_Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w);
		bool _CInside = InsideNearPlane(_Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w);

		size_t _CountInside = (size_t)(_AInside) + (size_t)(_BInside) + (size_t)(_CInside);

		switch (_CountInside)
		{
		case 0:
		{
			delete[] _Shapes[_IndexTriangle].A.Lerpers;
			delete[] _Shapes[_IndexTriangle].B.Lerpers;
			delete[] _Shapes[_IndexTriangle].C.Lerpers;

			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		case 1:
		{
			if (_BInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Aux.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Aux.Lerpers;
			}

			if (_CInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Aux.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Aux.Lerpers;
			}

			float _TB = GetTNearPlane(_Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w, _Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);
			float _TC = GetTNearPlane(_Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w, _Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);

			Shape _NewTriangle;

			_NewTriangle.A.Position = _Shapes[_IndexTriangle].A.Position;
			_NewTriangle.A.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

			_NewTriangle.B.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].B.Position, _Shapes[_IndexTriangle].A.Position, _TB);
			_NewTriangle.B.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;
			LerpAll(_Shapes[_IndexTriangle].B.Lerpers, _Shapes[_IndexTriangle].A.Lerpers, LerpersCountGeomToFrag, _TB, _NewTriangle.B.Lerpers);

			_NewTriangle.C.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].C.Position, _Shapes[_IndexTriangle].A.Position, _TC);
			_NewTriangle.C.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;
			LerpAll(_Shapes[_IndexTriangle].C.Lerpers, _Shapes[_IndexTriangle].A.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle.C.Lerpers);

			_Shapes.PushBack(_NewTriangle);

			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		case 2:
		{
			if (!_BInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Aux.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Aux.Lerpers;
			}

			if (!_CInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Aux.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Aux.Lerpers;
			}

			float _TB = GetTNearPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w, _Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w);
			float _TC = GetTNearPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w, _Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w);

			Shape _NewTriangle1;
			Shape _NewTriangle2;

			if (LerpersCountGeomToFrag)
			{
				_NewTriangle1.A.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle1.B.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle1.C.Lerpers = new float[LerpersCountGeomToFrag];

				_NewTriangle2.A.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle2.B.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle2.C.Lerpers = new float[LerpersCountGeomToFrag];

				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].B.Lerpers, LerpersCountGeomToFrag, _TB, _NewTriangle1.A.Lerpers);
				CopyAll(_Shapes[_IndexTriangle].B.Lerpers, _NewTriangle1.B.Lerpers, LerpersCountGeomToFrag);
				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle1.C.Lerpers);

				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle2.A.Lerpers);
				CopyAll(_Shapes[_IndexTriangle].B.Lerpers, _NewTriangle2.B.Lerpers, LerpersCountGeomToFrag);
				CopyAll(_Shapes[_IndexTriangle].C.Lerpers, _NewTriangle2.C.Lerpers, LerpersCountGeomToFrag);
			}

			_NewTriangle1.A.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].B.Position, _TB);
			_NewTriangle1.B.Position = _Shapes[_IndexTriangle].B.Position;
			_NewTriangle1.C.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].C.Position, _TC);

			_NewTriangle2.A.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].C.Position, _TC);
			_NewTriangle2.B.Position = _Shapes[_IndexTriangle].B.Position;
			_NewTriangle2.C.Position = _Shapes[_IndexTriangle].C.Position;

			_Shapes.PushBack(_NewTriangle1);
			_Shapes.PushBack(_NewTriangle2);

			delete[] _Shapes[_IndexTriangle].A.Lerpers;
			delete[] _Shapes[_IndexTriangle].B.Lerpers;
			delete[] _Shapes[_IndexTriangle].C.Lerpers;
			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		}
	}

	for (size_t _IndexTriangle = 0; _IndexTriangle < _Shapes.GetSize(); _IndexTriangle++)
	{
		if (!_Shapes[_IndexTriangle].Render)
		{
			continue;
		}

		bool _AInside = InsideFarPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);
		bool _BInside = InsideFarPlane(_Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w);
		bool _CInside = InsideFarPlane(_Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w);

		size_t _CountInside = (size_t)(_AInside) + (size_t)(_BInside) + (size_t)(_CInside);

		switch (_CountInside)
		{
		case 0:
		{
			delete[] _Shapes[_IndexTriangle].A.Lerpers;
			delete[] _Shapes[_IndexTriangle].B.Lerpers;
			delete[] _Shapes[_IndexTriangle].C.Lerpers;

			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		case 1:
		{
			if (_BInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Aux.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Aux.Lerpers;
			}

			if (_CInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Aux.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Aux.Lerpers;
			}

			float _TB = GetTFarPlane(_Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w, _Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);
			float _TC = GetTFarPlane(_Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w, _Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w);

			Shape _NewTriangle;

			_NewTriangle.A.Position = _Shapes[_IndexTriangle].A.Position;
			_NewTriangle.A.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

			_NewTriangle.B.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].B.Position, _Shapes[_IndexTriangle].A.Position, _TB);
			_NewTriangle.B.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;
			LerpAll(_Shapes[_IndexTriangle].B.Lerpers, _Shapes[_IndexTriangle].A.Lerpers, LerpersCountGeomToFrag, _TB, _NewTriangle.B.Lerpers);

			_NewTriangle.C.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].C.Position, _Shapes[_IndexTriangle].A.Position, _TC);
			_NewTriangle.C.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;
			LerpAll(_Shapes[_IndexTriangle].C.Lerpers, _Shapes[_IndexTriangle].A.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle.C.Lerpers);

			_Shapes.PushBack(_NewTriangle);

			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		case 2:
		{
			if (!_BInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Aux.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Aux.Lerpers;
			}

			if (!_CInside)
			{
				PointData _Aux;

				_Aux.Position = _Shapes[_IndexTriangle].A.Position;
				_Aux.Lerpers = _Shapes[_IndexTriangle].A.Lerpers;

				_Shapes[_IndexTriangle].A.Position = _Shapes[_IndexTriangle].C.Position;
				_Shapes[_IndexTriangle].A.Lerpers = _Shapes[_IndexTriangle].C.Lerpers;

				_Shapes[_IndexTriangle].C.Position = _Shapes[_IndexTriangle].B.Position;
				_Shapes[_IndexTriangle].C.Lerpers = _Shapes[_IndexTriangle].B.Lerpers;

				_Shapes[_IndexTriangle].B.Position = _Aux.Position;
				_Shapes[_IndexTriangle].B.Lerpers = _Aux.Lerpers;
			}

			float _TB = GetTFarPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w, _Shapes[_IndexTriangle].B.Position.z, _Shapes[_IndexTriangle].B.Position.w);
			float _TC = GetTFarPlane(_Shapes[_IndexTriangle].A.Position.z, _Shapes[_IndexTriangle].A.Position.w, _Shapes[_IndexTriangle].C.Position.z, _Shapes[_IndexTriangle].C.Position.w);

			Shape _NewTriangle1;
			Shape _NewTriangle2;

			if (LerpersCountGeomToFrag)
			{
				_NewTriangle1.A.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle1.B.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle1.C.Lerpers = new float[LerpersCountGeomToFrag];

				_NewTriangle2.A.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle2.B.Lerpers = new float[LerpersCountGeomToFrag];
				_NewTriangle2.C.Lerpers = new float[LerpersCountGeomToFrag];

				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].B.Lerpers, LerpersCountGeomToFrag, _TB, _NewTriangle1.A.Lerpers);
				CopyAll(_Shapes[_IndexTriangle].B.Lerpers, _NewTriangle1.B.Lerpers, LerpersCountGeomToFrag);
				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle1.C.Lerpers);

				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, _TC, _NewTriangle2.A.Lerpers);
				CopyAll(_Shapes[_IndexTriangle].B.Lerpers, _NewTriangle2.B.Lerpers, LerpersCountGeomToFrag);
				CopyAll(_Shapes[_IndexTriangle].C.Lerpers, _NewTriangle2.C.Lerpers, LerpersCountGeomToFrag);
			}

			_NewTriangle1.A.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].B.Position, _TB);
			_NewTriangle1.B.Position = _Shapes[_IndexTriangle].B.Position;
			_NewTriangle1.C.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].C.Position, _TC);

			_NewTriangle2.A.Position = Math::Vec4f::Mix(_Shapes[_IndexTriangle].A.Position, _Shapes[_IndexTriangle].C.Position, _TC);
			_NewTriangle2.B.Position = _Shapes[_IndexTriangle].B.Position;
			_NewTriangle2.C.Position = _Shapes[_IndexTriangle].C.Position;

			_Shapes.PushBack(_NewTriangle1);
			_Shapes.PushBack(_NewTriangle2);

			delete[] _Shapes[_IndexTriangle].A.Lerpers;
			delete[] _Shapes[_IndexTriangle].B.Lerpers;
			delete[] _Shapes[_IndexTriangle].C.Lerpers;
			_Shapes[_IndexTriangle].Render = false;
			_Shapes[_IndexTriangle].A.Lerpers = nullptr;
			_Shapes[_IndexTriangle].B.Lerpers = nullptr;
			_Shapes[_IndexTriangle].C.Lerpers = nullptr;

			break;
		}
		}
	}
}

void BSR::Rasterizer::Context::CullingPass(Vector<Shape>& _Shapes)
{
	switch (CullingType)
	{
	case _NoCulling:
	{
		for (size_t _Index = 0; _Index < _Shapes.GetSize(); _Index++)
		{
			if (!_Shapes[_Index].Render)
			{
				continue;
			}

			Math::Vec3f _A = Math::Vec3f(_Shapes[_Index].A.Position) / _Shapes[_Index].A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_Shapes[_Index].B.Position) / _Shapes[_Index].B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_Shapes[_Index].C.Position) / _Shapes[_Index].C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z == 0.0f)
			{
				delete[] _Shapes[_Index].A.Lerpers;
				delete[] _Shapes[_Index].B.Lerpers;
				delete[] _Shapes[_Index].C.Lerpers;
				_Shapes[_Index].Render = false;
				_Shapes[_Index].A.Lerpers = nullptr;
				_Shapes[_Index].B.Lerpers = nullptr;
				_Shapes[_Index].C.Lerpers = nullptr;
			}
		}

		break;
	}
	case _ClockWiseCulling:
	{
		for (size_t _Index = 0; _Index < _Shapes.GetSize(); _Index++)
		{
			if (!_Shapes[_Index].Render)
			{
				continue;
			}

			Math::Vec3f _A = Math::Vec3f(_Shapes[_Index].A.Position) / _Shapes[_Index].A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_Shapes[_Index].B.Position) / _Shapes[_Index].B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_Shapes[_Index].C.Position) / _Shapes[_Index].C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z <= 0.0f)
			{
				delete[] _Shapes[_Index].A.Lerpers;
				delete[] _Shapes[_Index].B.Lerpers;
				delete[] _Shapes[_Index].C.Lerpers;
				_Shapes[_Index].Render = false;
				_Shapes[_Index].A.Lerpers = nullptr;
				_Shapes[_Index].B.Lerpers = nullptr;
				_Shapes[_Index].C.Lerpers = nullptr;
			}
		}

		break;
	}
	case _CounterClockWiseCulling:
	{
		for (size_t _Index = 0; _Index < _Shapes.GetSize(); _Index++)
		{
			if (!_Shapes[_Index].Render)
			{
				continue;
			}

			Math::Vec3f _A = Math::Vec3f(_Shapes[_Index].A.Position) / _Shapes[_Index].A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_Shapes[_Index].B.Position) / _Shapes[_Index].B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_Shapes[_Index].C.Position) / _Shapes[_Index].C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z >= 0.0f)
			{
				delete[] _Shapes[_Index].A.Lerpers;
				delete[] _Shapes[_Index].B.Lerpers;
				delete[] _Shapes[_Index].C.Lerpers;
				_Shapes[_Index].Render = false;
				_Shapes[_Index].A.Lerpers = nullptr;
				_Shapes[_Index].B.Lerpers = nullptr;
				_Shapes[_Index].C.Lerpers = nullptr;
			}
		}

		break;
	}
	}
}

void BSR::Rasterizer::Context::RasterizingPass(Vector<Shape>& _Shapes)
{
	float* _FragmentLerpers = nullptr;

	if (LerpersCountGeomToFrag)
	{
		_FragmentLerpers = new float[LerpersCountGeomToFrag];
	}

	for (size_t _IndexTriangle = 0; _IndexTriangle < _Shapes.GetSize(); _IndexTriangle++)
	{
		if (!_Shapes[_IndexTriangle].Render)
		{
			continue;
		}

		MultiplyAll(_Shapes[_IndexTriangle].A.Lerpers, LerpersCountGeomToFrag, 1.0f / _Shapes[_IndexTriangle].A.Position.w);
		MultiplyAll(_Shapes[_IndexTriangle].B.Lerpers, LerpersCountGeomToFrag, 1.0f / _Shapes[_IndexTriangle].B.Position.w);
		MultiplyAll(_Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, 1.0f / _Shapes[_IndexTriangle].C.Position.w);

		Math::Vec3f _ScreenA = Math::Vec3f(_Shapes[_IndexTriangle].A.Position) / _Shapes[_IndexTriangle].A.Position.w;
		Math::Vec3f _ScreenB = Math::Vec3f(_Shapes[_IndexTriangle].B.Position) / _Shapes[_IndexTriangle].B.Position.w;
		Math::Vec3f _ScreenC = Math::Vec3f(_Shapes[_IndexTriangle].C.Position) / _Shapes[_IndexTriangle].C.Position.w;

		bool _FrontFacing = Math::Vec3f::Cross(_ScreenB - _ScreenA, _ScreenC - _ScreenA).z > 0.0f;

		size_t _StartX = (size_t)(Math::Clamp(floorf(Math::Min(Math::Min((_ScreenA.x + 1.0f) / 2.0f, (_ScreenB.x + 1.0f) / 2.0f), (_ScreenC.x + 1.0f) / 2.0f) * (float)(ViewPortWidth)), 0.0f, (float)(ViewPortWidth)));
		size_t _StartY = (size_t)(Math::Clamp(floorf(Math::Min(Math::Min((_ScreenA.y + 1.0f) / 2.0f, (_ScreenB.y + 1.0f) / 2.0f), (_ScreenC.y + 1.0f) / 2.0f) * (float)(ViewPortHeight)), 0.0f, (float)(ViewPortHeight)));

		size_t _EndX = (size_t)(Math::Clamp(ceilf(Math::Max(Math::Max((_ScreenA.x + 1.0f) / 2.0f, (_ScreenB.x + 1.0f) / 2.0f), (_ScreenC.x + 1.0f) / 2.0f) * (float)(ViewPortWidth)), 0.0f, (float)(ViewPortWidth)));
		size_t _EndY = (size_t)(Math::Clamp(ceilf(Math::Max(Math::Max((_ScreenA.y + 1.0f) / 2.0f, (_ScreenB.y + 1.0f) / 2.0f), (_ScreenC.y + 1.0f) / 2.0f) * (float)(ViewPortHeight)), 0.0f, (float)(ViewPortHeight)));

		for (size_t _Y = ViewPortY + _StartY; _Y < ViewPortY + _EndY; _Y++)
		{
			for (size_t _X = ViewPortX + _StartX; _X < ViewPortX + _EndX; _X++)
			{
				Math::Vec2f _ScreenP = Math::Vec2f(((float)(_X - ViewPortX) / (float)(ViewPortWidth) + 0.5f / (float)(ViewPortWidth)) * 2.0f - 1.0f, ((float)(_Y - ViewPortY) / (float)(ViewPortHeight) + 0.5f / (float)(ViewPortHeight)) * 2.0f - 1.0f);

				if (!PointInside(_ScreenP, Math::Vec2f(_ScreenA), Math::Vec2f(_ScreenB), Math::Vec2f(_ScreenC)))
				{
					continue;
				}

				float _T1 = GetT1((Math::Vec2f)(_ScreenA), (Math::Vec2f)(_ScreenB), (Math::Vec2f)(_ScreenC), _ScreenP);
				float _T2 = GetT2((Math::Vec2f)(_ScreenA), (Math::Vec2f)(_ScreenB), (Math::Vec2f)(_ScreenC), _ScreenP, _T1);

				float _PerspectiveCorrection = Math::Mix(Math::Mix(1.0f / _Shapes[_IndexTriangle].A.Position.w, 1.0f / _Shapes[_IndexTriangle].B.Position.w, _T1), 1.0f / _Shapes[_IndexTriangle].C.Position.w, _T2);

				LerpAll(_Shapes[_IndexTriangle].A.Lerpers, _Shapes[_IndexTriangle].B.Lerpers, LerpersCountGeomToFrag, _T1, _FragmentLerpers);
				LerpAll(_FragmentLerpers, _Shapes[_IndexTriangle].C.Lerpers, LerpersCountGeomToFrag, _T2, _FragmentLerpers);
				MultiplyAll(_FragmentLerpers, LerpersCountGeomToFrag, 1.0f / _PerspectiveCorrection);

				Math::Vec4f _FragCoord = Math::Vec4f(_ScreenP.x, _ScreenP.y, Math::Mix(Math::Mix(_ScreenA.z, _ScreenB.z, _T1), _ScreenC.z, _T2), _PerspectiveCorrection);
				_FragCoord.z = (_FragCoord.z + 1.0f) / 2.0f;

				FragmentShader(_X, _Y, _X - ViewPortX, _Y - ViewPortY, _FragmentLerpers, Uniforms, FrameBuffer, _FragCoord, _FrontFacing, DepthTestingType, BlendingType);
			}
		}
	}

	delete[] _FragmentLerpers;
}

const bool BSR::Rasterizer::Context::InsideNearPlane(const float _Z, const float _W)
{
	return _W > 0.0f && _Z / _W >= -1.0f;
}

const float BSR::Rasterizer::Context::GetTNearPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn)
{
	return (_ZOut + _WOut) / (_ZOut - _ZIn + _WOut - _WIn);
}

const bool BSR::Rasterizer::Context::InsideFarPlane(const float _Z, const float _W)
{
	return _W > 0.0f && _Z / _W <= 1.0f;
}

const float BSR::Rasterizer::Context::GetTFarPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn)
{
	if (_ZOut - _ZIn - _WOut + _WIn == 0.0f)
	{
		return 0.5f;
	}

	return (_ZOut - _WOut) / (_ZOut - _ZIn - _WOut + _WIn);
}

void BSR::Rasterizer::Context::LerpAll(const float* _A, const float* _B, const size_t _LerpersCount, const float _Percentage, float* _Out)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Out[_Index] = Math::Mix(_A[_Index], _B[_Index], _Percentage);
	}
}

void BSR::Rasterizer::Context::CopyAll(const float* _Src, float* _Dest, const size_t _LerpersCount)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Dest[_Index] = _Src[_Index];
	}
}

void BSR::Rasterizer::Context::MultiplyAll(float* _Out, const size_t _LerpersCount, const float _Value)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Out[_Index] *= _Value;
	}
}

const bool BSR::Rasterizer::Context::PointInside(const Math::Vec2f& _P, const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C)
{
	Math::Mat3f _OriginalMat;

	_OriginalMat[0][0] = _A.x; _OriginalMat[0][1] = _A.y; _OriginalMat[0][2] = 1.0f;
	_OriginalMat[1][0] = _B.x; _OriginalMat[1][1] = _B.y; _OriginalMat[1][2] = 1.0f;
	_OriginalMat[2][0] = _C.x; _OriginalMat[2][1] = _C.y; _OriginalMat[2][2] = 1.0f;

	Math::Mat3f _MatABP;
	Math::Mat3f _MatBCP;
	Math::Mat3f _MatCAP;

	_MatABP[0][0] = _A.x; _MatABP[0][1] = _A.y; _MatABP[0][2] = 1.0f;
	_MatABP[1][0] = _B.x; _MatABP[1][1] = _B.y; _MatABP[1][2] = 1.0f;
	_MatABP[2][0] = _P.x; _MatABP[2][1] = _P.y; _MatABP[2][2] = 1.0f;

	_MatBCP[0][0] = _B.x; _MatBCP[0][1] = _B.y; _MatBCP[0][2] = 1.0f;
	_MatBCP[1][0] = _C.x; _MatBCP[1][1] = _C.y; _MatBCP[1][2] = 1.0f;
	_MatBCP[2][0] = _P.x; _MatBCP[2][1] = _P.y; _MatBCP[2][2] = 1.0f;

	_MatCAP[0][0] = _C.x; _MatCAP[0][1] = _C.y; _MatCAP[0][2] = 1.0f;
	_MatCAP[1][0] = _A.x; _MatCAP[1][1] = _A.y; _MatCAP[1][2] = 1.0f;
	_MatCAP[2][0] = _P.x; _MatCAP[2][1] = _P.y; _MatCAP[2][2] = 1.0f;

	return fabs(fabs(_OriginalMat.Determinant()) - fabs(_MatABP.Determinant()) - fabs(_MatBCP.Determinant()) - fabs(_MatCAP.Determinant())) <= 0.00001f;
}

const float BSR::Rasterizer::Context::GetT1(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P)
{
	if (_A.x == _B.x)
	{
		Math::Vec3f _CP((_P.y - _C.y) / (_P.x - _C.x), -1.0f, _C.y - _C.x * (_P.y - _C.y) / (_P.x - _C.x));

		float _YIntersect = _A.x * _CP.x + _CP.z;

		return Math::Clamp((_YIntersect - _A.y) / (_B.y - _A.y), 0.0f, 1.0f);
	}
	else
	{
		if (_C.x == _P.x)
		{
			float _XIntersect = _C.x;

			return Math::Clamp((_XIntersect - _A.x) / (_B.x - _A.x), 0.0f, 1.0f);
		}
		else
		{
			Math::Vec3f _AB((_B.y - _A.y) / (_B.x - _A.x), -1.0f, _A.y - _A.x * (_B.y - _A.y) / (_B.x - _A.x));
			Math::Vec3f _CP((_P.y - _C.y) / (_P.x - _C.x), -1.0f, _C.y - _C.x * (_P.y - _C.y) / (_P.x - _C.x));

			float _XIntersect = (_CP.z - _AB.z) / (_AB.x - _CP.x);

			return Math::Clamp((_XIntersect - _A.x) / (_B.x - _A.x), 0.0f, 1.0f);
		}
	}

	return 0.0f;
}

const float BSR::Rasterizer::Context::GetT2(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P, const float _T1)
{
	Math::Vec2f _IntersectPoint = Math::Vec2f::Mix(_A, _B, _T1);

	return Math::Clamp((_IntersectPoint - _P).Magnitude() / (_C - _IntersectPoint).Magnitude(), 0.0f, 1.0f);
}
