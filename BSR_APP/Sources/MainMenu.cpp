#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::MainMenu::MainMenu() : BSR::RunTime::Menu(), Keys(), Camera(), Transform(), CameraMoveSpeed(3.0f), CameraRotateSpeed(180.0f * BSR::Math::fDegreesToRadians)
{

}

BSR_APP::RunTime::MainMenu::~MainMenu()
{

}

const uint64_t BSR_APP::RunTime::MainMenu::GetType() const
{
	return _MainMenu;
}

void BSR_APP::RunTime::MainMenu::Setup()
{
	Transform.Position = BSR::Math::Vec3f(0.0f, 0.0f, -3.0f);

	DeleteInput();

	TurnOn();
}

void BSR_APP::RunTime::MainMenu::Update()
{
	BSR_LOG(BSR_STRING_TYPE("FPS: "));
	BSR_LOG_LINE(1.0f / GetFrameTime(BSR::_Previous));

	Input();
	Engine();
	FrameBuild();
}

void BSR_APP::RunTime::MainMenu::Stop()
{

}

void BSR_APP::RunTime::MainMenu::Input()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	_MainWindowData.InputMutex->lock();

	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = Keys[BSR::_Current][_Index];
		Keys[BSR::_Current][_Index] = _MainWindowData.Keys[_Index];
	}

	_MainWindowData.InputMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::DeleteInput()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = false;
		Keys[BSR::_Current][_Index] = false;
	}
}

void BSR_APP::RunTime::MainMenu::Engine()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	if (Keys[BSR::_Current][VK_ESCAPE])
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	{
		BSR::Math::Vec3f _MoveVec;

		_MoveVec.x -= Keys[BSR::_Current]['A'];
		_MoveVec.y -= Keys[BSR::_Current][VK_SHIFT];
		_MoveVec.z -= Keys[BSR::_Current]['W'];
		_MoveVec.x += Keys[BSR::_Current]['D'];
		_MoveVec.y += Keys[BSR::_Current][VK_SPACE];
		_MoveVec.z += Keys[BSR::_Current]['S'];

		if (_MoveVec.Magnitude())
		{
			_MoveVec.Normalize();
		}

		_MoveVec *= CameraMoveSpeed * GetTimeStep();

		Camera.Position += BSR::Math::Vec3f(BSR::Math::Mat4f::GetRotation(Camera.AngleFlat, BSR::Math::Vec3f(0.0f, 1.0f, 0.0f)) * BSR::Math::Vec4f(_MoveVec, 1.0f));
	}

	Camera.AngleFlat -= CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_RIGHT];
	Camera.AngleVertical -= CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_DOWN];
	Camera.AngleTilt -= CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_F8];
	Camera.AngleFlat += CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_LEFT];
	Camera.AngleVertical += CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_UP];
	Camera.AngleTilt += CameraRotateSpeed * GetTimeStep() * Keys[BSR::_Current][VK_F7];

	while (Camera.AngleFlat < -180.0f * BSR::Math::fDegreesToRadians)
	{
		Camera.AngleFlat += 360.0f * BSR::Math::fDegreesToRadians;
	}
	while (Camera.AngleFlat >= 180.0f * BSR::Math::fDegreesToRadians)
	{
		Camera.AngleFlat -= 360.0f * BSR::Math::fDegreesToRadians;
	}
	Camera.AngleTilt = BSR::Math::Clamp(Camera.AngleTilt, -180.0f * BSR::Math::fDegreesToRadians, 180.0f * BSR::Math::fDegreesToRadians);
	Camera.AngleVertical = BSR::Math::Clamp(Camera.AngleVertical, -90.0f * BSR::Math::fDegreesToRadians, 90.0f * BSR::Math::fDegreesToRadians);

	if (Camera.Perspective)
	{
		if (Keys[BSR::_Current][VK_F3])
		{
			Camera.FieldOfView -= CameraRotateSpeed * GetTimeStep();
		}
		if (Keys[BSR::_Current][VK_F4])
		{
			Camera.FieldOfView += CameraRotateSpeed * GetTimeStep();
		}

		Camera.FieldOfView = BSR::Math::Clamp(Camera.FieldOfView, 10.0f * BSR::Math::fDegreesToRadians, 160.0f * BSR::Math::fDegreesToRadians);
	}
	else
	{
		if (Keys[BSR::_Current][VK_F3])
		{
			Camera.FieldOfView -= 2.0f * GetTimeStep();
		}
		if (Keys[BSR::_Current][VK_F4])
		{
			Camera.FieldOfView += 2.0f * GetTimeStep();
		}

		if (Camera.FieldOfView < 0.1f)
		{
			Camera.FieldOfView = 0.1f;
		}
	}

	if (!Keys[BSR::_Previous][VK_F6] && Keys[BSR::_Current][VK_F6])
	{
		if (Camera.Perspective)
		{
			Camera.Perspective = false;
			Camera.FieldOfView = 4.0f;
		}
		else
		{
			Camera.Perspective = true;
			Camera.FieldOfView = 90.0f * BSR::Math::fDegreesToRadians;
		}
	}

	_ApplicationObj->SetSync(_MainWindow.GetRefreshRate());

	_MainWindowData.CloseMutex->lock();

	if (_MainWindowData.Close)
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	_MainWindowData.CloseMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::FrameBuild()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	_MainWindowData.ImageMutex->lock();

	{
		delete[] _MainWindowData.Image.Data;

		int32_t _NewWidth = 0, _NewHeight = 0;
		_MainWindow.GetClientSize(_NewWidth, _NewHeight);

		_MainWindowData.Image.Width = _NewWidth / 10;
		_MainWindowData.Image.Height = _NewHeight / 10;
		_MainWindowData.Image.Data = new uint8_t[_MainWindowData.Image.Width * _MainWindowData.Image.Height * 3];
	}

	RenderPhong(_MainWindowData.Image);

	_MainWindowData.ImageMutex->unlock();

	_MainWindow.UpdateContent();
}

void BSR_APP::RunTime::MainMenu::RenderPhong(BSR::Image::Image& _Image)
{
	struct PhongUniforms
	{
		BSR::Math::Mat4f Model;
		BSR::Math::Mat3f ModelInversedTransposed;
		BSR::Math::Mat4f View;
		BSR::Math::Mat4f Projection;
		BSR::Math::Mat4f Mvp;
	};

	typedef BSR::Rasterizer::VertexData PhongLerpers;

	PhongUniforms _Uniforms;

	_Uniforms.Model = Transform.GetModelMatrix();
	_Uniforms.ModelInversedTransposed = BSR::Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
	_Uniforms.View = Camera.GetViewMatrix();
	_Uniforms.Projection = Camera.GetProjectionMatrix((float)(_Image.Width) / (float)(_Image.Height));
	_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;

	BSR::Rasterizer::Model& _Model = *(BSR::Rasterizer::Model*)(((Application*)(GetApplicationObj()))->GetSceneAssets().GetAssetData(L"Model"));

	BSR::Rasterizer::Context _Context;

	_Context.SetViewPort(0, 0, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 3 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 3 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 3 + 2] = 0;
		}
	}

	for (size_t _MeshIndex = 0; _MeshIndex < _Model.GetSize(); _MeshIndex++)
	{
		_Context.RenderMesh
		(
			_Model[_MeshIndex].VBO.GetData(), _Model[_MeshIndex].VBO.GetSize(), sizeof(BSR::Rasterizer::VertexData),
			_Model[_MeshIndex].IBO.GetData(), 0, _Model[_MeshIndex].IBO.GetSize() * 3,
			&_Uniforms,
			sizeof(PhongLerpers) / sizeof(float),
			sizeof(PhongLerpers) / sizeof(float),
			[](const void* _Vertex, const void* _Uniforms, float* _OutLerpers) -> const BSR::Math::Vec4f
			{
				const BSR::Rasterizer::VertexData& _TrueVertex = *(const BSR::Rasterizer::VertexData*)(_Vertex);
				const PhongUniforms& _TrueUniforms = *(const PhongUniforms*)(_Uniforms);
				PhongLerpers& _TrueLerpers = *(PhongLerpers*)(_OutLerpers);

				_TrueLerpers.Position = BSR::Math::Vec3f(_TrueUniforms.Model * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f));
				_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;
				_TrueLerpers.Tangent = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Tangent;
				_TrueLerpers.TextureCoords = _TrueVertex.TextureCoords;

				return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
			},
			nullptr,
			[](const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
			{
				const PhongLerpers& _TrueLerpers = *(const PhongLerpers*)(_Lerpers);
				const PhongUniforms& _TrueUniforms = *(const PhongUniforms*)(_Uniforms);
				BSR::Image::Image& _TrueFrameBuffer = *(BSR::Image::Image*)(_FrameBuffer);

				_TrueFrameBuffer.Data[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 0] = 255;
				_TrueFrameBuffer.Data[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 1] = 255;
				_TrueFrameBuffer.Data[(_X + _Y * _TrueFrameBuffer.Width) * 3 + 2] = 255;
			},
			&_Image
		);
	}
}
