#include "..\Headers\Main.hpp"



static const bool PointInside(const Math::Vec2f& _Point, const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C)
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
	_MatABP[2][0] = _Point.x; _MatABP[2][1] = _Point.y; _MatABP[2][2] = 1.0f;

	_MatBCP[0][0] = _B.x; _MatBCP[0][1] = _B.y; _MatBCP[0][2] = 1.0f;
	_MatBCP[1][0] = _C.x; _MatBCP[1][1] = _C.y; _MatBCP[1][2] = 1.0f;
	_MatBCP[2][0] = _Point.x; _MatBCP[2][1] = _Point.y; _MatBCP[2][2] = 1.0f;

	_MatCAP[0][0] = _C.x; _MatCAP[0][1] = _C.y; _MatCAP[0][2] = 1.0f;
	_MatCAP[1][0] = _A.x; _MatCAP[1][1] = _A.y; _MatCAP[1][2] = 1.0f;
	_MatCAP[2][0] = _Point.x; _MatCAP[2][1] = _Point.y; _MatCAP[2][2] = 1.0f;

	return abs(abs(_OriginalMat.Determinant()) - abs(_MatABP.Determinant()) - abs(_MatBCP.Determinant()) - abs(_MatCAP.Determinant())) <= 0.0000001f;
}



static void LerpAll(const float* _A, const float* _B, const size_t _Length, const float _Percentage, float* _Out)
{
	for (size_t _Index = 0; _Index < _Length; _Index++)
	{
		_Out[_Index] = Math::Mix(_A[_Index], _B[_Index], _Percentage);
	}
}



Rasterizer::Context::Context() : ViewPortX(0), ViewPortY(0), ViewPortWidth(0), ViewPortHeight(0), CullingType(_ClockWiseCulling), DepthTestingType(_LowerOrEqualDepthTesting), BlendingType(_AlphaBlending)
{

}

Rasterizer::Context::Context(const Context& _Other) : ViewPortX(_Other.ViewPortX), ViewPortY(_Other.ViewPortY), ViewPortWidth(_Other.ViewPortWidth), ViewPortHeight(_Other.ViewPortHeight), CullingType(_Other.CullingType), DepthTestingType(_Other.DepthTestingType), BlendingType(_Other.BlendingType)
{

}

Rasterizer::Context::Context(Context&& _Other) noexcept : ViewPortX(_Other.ViewPortX), ViewPortY(_Other.ViewPortY), ViewPortWidth(_Other.ViewPortWidth), ViewPortHeight(_Other.ViewPortHeight), CullingType(_Other.CullingType), DepthTestingType(_Other.DepthTestingType), BlendingType(_Other.BlendingType)
{
	_Other.ViewPortX = 0;
	_Other.ViewPortY = 0;
	_Other.ViewPortWidth = 0;
	_Other.ViewPortHeight = 0;
	_Other.CullingType = _ClockWiseCulling;
	_Other.DepthTestingType = _LowerOrEqualDepthTesting;
	_Other.BlendingType = _AlphaBlending;
}

Rasterizer::Context::~Context()
{

}

const bool Rasterizer::Context::RenderMesh(const void* _VBO, const size_t _VBOSize, const size_t _VBOStride, const size_t* _IBO, const size_t _IBOBegin, const size_t _IBOEnd, const void* _Uniforms, const size_t _LerpersCount, const VertexShaderFnc _VertexShader, const GeometryShaderFnc _GeometryShader, const FragmentShaderFnc _FragmentShader, void* _FrameBuffer) const
{
	struct VertexShaderOutput
	{
		Math::Vec4f Position = Math::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		float* Lerpers = nullptr;
	};

	std::vector<VertexShaderOutput> _VertexShaderOutputs;

	for (size_t _IndexVBO = 0; _IndexVBO < _VBOSize; _IndexVBO++)
	{
		VertexShaderOutput _Output;

		if (_LerpersCount)
		{
			_Output.Lerpers = new float[_LerpersCount];

			if (!_Output.Lerpers)
			{
				DEBUG_BREAK();

				for (size_t _IndexDelete = 0; _IndexDelete < _VertexShaderOutputs.size(); _IndexDelete++)
				{
					delete[] _VertexShaderOutputs[_IndexDelete].Lerpers;
				}

				return false;
			}
		}

		_Output.Position = _VertexShader((void*)((uint8_t*)(_VBO) + _IndexVBO * _VBOStride), _Uniforms, _Output.Lerpers);

		_VertexShaderOutputs.push_back(_Output);
	}

	struct GeometryShaderOutput
	{
		VertexShaderOutput A;
		VertexShaderOutput B;
		VertexShaderOutput C;
	};

	std::vector<GeometryShaderOutput> _GeometryShaderOutputs;

	if (_GeometryShader)
	{
		for (size_t _IndexIBO = _IBOBegin; _IndexIBO < _IBOEnd; _IndexIBO += 3)
		{
			std::vector<Math::Vec4f> _PositionsOut;
			std::vector<float*> _LerpersOut;

			if (!_GeometryShader(_VertexShaderOutputs[_IBO[_IndexIBO + 0]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 0]].Lerpers, _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Lerpers, _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Lerpers, _Uniforms, _PositionsOut, _LerpersOut))
			{
				DEBUG_BREAK();

				for (size_t _IndexDelete = 0; _IndexDelete < _VertexShaderOutputs.size(); _IndexDelete++)
				{
					delete[] _VertexShaderOutputs[_IndexDelete].Lerpers;
				}

				for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
				{
					delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
					delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
					delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
				}

				return false;
			}

			if (_PositionsOut.size() != _LerpersOut.size() || _PositionsOut.size() % 3 != 0)
			{
				DEBUG_BREAK();

				for (size_t _IndexDelete = 0; _IndexDelete < _LerpersOut.size(); _IndexDelete++)
				{
					delete[] _LerpersOut[_IndexDelete];
				}

				for (size_t _IndexDelete = 0; _IndexDelete < _VertexShaderOutputs.size(); _IndexDelete++)
				{
					delete[] _VertexShaderOutputs[_IndexDelete].Lerpers;
				}

				for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
				{
					delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
					delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
					delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
				}

				return false;
			}

			for (size_t _IndexTriangle = 0; _IndexTriangle < _PositionsOut.size(); _IndexTriangle += 3)
			{
				GeometryShaderOutput _Output;

				_Output.A.Position = _PositionsOut[_IndexTriangle + 0];
				_Output.B.Position = _PositionsOut[_IndexTriangle + 1];
				_Output.C.Position = _PositionsOut[_IndexTriangle + 2];

				_Output.A.Lerpers = _LerpersOut[_IndexTriangle + 0];
				_Output.B.Lerpers = _LerpersOut[_IndexTriangle + 1];
				_Output.C.Lerpers = _LerpersOut[_IndexTriangle + 2];

				_GeometryShaderOutputs.push_back(_Output);
			}
		}
	}
	else
	{
		for (size_t _IndexIBO = _IBOBegin; _IndexIBO < _IBOEnd; _IndexIBO += 3)
		{
			GeometryShaderOutput _Output;

			if (_LerpersCount)
			{
				_Output.A.Lerpers = new float[_LerpersCount];
				_Output.B.Lerpers = new float[_LerpersCount];
				_Output.C.Lerpers = new float[_LerpersCount];

				if (!_Output.A.Lerpers || !_Output.B.Lerpers || !_Output.C.Lerpers)
				{
					DEBUG_BREAK();

					delete[] _Output.A.Lerpers;
					delete[] _Output.B.Lerpers;
					delete[] _Output.C.Lerpers;

					for (size_t _IndexDelete = 0; _IndexDelete < _VertexShaderOutputs.size(); _IndexDelete++)
					{
						delete[] _VertexShaderOutputs[_IndexDelete].Lerpers;
					}

					for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
					{
						delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
					}

					return false;
				}

				for (size_t _IndexCopy = 0; _IndexCopy < _LerpersCount; _IndexCopy++)
				{
					_Output.A.Lerpers[_IndexCopy] = _VertexShaderOutputs[_IBO[_IndexIBO + 0]].Lerpers[_IndexCopy];
					_Output.B.Lerpers[_IndexCopy] = _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Lerpers[_IndexCopy];
					_Output.C.Lerpers[_IndexCopy] = _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Lerpers[_IndexCopy];
				}
			}

			_Output.A.Position = _VertexShaderOutputs[_IBO[_IndexIBO + 0]].Position;
			_Output.B.Position = _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Position;
			_Output.C.Position = _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Position;

			_GeometryShaderOutputs.push_back(_Output);
		}
	}

	for (size_t _IndexDelete = 0; _IndexDelete < _VertexShaderOutputs.size(); _IndexDelete++)
	{
		delete[] _VertexShaderOutputs[_IndexDelete].Lerpers;
	}

	_VertexShaderOutputs.clear();

	//TODO: Clipping

	switch (CullingType)
	{
	case _NoCulling:
	{
		for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
		{
			GeometryShaderOutput& _CurrentTriangle = _GeometryShaderOutputs[_IndexTriangle];

			Math::Vec3f _A = Math::Vec3f(_CurrentTriangle.A.Position) / _CurrentTriangle.A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_CurrentTriangle.B.Position) / _CurrentTriangle.B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_CurrentTriangle.C.Position) / _CurrentTriangle.C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z == 0.0f)
			{
				delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

				_IndexTriangle--;
			}
		}

		break;
	}
	case _ClockWiseCulling:
	{
		for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
		{
			GeometryShaderOutput& _CurrentTriangle = _GeometryShaderOutputs[_IndexTriangle];

			Math::Vec3f _A = Math::Vec3f(_CurrentTriangle.A.Position) / _CurrentTriangle.A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_CurrentTriangle.B.Position) / _CurrentTriangle.B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_CurrentTriangle.C.Position) / _CurrentTriangle.C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z <= 0.0f)
			{
				delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

				_IndexTriangle--;
			}
		}

		break;
	}
	case _CounterClockWiseCulling:
	{
		for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
		{
			GeometryShaderOutput& _CurrentTriangle = _GeometryShaderOutputs[_IndexTriangle];

			Math::Vec3f _A = Math::Vec3f(_CurrentTriangle.A.Position) / _CurrentTriangle.A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_CurrentTriangle.B.Position) / _CurrentTriangle.B.Position.w;
			Math::Vec3f _C = Math::Vec3f(_CurrentTriangle.C.Position) / _CurrentTriangle.C.Position.w;

			if (Math::Vec3f::Cross(_B - _A, _C - _A).z >= 0.0f)
			{
				delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

				_IndexTriangle--;
			}
		}

		break;
	}
	default:
	{
		DEBUG_BREAK();

		for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
		{
			delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
		}

		return false;
	}
	}

	float* _FragmentLerpers = nullptr;

	if (_LerpersCount)
	{
		_FragmentLerpers = new float[_LerpersCount];

		if (!_FragmentLerpers)
		{
			for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
			{
				delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
				delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
			}

			return false;
		}
	}

	for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
	{
		GeometryShaderOutput& _CurrentTriangle = _GeometryShaderOutputs[_IndexTriangle];

		Math::Vec3f _A = Math::Vec3f(_CurrentTriangle.A.Position) / _CurrentTriangle.A.Position.w;
		Math::Vec3f _B = Math::Vec3f(_CurrentTriangle.B.Position) / _CurrentTriangle.B.Position.w;
		Math::Vec3f _C = Math::Vec3f(_CurrentTriangle.C.Position) / _CurrentTriangle.C.Position.w;

		bool _FrontFacing = Math::Vec3f::Cross(_B - _A, _C - _A).z > 0.0f;

		size_t _StartX = (size_t)(floorf(Math::Min(Math::Min((_A.x + 1.0f) / 2.0f, (_B.x + 1.0f) / 2.0f), (_C.x + 1.0f) / 2.0f) * (float)(ViewPortWidth)));
		size_t _StartY = (size_t)(floorf(Math::Min(Math::Min((_A.y + 1.0f) / 2.0f, (_B.y + 1.0f) / 2.0f), (_C.y + 1.0f) / 2.0f) * (float)(ViewPortHeight)));

		size_t _EndX = (size_t)(ceilf(Math::Max(Math::Max((_A.x + 1.0f) / 2.0f, (_B.x + 1.0f) / 2.0f), (_C.x + 1.0f) / 2.0f) * (float)(ViewPortWidth))) + 1;
		size_t _EndY = (size_t)(ceilf(Math::Max(Math::Max((_A.y + 1.0f) / 2.0f, (_B.y + 1.0f) / 2.0f), (_C.y + 1.0f) / 2.0f) * (float)(ViewPortHeight))) + 1;

		for (size_t _Y = ViewPortY + _StartY; _Y < ViewPortY + _EndY; _Y++)
		{
			for (size_t _X = ViewPortX + _StartX; _X < ViewPortX + _EndX; _X++)
			{
				Math::Vec2f _Point = Math::Vec2f(((float)(_X) / (float)(ViewPortWidth) + 0.5f / (float)(ViewPortWidth)) * 2.0f - 1.0f, ((float)(_Y) / (float)(ViewPortHeight) + 0.5f / (float)(ViewPortHeight)) * 2.0f - 1.0f);

				if (PointInside(_Point, Math::Vec2f(_A), Math::Vec2f(_B), Math::Vec2f(_C)))
				{
					float _T1 = 0.0f;
					float _T2 = 0.0f;

					LerpAll(_CurrentTriangle.A.Lerpers, _CurrentTriangle.B.Lerpers, _LerpersCount, _T1, _FragmentLerpers);
					LerpAll(_FragmentLerpers, _CurrentTriangle.C.Lerpers, _LerpersCount, _T2, _FragmentLerpers);

					Math::Vec4f _FragCoord = Math::Vec4f::Mix(Math::Vec4f::Mix(_CurrentTriangle.A.Position, _CurrentTriangle.B.Position, _T1), _CurrentTriangle.C.Position, _T2);
					float _FragDepth = (_FragCoord.z / _FragCoord.w + 1.0f) / 2.0f;

					_FragmentShader(_X, _Y, _FragmentLerpers, _Uniforms, _FrameBuffer, _FragCoord, _FragDepth, _FrontFacing, DepthTestingType, BlendingType);
				}
			}
		}
	}

	delete[] _FragmentLerpers;

	for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
	{
		delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
		delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
		delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
	}

	_GeometryShaderOutputs.clear();

	return true;
}

void Rasterizer::Context::SetViewPort(const size_t _ViewPortX, const size_t _ViewPortY, const size_t _ViewPortWidth, const size_t _ViewPortHeight)
{
	ViewPortX = _ViewPortX;
	ViewPortY = _ViewPortY;
	ViewPortWidth = _ViewPortWidth;
	ViewPortHeight = _ViewPortHeight;
}

void Rasterizer::Context::SetCullingType(const uint8_t _CullingType)
{
	CullingType = _CullingType;
}

void Rasterizer::Context::SetDepthTestingType(const uint8_t _DepthTestingType)
{
	DepthTestingType = _DepthTestingType;
}

void Rasterizer::Context::SetBlendingType(const uint8_t _BlendingType)
{
	BlendingType = _BlendingType;
}

void Rasterizer::Context::GetViewPort(size_t& _ViewPortX, size_t& _ViewPortY, size_t& _ViewPortWidth, size_t& _ViewPortHeight) const
{
	_ViewPortX = ViewPortX;
	_ViewPortY = ViewPortY;
	_ViewPortWidth = ViewPortWidth;
	_ViewPortHeight = ViewPortHeight;
}

const uint8_t Rasterizer::Context::GetCullingType() const
{
	return CullingType;
}

const uint8_t Rasterizer::Context::GetDepthTestingType() const
{
	return DepthTestingType;
}

const uint8_t Rasterizer::Context::GetBlendingType() const
{
	return BlendingType;
}

void Rasterizer::Context::operator= (const Context& _Other)
{
	ViewPortX = _Other.ViewPortX;
	ViewPortY = _Other.ViewPortY;
	ViewPortWidth = _Other.ViewPortWidth;
	ViewPortHeight = _Other.ViewPortHeight;
	CullingType = _Other.CullingType;
	DepthTestingType = _Other.DepthTestingType;
	BlendingType = _Other.BlendingType;
}

void Rasterizer::Context::operator= (Context&& _Other) noexcept
{
	ViewPortX = _Other.ViewPortX;
	ViewPortY = _Other.ViewPortY;
	ViewPortWidth = _Other.ViewPortWidth;
	ViewPortHeight = _Other.ViewPortHeight;
	CullingType = _Other.CullingType;
	DepthTestingType = _Other.DepthTestingType;
	BlendingType = _Other.BlendingType;

	_Other.ViewPortX = 0;
	_Other.ViewPortY = 0;
	_Other.ViewPortWidth = 0;
	_Other.ViewPortHeight = 0;
	_Other.CullingType = _ClockWiseCulling;
	_Other.DepthTestingType = _LowerOrEqualDepthTesting;
	_Other.BlendingType = _AlphaBlending;
}
