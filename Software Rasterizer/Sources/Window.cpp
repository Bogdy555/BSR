#include "..\Headers\BSR.hpp"



BSR::Window* BSR::Window::LastWnd = nullptr;



BSR::Window::Window() : hWnd(NULL), WndThread(), UserData(nullptr)
{

}

BSR::Window::Window(Window&& _Other) noexcept : hWnd(_Other.hWnd), WndThread(std::move(_Other.WndThread)), UserData(_Other.UserData)
{
	_Other.hWnd = NULL;
	_Other.UserData = nullptr;
}

BSR::Window::~Window()
{
	BSR_ASSERT_MSG(!CheckOn(), BSR_STRING_TYPE("A window was not destroyed!"));
}

bool BSR::Window::Create(const WindowCreationDescriptor* _Descriptor)
{
	if (hWnd)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not create a window before cleaning the old one!"));
		return false;
	}

	if (!_Descriptor)
	{
		return false;
	}

	bool _Done = false;
	bool _Fail = false;

	WndThread = std::move(std::thread(WndThreadFunc, &_Done, &_Fail, this, _Descriptor));

	while (!_Done)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	if (_Fail)
	{
		WndThread.join();
		return false;
	}

	return true;
}

void BSR::Window::Destroy()
{
	if (!hWnd)
	{
		return;
	}

	PostMessage(hWnd, WM_QUIT, 0, 0);
	WndThread.join();
}

bool BSR::Window::Show(const int _ShowCmd)
{
	if (!_ShowCmd)
	{
		return false;
	}

	return ShowWindowAsync(hWnd, _ShowCmd) != false;
}

bool BSR::Window::UpdateContent()
{
	if (!hWnd)
	{
		return false;
	}

	if (!InvalidateRect(hWnd, nullptr, false))
	{
		return false;
	}

	return UpdateWindow(hWnd);
}

const bool BSR::Window::CheckOn() const
{
	return hWnd != NULL;
}

const HWND BSR::Window::GetHandle() const
{
	return hWnd;
}

void* BSR::Window::GetUserData()
{
	return UserData;
}

const void* BSR::Window::GetUserData() const
{
	return UserData;
}

const bool BSR::Window::GetClientSize(int32_t& _Width, int32_t& _Height) const
{
	if (!hWnd)
	{
		return false;
	}

	RECT _ClientRect = { 0 };

	if (!GetClientRect(hWnd, &_ClientRect))
	{
		return false;
	}

	_Width = _ClientRect.right - _ClientRect.left;
	_Height = _ClientRect.bottom - _ClientRect.top;

	return true;
}

const bool BSR::Window::GetWindowSize(int32_t& _Width, int32_t& _Height) const
{
	if (!hWnd)
	{
		return false;
	}

	RECT _WindowRect = { 0 };

	if (!GetWindowRect(hWnd, &_WindowRect))
	{
		return false;
	}

	_Width = _WindowRect.right - _WindowRect.left;
	_Height = _WindowRect.bottom - _WindowRect.top;

	return true;
}

const size_t BSR::Window::GetRefreshRate() const
{
	if (!hWnd)
	{
		return 0;
	}

	HMONITOR _hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

	if (!_hMonitor)
	{
		return 0;
	}

	MONITORINFOEX _MonitorInfo = { 0 };

	_MonitorInfo.cbSize = sizeof(MONITORINFOEX);

	if (!GetMonitorInfo(_hMonitor, &_MonitorInfo))
	{
		return 0;
	}

	DEVMODE _DevMode = { 0 };

	_DevMode.dmSize = sizeof(DEVMODE);

	if (!EnumDisplaySettingsEx(_MonitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &_DevMode, 0))
	{
		return 0;
	}

	return (size_t)(_DevMode.dmDisplayFrequency);
}

BSR::Window::operator const HWND() const
{
	return hWnd;
}

void BSR::Window::operator= (Window&& _Other) noexcept
{
	BSR_ASSERT_MSG(CheckOn(), BSR_STRING_TYPE("A window was not destroyed before assigning another one to it!"));

	hWnd = _Other.hWnd;
	WndThread = std::move(_Other.WndThread);
	UserData = _Other.UserData;

	_Other.hWnd = NULL;
	_Other.UserData = nullptr;
}

BSR::Window* BSR::Window::GetWindowPtr(const HWND _hWnd)
{
	if (!_hWnd)
	{
		return nullptr;
	}

	Window* _WndPtr = (Window*)(GetWindowLongPtr(_hWnd, GWLP_USERDATA));

	if (!_WndPtr)
	{
		return LastWnd;
	}

	return _WndPtr;
}

void BSR::Window::WndThreadFunc(bool* _Done, bool* _Fail, Window* _Wnd, const WindowCreationDescriptor* _Descriptor)
{
	if (_Descriptor->ThreadInitFnc)
	{
		if (!_Descriptor->ThreadInitFnc(_Descriptor->UserData))
		{
			*_Fail = true;
			*_Done = true;

			return;
		}
	}

	_Wnd->UserData = _Descriptor->UserData;

	static std::mutex LastWndPtrMutex;

	LastWndPtrMutex.lock();

	LastWnd = _Wnd;

	_Wnd->hWnd = CreateWindowEx(_Descriptor->dwExStyle, _Descriptor->lpClassName, _Descriptor->lpWindowName, _Descriptor->dwStyle, _Descriptor->X, _Descriptor->Y, _Descriptor->nWidth, _Descriptor->nHeight, _Descriptor->hWndParent, _Descriptor->hMenu, _Descriptor->hInstance, _Descriptor->lpParam);

	LastWnd = nullptr;

	LastWndPtrMutex.unlock();

	if (!_Wnd->hWnd)
	{
		_Wnd->UserData = nullptr;

		*_Fail = true;
		*_Done = true;

		return;
	}

	SetLastError(0);

	if (!SetWindowLongPtr(_Wnd->hWnd, GWLP_USERDATA, (LONG_PTR)(_Wnd)))
	{
		if (GetLastError())
		{
			DestroyWindow(_Wnd->hWnd);
			_Wnd->hWnd = NULL;

			_Wnd->UserData = nullptr;

			*_Fail = true;
			*_Done = true;

			return;
		}
	}

	if (_Descriptor->WndInitFunc)
	{
		if (!_Descriptor->WndInitFunc(_Wnd))
		{
			DestroyWindow(_Wnd->hWnd);
			_Wnd->hWnd = NULL;

			_Wnd->UserData = nullptr;

			*_Fail = true;
			*_Done = true;

			return;
		}
	}

	MSG _Msg = { 0 };

	HACCEL _hAccel = _Descriptor->hAccel;

	if (_Descriptor->ShowCmd != SW_HIDE)
	{
		_Wnd->Show(_Descriptor->ShowCmd);
	}

	*_Fail = false;
	*_Done = true;

	while (GetMessage(&_Msg, NULL, 0, 0))
	{
		if (_Msg.hwnd == _Wnd->hWnd)
		{
			if (_hAccel)
			{
				if (!TranslateAccelerator(_Msg.hwnd, _hAccel, &_Msg))
				{
					TranslateMessage(&_Msg);
					DispatchMessage(&_Msg);
				}
			}
			else
			{
				TranslateMessage(&_Msg);
				DispatchMessage(&_Msg);
			}
		}
		else
		{
			TranslateMessage(&_Msg);
			DispatchMessage(&_Msg);
		}
	}

	DestroyWindow(_Wnd->hWnd);
	_Wnd->hWnd = NULL;

	_Wnd->UserData = nullptr;
}
