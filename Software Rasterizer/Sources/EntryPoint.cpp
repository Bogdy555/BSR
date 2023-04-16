#include "..\Headers\Main.hpp"



constexpr size_t Width = 1920;
constexpr size_t Height = 1080;



struct FrameBuffer
{
	Image::Image ColorBuffer;
	Image::ImageFloat DepthBuffer;
};



struct Uniforms
{
	Math::Mat4f Mvp;
};



const Math::Vec4f VertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	return ((const Uniforms*)(_Uniforms))->Mvp * Math::Vec4f(((const Rasterizer::VertexData*)(_Vertex))->Position, 1.0f);
}



void FragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	if (_FragCoord.z <= ((FrameBuffer*)(_FrameBuffer))->DepthBuffer.Data[_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width])
	{
		((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 0] = (uint8_t)(_FragCoord.z * 255.0f);
		((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 1] = (uint8_t)(_FragCoord.z * 255.0f);
		((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 2] = (uint8_t)(_FragCoord.z * 255.0f);
		((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Data[(_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width) * 4 + 3] = 255;

		((FrameBuffer*)(_FrameBuffer))->DepthBuffer.Data[_X + _Y * ((FrameBuffer*)(_FrameBuffer))->ColorBuffer.Width] = _FragCoord.z;
	}
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

	_FrameBuffer.DepthBuffer.Width = Width;
	_FrameBuffer.DepthBuffer.Height = Height;
	_FrameBuffer.DepthBuffer.Data = new float[_FrameBuffer.DepthBuffer.Width * _FrameBuffer.DepthBuffer.Height];

	if (!_FrameBuffer.DepthBuffer.Data)
	{
		delete[] _FrameBuffer.ColorBuffer.Data;
		return -1;
	}

	for (size_t _Index = 0; _Index < _FrameBuffer.ColorBuffer.Width * _FrameBuffer.ColorBuffer.Height; _Index++)
	{
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 0] = 255;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 1] = 255;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 2] = 255;
		_FrameBuffer.ColorBuffer.Data[_Index * 4 + 3] = 255;

		_FrameBuffer.DepthBuffer.Data[_Index] = 1.0f;
	}

	Rasterizer::Camera _Camera;

	_Camera.NearPlane = 1.0f;
	_Camera.FarPlane = 5.0f;

	Rasterizer::Transform _Transform;

	_Transform.Position = Math::Vec3f(0.0f, 0.0f, -3.0f);

	Uniforms _Uniforms;

	_Uniforms.Mvp = _Camera.GetProjectionMatrix((float)(_FrameBuffer.ColorBuffer.Width) / (float)(_FrameBuffer.ColorBuffer.Height)) * _Camera.GetViewMatrix() * _Transform.GetModelMatrix();

	Rasterizer::Context _Context;

	_Context.SetViewPort(0, 0, _FrameBuffer.ColorBuffer.Width, _FrameBuffer.ColorBuffer.Height);

	for (size_t _IndexMesh = 0; _IndexMesh < _Model.GetSize(); _IndexMesh++)
	{
		if (!_Context.RenderMesh(_Model[_IndexMesh].VBO.GetData(), _Model[_IndexMesh].VBO.GetSize(), sizeof(Rasterizer::VertexData), _Model[_IndexMesh].IBO.GetData(), 0, _Model[_IndexMesh].IBO.GetSize() * 3, &_Uniforms, 0, VertexShader, nullptr, FragmentShader, &_FrameBuffer))
		{
			delete[] _FrameBuffer.DepthBuffer.Data;
			delete[] _FrameBuffer.ColorBuffer.Data;
			return -1;
		}
	}

	if (!Image::SaveBmp(L".\\Test.bmp", _FrameBuffer.ColorBuffer.Data, _FrameBuffer.ColorBuffer.Width, _FrameBuffer.ColorBuffer.Height))
	{
		delete[] _FrameBuffer.DepthBuffer.Data;
		delete[] _FrameBuffer.ColorBuffer.Data;
		return -1;
	}

	delete[] _FrameBuffer.DepthBuffer.Data;
	delete[] _FrameBuffer.ColorBuffer.Data;

	ShellExecute(NULL, nullptr, L".\\Test.bmp", nullptr, nullptr, SW_SHOW);

	return 0;
}
