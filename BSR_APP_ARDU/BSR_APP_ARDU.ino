#include <BSR.hpp>

struct FrameBuffer
{
	size_t Width = 0;
	size_t Height = 0;
	uint8_t* Color = nullptr;
};

struct VertexData
{
	BSR::Math::Vec2f Position = BSR::Math::Vec2f(0.0f, 0.0f);
	BSR::Math::Vec3f Color = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
};

struct Uniforms
{
	BSR::Math::Mat4f Mvp;
};

struct Lerpers
{
	BSR::Math::Vec3f Color = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
};

const BSR::Math::Vec4f VertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const VertexData& _TrueVertex = *(const VertexData*)(_Vertex);
	const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
	Lerpers& _TrueLerpers = *(Lerpers*)(_OutLerpers);

	_TrueLerpers.Color = _TrueVertex.Color;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 0.0f, 1.0f);
}

void FragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const Lerpers& _TrueLerpers = *(const Lerpers*)(_Lerpers);
	const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
	FrameBuffer& _TrueFrameBuffer = *(FrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Color[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 0] = (uint8_t)(_TrueLerpers.Color.x * 255.0f);
	_TrueFrameBuffer.Color[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 1] = (uint8_t)(_TrueLerpers.Color.y * 255.0f);
	_TrueFrameBuffer.Color[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 2] = (uint8_t)(_TrueLerpers.Color.z * 255.0f);
}

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	float VBO[] =
	{
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	size_t IBO[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	FrameBuffer _FrameBuffer;

	_FrameBuffer.Width = 10;
	_FrameBuffer.Height = 10;
	_FrameBuffer.Color = new uint8_t[_FrameBuffer.Width * _FrameBuffer.Height * 3];

	for (size_t _Y = 0; _Y < _FrameBuffer.Height; _Y++)
	{
		for (size_t _X = 0; _X < _FrameBuffer.Width; _X++)
		{
			_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 0] = 0;
			_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 1] = 0;
			_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 2] = 0;
		}
	}

	Uniforms _Uniforms;

	BSR::Rasterizer::Context _Context;
	_Context.ViewPortX = 0;
	_Context.ViewPortY = 0;
	_Context.ViewPortWidth = _FrameBuffer.Width;
	_Context.ViewPortHeight = _FrameBuffer.Height;
	_Context.CullingType = BSR::Rasterizer::_ClockWiseCulling;
	_Context.DepthTestingType = BSR::Rasterizer::_NoDepthTesting;
	_Context.BlendingType = BSR::Rasterizer::_NoBlending;
	_Context.VBO = VBO;
	_Context.VBOSize = sizeof(VBO) / sizeof(VertexData);
	_Context.VBOStride = sizeof(VertexData);
	_Context.IBO = IBO;
	_Context.IBOBegin = 0;
	_Context.IBOEnd = sizeof(IBO) / sizeof(size_t);
	_Context.Uniforms = &_Uniforms;
	_Context.VertexShader = VertexShader;
	_Context.LerpersCountVertToGeom = sizeof(Lerpers) / sizeof(float);
	_Context.GeometryShader = nullptr;
	_Context.LerpersCountGeomToFrag = sizeof(Lerpers) / sizeof(float);
	_Context.FragmentShader = FragmentShader;
	_Context.FrameBuffer = &_FrameBuffer;

	_Context.DrawCall();

	Serial.println("Frame:");

	for (size_t _Y = 0; _Y < _FrameBuffer.Height; _Y++)
	{
		for (size_t _X = 0; _X < _FrameBuffer.Width; _X++)
		{
			Serial.print((int)(_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 0]));
			Serial.print(' ');
			Serial.print((int)(_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 1]));
			Serial.print(' ');
			Serial.print((int)(_FrameBuffer.Color[(_X + _Y * _FrameBuffer.Width) * 3 + 2]));
			Serial.print(' ');
		}
		Serial.print('\n');
	}

	delete[] _FrameBuffer.Color;
}
