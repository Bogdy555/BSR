#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::Application::Application() :
	BSR::RunTime::Application(),
	MainWindow(), MainWindowData(), CloseMutex(), MinSizeMutex(), InputMutex()
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
	_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	_WndClass.lpszMenuName = nullptr;
	_WndClass.lpszClassName = L"BSR_APP_MainWindow";
	_WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&_WndClass))
	{
		return false;
	}

	MainWindowData.CloseMutex = &CloseMutex;
	MainWindowData.MinSizeMutex = &MinSizeMutex;
	MainWindowData.InputMutex = &InputMutex;

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
	MainWindowData = WindowData();
	UnregisterClass(L"BSR_APP_MainWindow", GetInstanceHandle());
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
}
