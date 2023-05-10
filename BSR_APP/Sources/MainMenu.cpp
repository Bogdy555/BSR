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

	if (!Keys[BSR::_Previous][VK_F11] && Keys[BSR::_Current][VK_F11])
	{
		_ApplicationObj->UpdateFullScreen();
	}

	if (!Keys[BSR::_Previous][VK_F5] && Keys[BSR::_Current][VK_F5])
	{
		RenderAndSave();
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
		BSR_DEBUG_CALL(constexpr size_t _Resize = 10);
		BSR_RELEASE_CALL(constexpr size_t _Resize = 5);

		int32_t _NewWidth = 0, _NewHeight = 0;
		_MainWindow.GetClientSize(_NewWidth, _NewHeight);

		if ((size_t)(_NewWidth) / _Resize != _MainWindowData.Image.Width || (size_t)(_NewHeight) / _Resize != _MainWindowData.Image.Height)
		{
			delete[] _MainWindowData.Depth;
			delete[] _MainWindowData.Image.Data;

			_MainWindowData.Image.Width = _NewWidth / _Resize;
			_MainWindowData.Image.Height = _NewHeight / _Resize;
			_MainWindowData.Image.Data = new uint8_t[_MainWindowData.Image.Width * _MainWindowData.Image.Height * 4];
			_MainWindowData.Depth = new float[_MainWindowData.Image.Width * _MainWindowData.Image.Height];
		}
	}

	Render();

	_MainWindowData.ImageMutex->unlock();

	_MainWindow.UpdateContent();
}

void BSR_APP::RunTime::MainMenu::RenderAndSave()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	const BSR::AssetManager& _SceneAssets = _ApplicationObj->GetSceneAssets();

	const BSR::Renderer::Model& _Model = *(const BSR::Renderer::Model*)(_SceneAssets.GetAssetData(L"Model"));

	BSR::Renderer::FrameBuffer _FrameBuffer;

	_FrameBuffer.Width = 1920;
	_FrameBuffer.Height = 1080;
	_FrameBuffer.Albedo = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Metalness = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Roughness = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.AmbientOcclusion = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.NormalMap = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Emission = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Position = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Depth = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Stencil = new uint64_t[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Result = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];

	BSR::Renderer::Instance _RendererInstance;

	_RendererInstance.StartScene(_FrameBuffer, Camera);

	for (size_t _Index = 0; _Index < _Model.GetSize(); _Index++)
	{
		_RendererInstance.SubmitModel(_Model[_Index], *(const BSR::Renderer::Material*)(_SceneAssets.GetAssetData((std::wstring(L"Material ") + std::to_wstring(_Index)).c_str())), Transform);
	}

	_RendererInstance.FlushScene();

	BSR::Image::Image _Image;

	_Image.Width = _FrameBuffer.Width;
	_Image.Height = _FrameBuffer.Height;
	_Image.Data = new uint8_t[_Image.Width * _Image.Height * 4];

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Albedo.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Metalness.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Roughness.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\AmbientOcclusion.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\NormalMap.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Emission.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Position.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Depth.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Stencil.bmp", _Image.Data, _Image.Width, _Image.Height);

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = 0;
			_Image.Data[(_X + _Y * _Image.Width) * 4 + 3] = 255;
		}
	}

	BSR::Image::SaveBmp(L".\\Result.bmp", _Image.Data, _Image.Width, _Image.Height);

	delete[] _Image.Data;

	delete[] _FrameBuffer.Albedo;
	delete[] _FrameBuffer.Metalness;
	delete[] _FrameBuffer.Roughness;
	delete[] _FrameBuffer.AmbientOcclusion;
	delete[] _FrameBuffer.NormalMap;
	delete[] _FrameBuffer.Emission;
	delete[] _FrameBuffer.Position;
	delete[] _FrameBuffer.Depth;
	delete[] _FrameBuffer.Stencil;
	delete[] _FrameBuffer.Result;

	ShellExecute(_ApplicationObj->GetMainWindow(), nullptr, L".\\Result.bmp", nullptr, nullptr, SW_SHOW);
}

void BSR_APP::RunTime::MainMenu::Render()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	struct FrameBuffer
	{
		size_t Width = 0;
		size_t Height = 0;
		uint8_t* Data = nullptr;
		float* Depth = nullptr;
	};

	FrameBuffer _FrameBuffer;

	_FrameBuffer.Width = _MainWindowData.Image.Width;
	_FrameBuffer.Height = _MainWindowData.Image.Height;
	_FrameBuffer.Data = _MainWindowData.Image.Data;
	_FrameBuffer.Depth = _MainWindowData.Depth;

	struct Uniforms
	{
		BSR::Math::Mat4f Model;
		BSR::Math::Mat3f ModelInversedTransposed;
		BSR::Math::Mat4f View;
		BSR::Math::Mat4f Projection;
		BSR::Math::Mat4f Mvp;
	};

	struct Lerpers
	{
		BSR::Math::Vec3f Normal = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	};

	Uniforms _Uniforms;

	_Uniforms.Model = Transform.GetModelMatrix();
	_Uniforms.ModelInversedTransposed = BSR::Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
	_Uniforms.View = Camera.GetViewMatrix();
	_Uniforms.Projection = Camera.GetProjectionMatrix((float)(_FrameBuffer.Width) / (float)(_FrameBuffer.Height));
	_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;

	BSR::Renderer::Model& _Model = *(BSR::Renderer::Model*)(((Application*)(GetApplicationObj()))->GetSceneAssets().GetAssetData(L"Model"));

	BSR::Rasterizer::Context _Context;

	_Context.SetViewPort(0, 0, _FrameBuffer.Width, _FrameBuffer.Height);

	for (size_t _Y = 0; _Y < _FrameBuffer.Height; _Y++)
	{
		for (size_t _X = 0; _X < _FrameBuffer.Width; _X++)
		{
			_FrameBuffer.Data[(_X + _Y * _FrameBuffer.Width) * 4 + 0] = 0;
			_FrameBuffer.Data[(_X + _Y * _FrameBuffer.Width) * 4 + 1] = 0;
			_FrameBuffer.Data[(_X + _Y * _FrameBuffer.Width) * 4 + 2] = 0;
			_FrameBuffer.Data[(_X + _Y * _FrameBuffer.Width) * 4 + 3] = 0;

			_FrameBuffer.Depth[_X + _Y * _FrameBuffer.Width] = 1.0f;
		}
	}

	for (size_t _MeshIndex = 0; _MeshIndex < _Model.GetSize(); _MeshIndex++)
	{
		_Context.RenderMesh
		(
			_Model[_MeshIndex].VBO.GetData(), _Model[_MeshIndex].VBO.GetSize(), sizeof(BSR::Renderer::VertexData),
			_Model[_MeshIndex].IBO.GetData(), 0, _Model[_MeshIndex].IBO.GetSize() * 3,
			&_Uniforms,
			sizeof(Lerpers) / sizeof(float),
			sizeof(Lerpers) / sizeof(float),
			[](const void* _Vertex, const void* _Uniforms, float* _OutLerpers) -> const BSR::Math::Vec4f
			{
				const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
				const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
				Lerpers& _TrueLerpers = *(Lerpers*)(_OutLerpers);

				_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;

				return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
			},
			nullptr,
			[](const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
			{
				const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
				FrameBuffer& _TrueFrameBuffer = *(FrameBuffer*)(_FrameBuffer);
				const Lerpers& _TrueLerpers = *(const Lerpers*)(_Lerpers);

				if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width], _DepthTestingType))
				{
					return;
				}

				_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] = _FragCoord.z;

				float _Dot = BSR::Math::Clamp(BSR::Math::Vec3f::Dot(BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), _TrueLerpers.Normal.Normalized()), 0.1f, 1.0f);

				_TrueFrameBuffer.Data[(_X + (_TrueFrameBuffer.Height - 1 - _Y) * _TrueFrameBuffer.Width) * 4 + 0] = (uint8_t)(_Dot * 255.0f);
				_TrueFrameBuffer.Data[(_X + (_TrueFrameBuffer.Height - 1 - _Y) * _TrueFrameBuffer.Width) * 4 + 1] = (uint8_t)(_Dot * 255.0f);
				_TrueFrameBuffer.Data[(_X + (_TrueFrameBuffer.Height - 1 - _Y) * _TrueFrameBuffer.Width) * 4 + 2] = (uint8_t)(_Dot * 255.0f);
				_TrueFrameBuffer.Data[(_X + (_TrueFrameBuffer.Height - 1 - _Y) * _TrueFrameBuffer.Width) * 4 + 3] = 255;
			},
			&_FrameBuffer
		);
	}
}
