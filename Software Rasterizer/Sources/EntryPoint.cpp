#include "..\Headers\Main.hpp"



constexpr size_t Width = 1920;
constexpr size_t Height = 1080;



struct FrameBuffer
{
	Image::Image ColorBuffer;
};



struct Uniforms
{
	Math::Mat4f Mvp;
};



const Math::Vec4f VertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	return ((const Uniforms*)(_Uniforms))->Mvp * Math::Vec4f(((const Rasterizer::VertexData*)(_Vertex))->Position, 1.0f);
}



const bool GeometryShader(const Math::Vec4f& _APosition, const Math::Vec4f& _BPosition, const Math::Vec4f& _CPosition, const float* _ALerpers, const float* _BLerpers, const float* _CLerpers, const void* _Uniforms, std::vector<Math::Vec4f>& _PositionsOut, std::vector<float*>& _LerpersOut)
{
	_PositionsOut.push_back(_APosition);
	_PositionsOut.push_back(_BPosition);
	_PositionsOut.push_back(_CPosition);

	_LerpersOut.push_back(nullptr);
	_LerpersOut.push_back(nullptr);
	_LerpersOut.push_back(nullptr);

	return true;
}



void FragmentShader(const size_t _X, const size_t _Y, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const float _FragDepth, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 0] = 50;
	((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 1] = 200;
	((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 2] = 255;
	((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 3] = 255;
}



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	Rasterizer::Model _Model;

	if (!_Model.Load(L".\\Scene.wfobj"))
	{
		return -1;
	}

	LOG_LINE(STRING_TYPE("The model was loaded. Those are the objects in it:\n"));

	for (size_t _IndexMesh = 0; _IndexMesh < _Model.GetSize(); _IndexMesh++)
	{
		LOG_LINE(_Model[_IndexMesh].Name);
	}

	LOG_LINE(STRING_TYPE("\nRendering the scene"));

	FrameBuffer _FrameBuffer;

	_FrameBuffer.ColorBuffer.Width = Width;
	_FrameBuffer.ColorBuffer.Height = Height;
	_FrameBuffer.ColorBuffer.Data = new uint8_t[_FrameBuffer.ColorBuffer.Width * _FrameBuffer.ColorBuffer.Height * 4];

	if (!_FrameBuffer.ColorBuffer.Data)
	{
		return -1;
	}

	for (size_t _Index = 0; _Index < _FrameBuffer.ColorBuffer.Width * _FrameBuffer.ColorBuffer.Height; _Index++)
	{
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 0] = 0;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 1] = 0;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 2] = 0;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 3] = 255;
	}

	Rasterizer::Camera _Camera;

	Rasterizer::Transform _Transform;

	_Transform.Position = Math::Vec3f(0.0f, 0.0f, -3.0f);

	Uniforms _Uniforms;

	_Uniforms.Mvp = _Camera.GetProjectionMatrix((float)(_FrameBuffer.ColorBuffer.Width) / (float)(_FrameBuffer.ColorBuffer.Height)) * _Camera.GetViewMatrix() * _Transform.GetModelMatrix();

	Rasterizer::Context _Context;

	_Context.SetViewPort(0, 0, _FrameBuffer.ColorBuffer.Width, _FrameBuffer.ColorBuffer.Height);

	for (size_t _IndexMesh = 0; _IndexMesh < _Model.GetSize(); _IndexMesh++)
	{
		if (!_Context.RenderMesh(_Model[_IndexMesh].VBO.GetData(), _Model[_IndexMesh].VBO.GetSize(), sizeof(Rasterizer::VertexData), _Model[_IndexMesh].IBO.GetData(), 0, _Model[_IndexMesh].IBO.GetSize() * 3, &_Uniforms, 0, VertexShader, GeometryShader, FragmentShader, &_FrameBuffer))
		{
			delete[] _FrameBuffer.ColorBuffer.Data;
			return -1;
		}
	}

	if (!Image::SaveBmp(L".\\Test.bmp", _FrameBuffer.ColorBuffer.Data, _FrameBuffer.ColorBuffer.Width, _FrameBuffer.ColorBuffer.Height))
	{
		delete[] _FrameBuffer.ColorBuffer.Data;
		return -1;
	}

	delete[] _FrameBuffer.ColorBuffer.Data;

	ShellExecute(NULL, nullptr, L".\\Test.bmp", nullptr, nullptr, SW_SHOW);

	return 0;
}
