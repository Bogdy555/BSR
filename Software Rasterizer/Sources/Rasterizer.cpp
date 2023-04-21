#include "..\Headers\Main.hpp"



static const bool PointInside(const Math::Vec2f& _P, const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C)
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

	return abs(abs(_OriginalMat.Determinant()) - abs(_MatABP.Determinant()) - abs(_MatBCP.Determinant()) - abs(_MatCAP.Determinant())) <= 0.00001f;
}

static const float GetT1(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P)
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

	DEBUG_BREAK();

	return 0.0f;
}

static const float GetT2(const Math::Vec2f& _A, const Math::Vec2f& _B, const Math::Vec2f& _C, const Math::Vec2f& _P, const float _T1)
{
	Math::Vec2f _IntersectPoint = Math::Vec2f::Mix(_A, _B, _T1);

	return Math::Clamp((_IntersectPoint - _P).Magnitude() / (_C - _IntersectPoint).Magnitude(), 0.0f, 1.0f);
}

static void MultiplyAll(float* _Out, const size_t _LerpersCount, const float _Value)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Out[_Index] *= _Value;
	}
}

static void LerpAll(const float* _A, const float* _B, const size_t _LerpersCount, const float _Percentage, float* _Out)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Out[_Index] = Math::Mix(_A[_Index], _B[_Index], _Percentage);
	}
}

static void CopyAll(const float* _Src, float* _Dest, const size_t _LerpersCount)
{
	for (size_t _Index = 0; _Index < _LerpersCount; _Index++)
	{
		_Dest[_Index] = _Src[_Index];
	}
}

static const bool InsideNearPlane(const float _Z, const float _W)
{
	return _W > 0.0f && _Z / _W >= -1.0f;
}

static const bool InsideFarPlane(const float _Z, const float _W)
{
	return _W > 0.0f && _Z / _W <= 1.0f;
}

static const float GetTNearPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn)
{
	return (_ZOut + _WOut) / (_ZOut - _ZIn + _WOut - _WIn);
}

static const float GetTFarPlane(const float _ZOut, const float _WOut, const float _ZIn, const float _WIn)
{
	if (_ZOut - _ZIn - _WOut + _WIn == 0.0f)
	{
		return 0.5f;
	}

	return (_ZOut - _WOut) / (_ZOut - _ZIn - _WOut + _WIn);
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

const bool Rasterizer::Context::RenderMesh(const void* _VBO, const size_t _VBOSize, const size_t _VBOStride, const size_t* _IBO, const size_t _IBOBegin, const size_t _IBOEnd, const void* _Uniforms, const size_t _LerpersCountVertToGeom, const size_t _LerpersCountGeomToFrag, const VertexShaderFnc _VertexShader, const GeometryShaderFnc _GeometryShader, const FragmentShaderFnc _FragmentShader, void* _FrameBuffer) const
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

		if (_LerpersCountVertToGeom)
		{
			_Output.Lerpers = new float[_LerpersCountVertToGeom];

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
		ASSERT_MSG(_LerpersCountVertToGeom == _LerpersCountGeomToFrag, STRING_TYPE("No geometry shader assigned so the count for vertex to geometry lerpers should be the same as the count of lerpers from geometry shader to fragment shader."));

		for (size_t _IndexIBO = _IBOBegin; _IndexIBO < _IBOEnd; _IndexIBO += 3)
		{
			GeometryShaderOutput _Output;

			if (_LerpersCountGeomToFrag)
			{
				_Output.A.Lerpers = new float[_LerpersCountGeomToFrag];
				_Output.B.Lerpers = new float[_LerpersCountGeomToFrag];
				_Output.C.Lerpers = new float[_LerpersCountGeomToFrag];

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

				for (size_t _IndexCopy = 0; _IndexCopy < _LerpersCountGeomToFrag; _IndexCopy++)
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

	for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
	{
		bool _AInside = InsideNearPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);
		bool _BInside = InsideNearPlane(_GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w);
		bool _CInside = InsideNearPlane(_GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w);

		size_t _CountInside = (size_t)(_AInside) + (size_t)(_BInside) + (size_t)(_CInside);

		switch (_CountInside)
		{
		case 0:
		{
			delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 1:
		{
			if (_BInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _AuxLerpers;
			}

			if (_CInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _AuxLerpers;
			}

			float _TB = GetTNearPlane(_GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w, _GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);
			float _TC = GetTNearPlane(_GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w, _GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);

			GeometryShaderOutput _NewTriangle;

			_NewTriangle.A.Position = _GeometryShaderOutputs[_IndexTriangle].A.Position;
			_NewTriangle.A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

			_NewTriangle.B.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].B.Position, _GeometryShaderOutputs[_IndexTriangle].A.Position, _TB);
			_NewTriangle.B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			LerpAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _LerpersCountGeomToFrag, _TB, _NewTriangle.B.Lerpers);

			_NewTriangle.C.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].C.Position, _GeometryShaderOutputs[_IndexTriangle].A.Position, _TC);
			_NewTriangle.C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;
			LerpAll(_GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle.C.Lerpers);

			_GeometryShaderOutputs.push_back(_NewTriangle);

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 2:
		{
			if (!_BInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _AuxLerpers;
			}

			if (!_CInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _AuxLerpers;
			}

			float _TB = GetTNearPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w, _GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w);
			float _TC = GetTNearPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w, _GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w);

			GeometryShaderOutput _NewTriangle1;
			GeometryShaderOutput _NewTriangle2;

			if (_LerpersCountGeomToFrag)
			{
				_NewTriangle1.A.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle1.B.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle1.C.Lerpers = new float[_LerpersCountGeomToFrag];

				_NewTriangle2.A.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle2.B.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle2.C.Lerpers = new float[_LerpersCountGeomToFrag];

				if (!_NewTriangle1.A.Lerpers || !_NewTriangle1.B.Lerpers || !_NewTriangle1.C.Lerpers || !_NewTriangle2.A.Lerpers || !_NewTriangle2.B.Lerpers || !_NewTriangle2.C.Lerpers)
				{
					delete[] _NewTriangle1.A.Lerpers;
					delete[] _NewTriangle1.B.Lerpers;
					delete[] _NewTriangle1.C.Lerpers;

					delete[] _NewTriangle2.A.Lerpers;
					delete[] _NewTriangle2.B.Lerpers;
					delete[] _NewTriangle2.C.Lerpers;

					for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
					{
						delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
					}

					return false;
				}

				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _LerpersCountGeomToFrag, _TB, _NewTriangle1.A.Lerpers);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _NewTriangle1.B.Lerpers, _LerpersCountGeomToFrag);
				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle1.C.Lerpers);

				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle2.A.Lerpers);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _NewTriangle2.B.Lerpers, _LerpersCountGeomToFrag);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _NewTriangle2.C.Lerpers, _LerpersCountGeomToFrag);
			}

			_NewTriangle1.A.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].B.Position, _TB);
			_NewTriangle1.B.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
			_NewTriangle1.C.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].C.Position, _TC);

			_NewTriangle2.A.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].C.Position, _TC);
			_NewTriangle2.B.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
			_NewTriangle2.C.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;

			_GeometryShaderOutputs.push_back(_NewTriangle1);
			_GeometryShaderOutputs.push_back(_NewTriangle2);

			delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 3:
		{
			break;
		}
		}
	}

	for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
	{
		bool _AInside = InsideFarPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);
		bool _BInside = InsideFarPlane(_GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w);
		bool _CInside = InsideFarPlane(_GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w);

		size_t _CountInside = (size_t)(_AInside) + (size_t)(_BInside) + (size_t)(_CInside);

		switch (_CountInside)
		{
		case 0:
		{
			delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 1:
		{
			if (_BInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _AuxLerpers;
			}

			if (_CInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _AuxLerpers;
			}

			float _TB = GetTFarPlane(_GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w, _GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);
			float _TC = GetTFarPlane(_GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w, _GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w);

			GeometryShaderOutput _NewTriangle;

			_NewTriangle.A.Position = _GeometryShaderOutputs[_IndexTriangle].A.Position;
			_NewTriangle.A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

			_NewTriangle.B.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].B.Position, _GeometryShaderOutputs[_IndexTriangle].A.Position, _TB);
			_NewTriangle.B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			LerpAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _LerpersCountGeomToFrag, _TB, _NewTriangle.B.Lerpers);

			_NewTriangle.C.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].C.Position, _GeometryShaderOutputs[_IndexTriangle].A.Position, _TC);
			_NewTriangle.C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;
			LerpAll(_GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle.C.Lerpers);

			_GeometryShaderOutputs.push_back(_NewTriangle);

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 2:
		{
			if (!_BInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _AuxLerpers;
			}

			if (!_CInside)
			{
				Math::Vec4f _AuxPosition = _GeometryShaderOutputs[_IndexTriangle].A.Position;
				float* _AuxLerpers = _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].A.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;
				_GeometryShaderOutputs[_IndexTriangle].A.Lerpers = _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].C.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
				_GeometryShaderOutputs[_IndexTriangle].C.Lerpers = _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;

				_GeometryShaderOutputs[_IndexTriangle].B.Position = _AuxPosition;
				_GeometryShaderOutputs[_IndexTriangle].B.Lerpers = _AuxLerpers;
			}

			float _TB = GetTFarPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w, _GeometryShaderOutputs[_IndexTriangle].B.Position.z, _GeometryShaderOutputs[_IndexTriangle].B.Position.w);
			float _TC = GetTFarPlane(_GeometryShaderOutputs[_IndexTriangle].A.Position.z, _GeometryShaderOutputs[_IndexTriangle].A.Position.w, _GeometryShaderOutputs[_IndexTriangle].C.Position.z, _GeometryShaderOutputs[_IndexTriangle].C.Position.w);

			GeometryShaderOutput _NewTriangle1;
			GeometryShaderOutput _NewTriangle2;

			if (_LerpersCountGeomToFrag)
			{
				_NewTriangle1.A.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle1.B.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle1.C.Lerpers = new float[_LerpersCountGeomToFrag];

				_NewTriangle2.A.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle2.B.Lerpers = new float[_LerpersCountGeomToFrag];
				_NewTriangle2.C.Lerpers = new float[_LerpersCountGeomToFrag];

				if (!_NewTriangle1.A.Lerpers || !_NewTriangle1.B.Lerpers || !_NewTriangle1.C.Lerpers || !_NewTriangle2.A.Lerpers || !_NewTriangle2.B.Lerpers || !_NewTriangle2.C.Lerpers)
				{
					delete[] _NewTriangle1.A.Lerpers;
					delete[] _NewTriangle1.B.Lerpers;
					delete[] _NewTriangle1.C.Lerpers;

					delete[] _NewTriangle2.A.Lerpers;
					delete[] _NewTriangle2.B.Lerpers;
					delete[] _NewTriangle2.C.Lerpers;

					for (size_t _IndexDelete = 0; _IndexDelete < _GeometryShaderOutputs.size(); _IndexDelete++)
					{
						delete[] _GeometryShaderOutputs[_IndexDelete].A.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].B.Lerpers;
						delete[] _GeometryShaderOutputs[_IndexDelete].C.Lerpers;
					}

					return false;
				}

				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _LerpersCountGeomToFrag, _TB, _NewTriangle1.A.Lerpers);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _NewTriangle1.B.Lerpers, _LerpersCountGeomToFrag);
				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle1.C.Lerpers);

				LerpAll(_GeometryShaderOutputs[_IndexTriangle].A.Lerpers, _GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _LerpersCountGeomToFrag, _TC, _NewTriangle2.A.Lerpers);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].B.Lerpers, _NewTriangle2.B.Lerpers, _LerpersCountGeomToFrag);
				CopyAll(_GeometryShaderOutputs[_IndexTriangle].C.Lerpers, _NewTriangle2.C.Lerpers, _LerpersCountGeomToFrag);
			}

			_NewTriangle1.A.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].B.Position, _TB);
			_NewTriangle1.B.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
			_NewTriangle1.C.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].C.Position, _TC);

			_NewTriangle2.A.Position = Math::Vec4f::Mix(_GeometryShaderOutputs[_IndexTriangle].A.Position, _GeometryShaderOutputs[_IndexTriangle].C.Position, _TC);
			_NewTriangle2.B.Position = _GeometryShaderOutputs[_IndexTriangle].B.Position;
			_NewTriangle2.C.Position = _GeometryShaderOutputs[_IndexTriangle].C.Position;

			_GeometryShaderOutputs.push_back(_NewTriangle1);
			_GeometryShaderOutputs.push_back(_NewTriangle2);

			delete[] _GeometryShaderOutputs[_IndexTriangle].A.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].B.Lerpers;
			delete[] _GeometryShaderOutputs[_IndexTriangle].C.Lerpers;

			_GeometryShaderOutputs.erase(_GeometryShaderOutputs.begin() + _IndexTriangle);

			_IndexTriangle--;

			break;
		}
		case 3:
		{
			break;
		}
		}
	}

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

	if (_LerpersCountGeomToFrag)
	{
		_FragmentLerpers = new float[_LerpersCountGeomToFrag];

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

		MultiplyAll(_CurrentTriangle.A.Lerpers, _LerpersCountGeomToFrag, 1.0f / _CurrentTriangle.A.Position.w);
		MultiplyAll(_CurrentTriangle.B.Lerpers, _LerpersCountGeomToFrag, 1.0f / _CurrentTriangle.B.Position.w);
		MultiplyAll(_CurrentTriangle.C.Lerpers, _LerpersCountGeomToFrag, 1.0f / _CurrentTriangle.C.Position.w);

		Math::Vec3f _ScreenA = Math::Vec3f(_CurrentTriangle.A.Position) / _CurrentTriangle.A.Position.w;
		Math::Vec3f _ScreenB = Math::Vec3f(_CurrentTriangle.B.Position) / _CurrentTriangle.B.Position.w;
		Math::Vec3f _ScreenC = Math::Vec3f(_CurrentTriangle.C.Position) / _CurrentTriangle.C.Position.w;

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

				if (PointInside(_ScreenP, Math::Vec2f(_ScreenA), Math::Vec2f(_ScreenB), Math::Vec2f(_ScreenC)))
				{
					float _T1 = GetT1(Math::Vec2f(_ScreenA), Math::Vec2f(_ScreenB), Math::Vec2f(_ScreenC), _ScreenP);
					float _T2 = GetT2(Math::Vec2f(_ScreenA), Math::Vec2f(_ScreenB), Math::Vec2f(_ScreenC), _ScreenP, _T1);

					float _PerspectiveCorrection = Math::Mix(Math::Mix(1.0f / _CurrentTriangle.A.Position.w, 1.0f / _CurrentTriangle.B.Position.w, _T1), 1.0f / _CurrentTriangle.C.Position.w, _T2);

					LerpAll(_CurrentTriangle.A.Lerpers, _CurrentTriangle.B.Lerpers, _LerpersCountGeomToFrag, _T1, _FragmentLerpers);
					LerpAll(_FragmentLerpers, _CurrentTriangle.C.Lerpers, _LerpersCountGeomToFrag, _T2, _FragmentLerpers);
					MultiplyAll(_FragmentLerpers, _LerpersCountGeomToFrag, 1.0f / _PerspectiveCorrection);

					Math::Vec4f _FragCoord = Math::Vec4f(_ScreenP.x, _ScreenP.y, Math::Mix(Math::Mix(_ScreenA.z, _ScreenB.z, _T1), _ScreenC.z, _T2), _PerspectiveCorrection);
					_FragCoord.z = (_FragCoord.z + 1.0f) / 2.0f;

					_FragmentShader(_X, _Y, _X - ViewPortX, _Y - ViewPortY, _FragmentLerpers, _Uniforms, _FrameBuffer, _FragCoord, _FrontFacing, DepthTestingType, BlendingType);
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

const bool Rasterizer::Context::DepthTest(const float _NewDepth, const float _OldDepth, const uint8_t _DepthTestingType)
{
	switch (_DepthTestingType)
	{
	case _NoDepthTesting:
	{
		break;
	}
	case _LowerDepthTesting:
	{
		if (_NewDepth >= _OldDepth)
		{
			return false;
		}

		break;
	}
	case _HigherDepthTesting:
	{
		if (_NewDepth <= _OldDepth)
		{
			return false;
		}

		break;
	}
	case _LowerOrEqualDepthTesting:
	{
		if (_NewDepth > _OldDepth)
		{
			return false;
		}

		break;
	}
	case _HigherOrEqualDepthTesting:
	{
		if (_NewDepth < _OldDepth)
		{
			return false;
		}

		break;
	}
	default:
	{
		return false;
	}
	}

	return true;
}

const Math::Vec3f Rasterizer::Context::Blend(const Math::Vec3f& _OldColor, const Math::Vec4f& _Color, const uint8_t _BlendingType, const bool _Clamp)
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
	}

	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}
