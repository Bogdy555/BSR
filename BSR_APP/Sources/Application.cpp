#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::Application::Application() :
	BSR::RunTime::Application(),
	MainWindow(), MainWindowData(), PlacementMutex(), RectMutex(), FullScreenMutex(), CloseMutex(), MinSizeMutex(), InputMutex(), ImageMutex(),
	SceneAssets()
{

}

BSR_APP::RunTime::Application::~Application()
{

}

BSR::Window& BSR_APP::RunTime::Application::GetMainWindow()
{
	return MainWindow;
}

const BSR::Window& BSR_APP::RunTime::Application::GetMainWindow() const
{
	return MainWindow;
}

BSR_APP::WindowData& BSR_APP::RunTime::Application::GetMainWindowData()
{
	return MainWindowData;
}

const BSR_APP::WindowData& BSR_APP::RunTime::Application::GetMainWindowData() const
{
	return MainWindowData;
}

BSR::AssetManager& BSR_APP::RunTime::Application::GetSceneAssets()
{
	return SceneAssets;
}

const BSR::AssetManager& BSR_APP::RunTime::Application::GetSceneAssets() const
{
	return SceneAssets;
}

void BSR_APP::RunTime::Application::UpdateFullScreen()
{
	MainWindowData.FullScreenMutex->lock();

	MainWindowData.FullScreen = !MainWindowData.FullScreen;

	if (MainWindowData.FullScreen)
	{
		MainWindowData.FullScreenMutex->unlock();

		MainWindowData.PlacementMutex->lock();

		GetWindowPlacement(MainWindow, &MainWindowData.Placement);

		MainWindowData.PlacementMutex->unlock();

		MainWindowData.RectMutex->lock();

		GetWindowRect(MainWindow, &MainWindowData.Rect);

		MainWindowData.RectMutex->unlock();

		HMONITOR _hMonitor = MonitorFromWindow(MainWindow, MONITOR_DEFAULTTOPRIMARY);

		MONITORINFOEX _MonitorInfo = { 0 };

		_MonitorInfo.cbSize = sizeof(MONITORINFOEX);

		GetMonitorInfo(_hMonitor, &_MonitorInfo);

		SetWindowLongPtr(MainWindow, GWL_STYLE, WS_POPUP);

		SetWindowPos(MainWindow, HWND_TOP, _MonitorInfo.rcMonitor.left, _MonitorInfo.rcMonitor.top, _MonitorInfo.rcMonitor.right - _MonitorInfo.rcMonitor.left, _MonitorInfo.rcMonitor.bottom - _MonitorInfo.rcMonitor.top, SWP_ASYNCWINDOWPOS);

		MainWindow.Show(SW_SHOW);
	}
	else
	{
		MainWindowData.FullScreenMutex->unlock();

		SetWindowLongPtr(MainWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		MainWindowData.PlacementMutex->lock();

		if (MainWindowData.Placement.showCmd != SW_NORMAL)
		{
			SetWindowPos(MainWindow, HWND_TOP, MainWindowData.Placement.rcNormalPosition.left, MainWindowData.Placement.rcNormalPosition.top, MainWindowData.Placement.rcNormalPosition.right - MainWindowData.Placement.rcNormalPosition.left, MainWindowData.Placement.rcNormalPosition.bottom - MainWindowData.Placement.rcNormalPosition.top, SWP_ASYNCWINDOWPOS);

			MainWindow.Show(MainWindowData.Placement.showCmd);

			MainWindowData.PlacementMutex->unlock();
		}
		else
		{
			MainWindowData.PlacementMutex->unlock();

			MainWindowData.RectMutex->lock();

			SetWindowPos(MainWindow, HWND_TOP, MainWindowData.Rect.left, MainWindowData.Rect.top, MainWindowData.Rect.right - MainWindowData.Rect.left, MainWindowData.Rect.bottom - MainWindowData.Rect.top, SWP_ASYNCWINDOWPOS);

			MainWindow.Show(SW_SHOW);

			MainWindowData.RectMutex->unlock();
		}
	}
}

BSR_APP::RunTime::Application* BSR_APP::RunTime::Application::GetInstance()
{
	static std::unique_ptr<Application> _ApplicationObj(new Application);

	return _ApplicationObj.get();
}

bool BSR_APP::RunTime::Application::InitInstance()
{
	GetSharedInstanceMutex().Lock();

	uint64_t _InstanceCount = *((const uint64_t*)(GetSharedInstanceMemory().GetData()));

	GetSharedInstanceMutex().Unlock();

	if (_InstanceCount != 1)
	{
		HWND _hWnd = FindWindow(L"BSR_APP_MainWindow", L"BSR_APP");

		if (_hWnd)
		{
			SetForegroundWindow(_hWnd);
		}

		return false;
	}

	return true;
}

bool BSR_APP::RunTime::Application::InitMainWindow()
{
	WNDCLASSEX _WndClass = { 0 };

	_WndClass.cbSize = sizeof(WNDCLASSEX);
	_WndClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	_WndClass.lpfnWndProc = WindowProcedure;
	_WndClass.cbClsExtra = 0;
	_WndClass.cbWndExtra = 0;
	_WndClass.hInstance = GetInstanceHandle();
	_WndClass.hIcon = LoadIcon(GetInstanceHandle(), MAKEINTRESOURCE(BSR_APP_IDI_MAIN_ICON));
	_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClass.hbrBackground = NULL;
	_WndClass.lpszMenuName = nullptr;
	_WndClass.lpszClassName = L"BSR_APP_MainWindow";
	_WndClass.hIconSm = LoadIcon(GetInstanceHandle(), MAKEINTRESOURCE(BSR_APP_IDI_MAIN_ICON));

	if (!RegisterClassEx(&_WndClass))
	{
		return false;
	}

	MainWindowData.PlacementMutex = &PlacementMutex;
	MainWindowData.RectMutex = &RectMutex;
	MainWindowData.FullScreenMutex = &FullScreenMutex;
	MainWindowData.CloseMutex = &CloseMutex;
	MainWindowData.MinSizeMutex = &MinSizeMutex;
	MainWindowData.InputMutex = &InputMutex;
	MainWindowData.ImageMutex = &ImageMutex;

	MainWindowData.Image.Width = 160;
	MainWindowData.Image.Height = 90;
	MainWindowData.Image.Data = new uint8_t[MainWindowData.Image.Width * MainWindowData.Image.Height * 4];

	if (!MainWindowData.Image.Data)
	{
		MainWindowData = WindowData();
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	MainWindowData.Depth = new float[MainWindowData.Image.Width * MainWindowData.Image.Height];

	if (!MainWindowData.Depth)
	{
		delete[] MainWindowData.Image.Data;
		MainWindowData = WindowData();
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	for (size_t _Index = 0; _Index < MainWindowData.Image.Width * MainWindowData.Image.Height; _Index++)
	{
		MainWindowData.Image.Data[_Index * 3 + 0] = 0;
		MainWindowData.Image.Data[_Index * 3 + 1] = 0;
		MainWindowData.Image.Data[_Index * 3 + 2] = 0;
	}

	BSR::WindowCreationDescriptor _WindowCreationDescriptor;

	_WindowCreationDescriptor.dwExStyle = NULL;
	_WindowCreationDescriptor.lpClassName = L"BSR_APP_MainWindow";
	_WindowCreationDescriptor.lpWindowName = L"BSR_APP";
	_WindowCreationDescriptor.dwStyle = WS_OVERLAPPEDWINDOW;
	_WindowCreationDescriptor.X = CW_USEDEFAULT;
	_WindowCreationDescriptor.Y = CW_USEDEFAULT;
	_WindowCreationDescriptor.nWidth = CW_USEDEFAULT;
	_WindowCreationDescriptor.nHeight = CW_USEDEFAULT;
	_WindowCreationDescriptor.HandleParent = HWND_DESKTOP;
	_WindowCreationDescriptor.hMenu = NULL;
	_WindowCreationDescriptor.hInstance = GetInstanceHandle();
	_WindowCreationDescriptor.lpParam = nullptr;
	_WindowCreationDescriptor.hAccel = NULL;
	_WindowCreationDescriptor.ShowCmd = SW_HIDE;
	_WindowCreationDescriptor.ThreadInitFnc = InitWindowThread;
	_WindowCreationDescriptor.WndInitFunc = InitWindow;
	_WindowCreationDescriptor.UserData = &MainWindowData;

	if (!MainWindow.Create(&_WindowCreationDescriptor))
	{
		delete[] MainWindowData.Depth;
		delete[] MainWindowData.Image.Data;
		MainWindowData = WindowData();
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	SetSync(MainWindow.GetRefreshRate());

	return true;
}

void BSR_APP::RunTime::Application::CleanUpMainWindow()
{
	if (!MainWindow.CheckOn())
	{
		return;
	}

	MainWindow.Destroy();
	delete[] MainWindowData.Depth;
	delete[] MainWindowData.Image.Data;
	MainWindowData = WindowData();
	UnregisterClass(L"BSR_APP_MainWindow", GetInstanceHandle());
}

bool BSR_APP::RunTime::Application::LoadModel(const wchar_t* _Path, const wchar_t* _AssetName)
{
	BSR::Renderer::Model* _Model = new BSR::Renderer::Model;

	if (!_Model)
	{
		return false;
	}

	if (!_Model->Load(_Path))
	{
		delete _Model;
		return false;
	}

	if (!SceneAssets.AddAsset(_Model, _AssetName))
	{
		delete _Model;
		return false;
	}

	return true;
}

void BSR_APP::RunTime::Application::CleanUpModel(const wchar_t* _AssetName)
{
	delete (BSR::Renderer::Model*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::LoadTexture_R(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType)
{
	BSR::Image::Image _Image;

	_Image.Data = BSR::Image::LoadBmp(_Path, _Image.Width, _Image.Height);

	if (!_Image.Data)
	{
		return false;
	}

	BSR::Rasterizer::Texture_R* _Texture = new BSR::Rasterizer::Texture_R;

	if (!_Texture)
	{
		delete[] _Image.Data;
		return false;
	}

	_Texture->SetLerpType(_LerpType);
	_Texture->SetWrapType(_WrapType);

	if (!_Texture->AddMip(_Image))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	delete[] _Image.Data;

	return true;
}

void BSR_APP::RunTime::Application::CleanUpTexture_R(const wchar_t* _AssetName)
{
	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_R*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (BSR::Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::LoadTexture_RG(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType)
{
	BSR::Image::Image _Image;

	_Image.Data = BSR::Image::LoadBmp(_Path, _Image.Width, _Image.Height);

	if (!_Image.Data)
	{
		return false;
	}

	BSR::Rasterizer::Texture_RG* _Texture = new BSR::Rasterizer::Texture_RG;

	if (!_Texture)
	{
		delete[] _Image.Data;
		return false;
	}

	_Texture->SetLerpType(_LerpType);
	_Texture->SetWrapType(_WrapType);

	if (!_Texture->AddMip(_Image))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	delete[] _Image.Data;

	return true;
}

void BSR_APP::RunTime::Application::CleanUpTexture_RG(const wchar_t* _AssetName)
{
	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_RG*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (BSR::Rasterizer::Texture_RG*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::LoadTexture_RGB(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType, const bool _GammaCorrect)
{
	BSR::Image::Image _Image;

	_Image.Data = BSR::Image::LoadBmp(_Path, _Image.Width, _Image.Height);

	if (!_Image.Data)
	{
		return false;
	}

	if (_GammaCorrect)
	{
		for (size_t _Y = 0; _Y < _Image.Height; _Y++)
		{
			for (size_t _X = 0; _X < _Image.Width; _X++)
			{
				BSR::Math::Vec3f _Color((float)(_Image.Data[(_X + _Y * _Image.Width) * 4 + 0]) / 255.0f, (float)(_Image.Data[(_X + _Y * _Image.Width) * 4 + 1]) / 255.0f, (float)(_Image.Data[(_X + _Y * _Image.Width) * 4 + 2]) / 255.0f);

				_Color = BSR::Math::Vec3f::Pow(_Color, BSR::Math::Vec3f(2.2f, 2.2f, 2.2f));

				_Image.Data[(_X + _Y * _Image.Width) * 4 + 0] = (uint8_t)(_Color.x * 255.0f);
				_Image.Data[(_X + _Y * _Image.Width) * 4 + 1] = (uint8_t)(_Color.y * 255.0f);
				_Image.Data[(_X + _Y * _Image.Width) * 4 + 2] = (uint8_t)(_Color.z * 255.0f);
			}
		}
	}

	BSR::Rasterizer::Texture_RGB* _Texture = new BSR::Rasterizer::Texture_RGB;

	if (!_Texture)
	{
		delete[] _Image.Data;
		return false;
	}

	_Texture->SetLerpType(_LerpType);
	_Texture->SetWrapType(_WrapType);

	if (!_Texture->AddMip(_Image))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	delete[] _Image.Data;

	return true;
}

void BSR_APP::RunTime::Application::CleanUpTexture_RGB(const wchar_t* _AssetName)
{
	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_RGB*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (BSR::Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::LoadTexture_Float_RGB(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType)
{
	BSR::Image::ImageFloat _Image;

	_Image.Data = BSR::Image::LoadHdr(_Path, _Image.Width, _Image.Height);

	if (!_Image.Data)
	{
		return false;
	}

	BSR::Rasterizer::Texture_Float_RGB* _Texture = new BSR::Rasterizer::Texture_Float_RGB;

	if (!_Texture)
	{
		delete[] _Image.Data;
		return false;
	}

	_Texture->SetLerpType(_LerpType);
	_Texture->SetWrapType(_WrapType);

	if (!_Texture->AddMip(_Image))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		return false;
	}

	delete[] _Image.Data;

	return true;
}

void BSR_APP::RunTime::Application::CleanUpTexture_Float_RGB(const wchar_t* _AssetName)
{
	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_Float_RGB*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (BSR::Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::GenerateWhiteTextures()
{
	{
		uint32_t _White = 0xFFFFFFFF;

		BSR::Image::Image _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = (uint8_t*)(&_White);

		BSR::Rasterizer::Texture_R* _Texture = new BSR::Rasterizer::Texture_R;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_R"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		uint32_t _White = 0xFFFFFFFF;

		BSR::Image::Image _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = (uint8_t*)(&_White);

		BSR::Rasterizer::Texture_RG* _Texture = new BSR::Rasterizer::Texture_RG;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_RG"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		uint32_t _White = 0xFFFFFFFF;

		BSR::Image::Image _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = (uint8_t*)(&_White);

		BSR::Rasterizer::Texture_RGB* _Texture = new BSR::Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_RGB"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		uint32_t _White = 0xFFFFFFFF;

		BSR::Image::Image _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = (uint8_t*)(&_White);

		BSR::Rasterizer::Texture_RGBA* _Texture = new BSR::Rasterizer::Texture_RGBA;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_RGBA"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		BSR::Math::Vec4f _White(1.0f, 1.0f, 1.0f, 1.0f);

		BSR::Image::ImageFloat _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = _White.Data();

		BSR::Rasterizer::Texture_Float_R* _Texture = new BSR::Rasterizer::Texture_Float_R;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_Float_R"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		BSR::Math::Vec4f _White(1.0f, 1.0f, 1.0f, 1.0f);

		BSR::Image::ImageFloat _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = _White.Data();

		BSR::Rasterizer::Texture_Float_RG* _Texture = new BSR::Rasterizer::Texture_Float_RG;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_Float_RG"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		BSR::Math::Vec4f _White(1.0f, 1.0f, 1.0f, 1.0f);

		BSR::Image::ImageFloat _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = _White.Data();

		BSR::Rasterizer::Texture_Float_RGB* _Texture = new BSR::Rasterizer::Texture_Float_RGB;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_Float_RGB"))
		{
			delete _Texture;
			return false;
		}
	}

	{
		BSR::Math::Vec4f _White(1.0f, 1.0f, 1.0f, 1.0f);

		BSR::Image::ImageFloat _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = _White.Data();

		BSR::Rasterizer::Texture_Float_RGBA* _Texture = new BSR::Rasterizer::Texture_Float_RGBA;

		if (!_Texture)
		{
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_Float_RGBA"))
		{
			delete _Texture;
			return false;
		}
	}

	return true;
}

void BSR_APP::RunTime::Application::CleanUpWhiteTextures()
{
	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_R*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_R"))) || SceneAssets.GetAssetData(L"White Texture_R") == nullptr);
	delete (BSR::Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"White Texture_R"));
	SceneAssets.RemoveAsset(L"White Texture_R");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_RG*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_RG"))) || SceneAssets.GetAssetData(L"White Texture_RG") == nullptr);
	delete (BSR::Rasterizer::Texture_RG*)(SceneAssets.GetAssetData(L"White Texture_RG"));
	SceneAssets.RemoveAsset(L"White Texture_RG");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_RGB*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_RGB"))) || SceneAssets.GetAssetData(L"White Texture_RGB") == nullptr);
	delete (BSR::Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"White Texture_RGB"));
	SceneAssets.RemoveAsset(L"White Texture_RGB");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_RGBA*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_RGBA"))) || SceneAssets.GetAssetData(L"White Texture_RGBA") == nullptr);
	delete (BSR::Rasterizer::Texture_RGBA*)(SceneAssets.GetAssetData(L"White Texture_RGBA"));
	SceneAssets.RemoveAsset(L"White Texture_RGBA");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_Float_R*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_Float_R"))) || SceneAssets.GetAssetData(L"White Texture_Float_R") == nullptr);
	delete (BSR::Rasterizer::Texture_Float_R*)(SceneAssets.GetAssetData(L"White Texture_Float_R"));
	SceneAssets.RemoveAsset(L"White Texture_Float_R");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_Float_RG*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_Float_RG"))) || SceneAssets.GetAssetData(L"White Texture_Float_RG") == nullptr);
	delete (BSR::Rasterizer::Texture_Float_RG*)(SceneAssets.GetAssetData(L"White Texture_Float_RG"));
	SceneAssets.RemoveAsset(L"White Texture_Float_RG");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_Float_RGB*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_Float_RGB"))) || SceneAssets.GetAssetData(L"White Texture_Float_RGB") == nullptr);
	delete (BSR::Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"White Texture_Float_RGB"));
	SceneAssets.RemoveAsset(L"White Texture_Float_RGB");

	BSR_ASSERT(dynamic_cast<BSR::Rasterizer::Texture_Float_RGBA*>((BSR::Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_Float_RGBA"))) || SceneAssets.GetAssetData(L"White Texture_Float_RGBA") == nullptr);
	delete (BSR::Rasterizer::Texture_Float_RGBA*)(SceneAssets.GetAssetData(L"White Texture_Float_RGBA"));
	SceneAssets.RemoveAsset(L"White Texture_Float_RGBA");
}

bool BSR_APP::RunTime::Application::GenerateMaterial(const wchar_t* _AssetName, const wchar_t* _Albedo, const wchar_t* _Metalness, const wchar_t* _Roughness, const wchar_t* _AmbientOcclusion, const wchar_t* _NormalMap)
{
	BSR::Renderer::Material* _Material = new BSR::Renderer::Material;

	if (!_Material)
	{
		return false;
	}

	_Material->Albedo = (BSR::Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_Albedo));
	_Material->Metalness = (BSR::Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_Metalness));
	_Material->Roughness = (BSR::Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_Roughness));
	_Material->AmbientOcclusion = (BSR::Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_AmbientOcclusion));
	_Material->NormalMap = (BSR::Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_NormalMap));
	_Material->Emission = (BSR::Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"White Texture_RGB"));
	_Material->EmissionMultiplier = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	if (!SceneAssets.AddAsset(_Material, _AssetName))
	{
		delete _Material;
		return false;
	}

	return true;
}

void BSR_APP::RunTime::Application::CleanUpMaterial(const wchar_t* _AssetName)
{
	delete (BSR::Renderer::Material*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

bool BSR_APP::RunTime::Application::InitSceneAssets()
{
	BSR_DEBUG_CALL(BSR::Time::Timer _LoadTime);

	BSR_DEBUG_CALL(_LoadTime.Start());

	if (!LoadModel(L".\\3D Models\\Objects.wfobj", L"Model"))
	{
		return false;
	}

	if (!LoadTexture_Float_RGB(L".\\Environments\\ParkingLot\\Environment.hdr", L"Environment texture", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_Float_RGB(L".\\Environments\\ParkingLot\\Iradiance.hdr", L"Iradiance texture", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Aluminum\\Albedo.bmp", L"Aluminum albedo", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Aluminum\\Metalness.bmp", L"Aluminum metalness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Aluminum\\Normal.bmp", L"Aluminum normal", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, false))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Aluminum\\Roughness.bmp", L"Aluminum roughness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Container\\Albedo.bmp", L"Container albedo", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\AmbientOcclusion.bmp", L"Container ambient occlusion", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\Metalness.bmp", L"Container metalness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Container\\Normal.bmp", L"Container normal", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, false))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\Roughness.bmp", L"Container roughness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Gold\\Albedo.bmp", L"Gold albedo", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Gold\\Metalness.bmp", L"Gold metalness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Gold\\Normal.bmp", L"Gold normal", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, false))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Gold\\Roughness.bmp", L"Gold roughness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Iron\\Albedo.bmp", L"Iron albedo", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Iron\\Metalness.bmp", L"Iron metalness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Iron\\Normal.bmp", L"Iron normal", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, false))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Iron\\Roughness.bmp", L"Iron roughness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Albedo Green.bmp", L"Plastic albedo green", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Albedo Red.bmp", L"Plastic albedo red", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, true))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\AmbientOcclusion.bmp", L"Plastic ambient occlusion", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\Metalness.bmp", L"Plastic metalness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Normal.bmp", L"Plastic normal", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat, false))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\Roughness.bmp", L"Plastic roughness", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapRepeat))
	{
		return false;
	}

	if (!GenerateWhiteTextures())
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 0", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 1", L"Gold albedo", L"Gold metalness", L"Gold roughness", L"White Texture_R", L"Gold normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 2", L"Aluminum albedo", L"Aluminum metalness", L"Aluminum roughness", L"White Texture_R", L"Aluminum normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 3", L"Plastic albedo green", L"Plastic metalness", L"Plastic roughness", L"Plastic ambient occlusion", L"Plastic normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 4", L"Iron albedo", L"Iron metalness", L"Iron roughness", L"White Texture_R", L"Iron normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 5", L"Plastic albedo red", L"Plastic metalness", L"Plastic roughness", L"Plastic ambient occlusion", L"Plastic normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 6", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!GenerateMaterial(L"Material 7", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!LoadTexture_RG(L".\\Intern\\BRDFLookUp.bmp", L"BRDF lookup", BSR::Rasterizer::_LerpLinear, BSR::Rasterizer::_WrapClamp))
	{
		return false;
	}

	BSR_DEBUG_CALL(_LoadTime.Stop());

	BSR_LOG(BSR_STRING_TYPE("Load time: "));
	BSR_LOG_LINE(_LoadTime);

	return true;
}

void BSR_APP::RunTime::Application::CleanUpSceneAssets()
{
	CleanUpModel(L"Model");

	CleanUpTexture_Float_RGB(L"Environment texture");
	CleanUpTexture_Float_RGB(L"Iradiance texture");

	CleanUpTexture_RGB(L"Aluminum albedo");
	CleanUpTexture_R(L"Aluminum metalness");
	CleanUpTexture_RGB(L"Aluminum normal");
	CleanUpTexture_R(L"Aluminum roughness");

	CleanUpTexture_RGB(L"Container albedo");
	CleanUpTexture_R(L"Container ambient occlusion");
	CleanUpTexture_R(L"Container metalness");
	CleanUpTexture_RGB(L"Container normal");
	CleanUpTexture_R(L"Container roughness");

	CleanUpTexture_RGB(L"Gold albedo");
	CleanUpTexture_R(L"Gold metalness");
	CleanUpTexture_RGB(L"Gold normal");
	CleanUpTexture_R(L"Gold roughness");

	CleanUpTexture_RGB(L"Iron albedo");
	CleanUpTexture_R(L"Iron metalness");
	CleanUpTexture_RGB(L"Iron normal");
	CleanUpTexture_R(L"Iron roughness");

	CleanUpTexture_RGB(L"Plastic albedo green");
	CleanUpTexture_RGB(L"Plastic albedo red");
	CleanUpTexture_R(L"Plastic ambient occlusion");
	CleanUpTexture_R(L"Plastic metalness");
	CleanUpTexture_RGB(L"Plastic normal");
	CleanUpTexture_R(L"Plastic roughness");

	CleanUpWhiteTextures();

	CleanUpMaterial(L"Material 0");
	CleanUpMaterial(L"Material 1");
	CleanUpMaterial(L"Material 2");
	CleanUpMaterial(L"Material 3");
	CleanUpMaterial(L"Material 4");
	CleanUpMaterial(L"Material 5");
	CleanUpMaterial(L"Material 6");
	CleanUpMaterial(L"Material 7");

	CleanUpTexture_RG(L"BRDF lookup");

	BSR_ASSERT(SceneAssets.GetAssetsCount() == 0);
}

void BSR_APP::RunTime::Application::Setup()
{
	if (GetArgC() != 1)
	{
		Close(BSR::MultiProcessing::_ReturnError);
		return;
	}

	if (!InitInstance())
	{
		Close(BSR::MultiProcessing::_ReturnNoError);
		return;
	}

	if (!InitSceneAssets())
	{
		Close(BSR::MultiProcessing::_ReturnError);
		return;
	}

	if (!InitMainWindow())
	{
		Close(BSR::MultiProcessing::_ReturnError);
		return;
	}

	if (!MainWindow.Show(GetShowCmd()))
	{
		Close(BSR::MultiProcessing::_ReturnError);
		return;
	}

	SetCurrentMenu(_MainMenu);
	TurnOn();
}

void BSR_APP::RunTime::Application::Update()
{
	switch (GetCurrentMenu())
	{
	case _MainMenu:
	{
		MainMenu _Menu;
		_Menu.Run(this);
		break;
	}
	case BSR::RunTime::_NullMenu:
	{
		BSR_DEBUG_BREAK();
		Close(BSR::MultiProcessing::_ReturnError);
		break;
	}
	default:
	{
		BSR_DEBUG_BREAK();
		Close(BSR::MultiProcessing::_ReturnError);
		break;
	}
	}
}

void BSR_APP::RunTime::Application::Stop()
{
	CleanUpMainWindow();
	CleanUpSceneAssets();
}
