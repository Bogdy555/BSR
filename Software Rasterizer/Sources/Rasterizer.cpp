#include "..\Headers\Main.hpp"



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

const bool Rasterizer::Context::RenderMesh(const float* _VBO, const size_t _VBOSize, const size_t _VBOStride, const size_t* _IBO, const size_t _IBOBegin, const size_t _IBOEnd, const void* _Uniforms, const size_t _LerpersCount, const VertexShaderFnc _VertexShader, const GeometryShaderFnc _GeometryShader) const
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

		_Output.Position = _VertexShader(_VBO + _IndexVBO * _VBOStride, _Uniforms, _Output.Lerpers);

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

			if (!_GeometryShader(_VertexShaderOutputs[_IBO[_IndexIBO + 0]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Position, _VertexShaderOutputs[_IBO[_IndexIBO + 0]].Lerpers, _VertexShaderOutputs[_IBO[_IndexIBO + 1]].Lerpers, _VertexShaderOutputs[_IBO[_IndexIBO + 2]].Lerpers, _PositionsOut, _LerpersOut))
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



	switch (CullingType)
	{
	case _NoCulling:
	{
		break;
	}
	case _ClockWiseCulling:
	{
		for (size_t _IndexTriangle = 0; _IndexTriangle < _GeometryShaderOutputs.size(); _IndexTriangle++)
		{
			Math::Vec3f _A = Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].B.Position) / _GeometryShaderOutputs[_IndexTriangle].B.Position.w - Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].A.Position) / _GeometryShaderOutputs[_IndexTriangle].A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].C.Position) / _GeometryShaderOutputs[_IndexTriangle].C.Position.w - Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].A.Position) / _GeometryShaderOutputs[_IndexTriangle].A.Position.w;

			if (Math::Vec3f::Cross(_A, _B).z <= 0.0f)
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
			Math::Vec3f _A = Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].B.Position) / _GeometryShaderOutputs[_IndexTriangle].B.Position.w - Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].A.Position) / _GeometryShaderOutputs[_IndexTriangle].A.Position.w;
			Math::Vec3f _B = Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].C.Position) / _GeometryShaderOutputs[_IndexTriangle].C.Position.w - Math::Vec3f(_GeometryShaderOutputs[_IndexTriangle].A.Position) / _GeometryShaderOutputs[_IndexTriangle].A.Position.w;

			if (Math::Vec3f::Cross(_A, _B).z >= 0.0f)
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
