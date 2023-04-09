#include "..\Headers\Main.hpp"



Rasterizer::Context::Context() : ViewPortX(0), ViewPortY(0), ViewPortWidth(0), ViewPortHeight(0), CullingType(_ClockWiseCulling), DepthTestingType(_LowerDepthTesting), BlendingType(_AlphaBlending)
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
	_Other.DepthTestingType = _LowerDepthTesting;
	_Other.BlendingType = _AlphaBlending;
}

Rasterizer::Context::~Context()
{

}

void Rasterizer::Context::RenderMesh(const VertexBuffer& _VBO, const IndexBuffer& _IBO, const size_t _IBOBegin, const size_t _IBOEnd, const void* _Uniforms, const size_t _LerpersCount, const VertexShaderFnc _VertexShader) const
{
	struct VertexShaderOutput
	{
		Math::Vec4f Position = Math::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		float* Lerpers = nullptr;
	};

	std::vector<VertexShaderOutput> _VertexShaderOutputs;

	for (size_t _IndexVBO = 0; _IndexVBO < _VBO.GetSize(); _IndexVBO++)
	{
		VertexShaderOutput _Output;

		_Output.Lerpers = new float[_LerpersCount];

		if (!_Output.Lerpers)
		{
			DEBUG_BREAK();

			for (size_t _Index = 0; _Index < _VertexShaderOutputs.size(); _Index++)
			{
				delete[] _VertexShaderOutputs[_Index].Lerpers;
			}

			return;
		}

		_Output.Position = _VertexShader(_VBO[_IndexVBO], _Uniforms, _Output.Lerpers);

		_VertexShaderOutputs.push_back(_Output);
	}

	for (size_t _Index = 0; _Index < _VertexShaderOutputs.size(); _Index++)
	{
		delete[] _VertexShaderOutputs[_Index].Lerpers;
	}

	_VertexShaderOutputs.clear();
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
	_Other.DepthTestingType = _LowerDepthTesting;
	_Other.BlendingType = _AlphaBlending;
}
