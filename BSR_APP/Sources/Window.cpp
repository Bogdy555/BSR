#include "..\Headers\BSR_APP.hpp"



BSR_APP::WindowData::WindowData() : CloseMutex(nullptr), Close(false), InputMutex(nullptr), Focus(false), Keys(), MinSizeMutex(nullptr), MinWidth(700), MinHeight(400), ImageMutex(nullptr), Image()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = false;
	}
}

BSR_APP::WindowData::WindowData(const WindowData& _Other) : CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight), ImageMutex(_Other.ImageMutex), Image(_Other.Image)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
	}
}

BSR_APP::WindowData::WindowData(WindowData&& _Other) noexcept : CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight), ImageMutex(_Other.ImageMutex), Image(_Other.Image)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
		_Other.Keys[_Index] = false;
	}

	_Other.CloseMutex = nullptr;
	_Other.Close = false;
	_Other.InputMutex = nullptr;
	_Other.Focus = false;
	_Other.MinSizeMutex = nullptr;
	_Other.MinWidth = 700;
	_Other.MinHeight = 400;
	_Other.ImageMutex = nullptr;
	_Other.Image = BSR::Image::Image();
}

BSR_APP::WindowData::~WindowData()
{

}

void BSR_APP::WindowData::operator= (const WindowData& _Other)
{
	CloseMutex = _Other.CloseMutex;
	Close = _Other.Close;
	InputMutex = _Other.InputMutex;
	Focus = _Other.Focus;
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
	}
	MinSizeMutex = _Other.MinSizeMutex;
	MinWidth = _Other.MinWidth;
	MinHeight = _Other.MinHeight;
	ImageMutex = _Other.ImageMutex;
	Image = _Other.Image;
}

void BSR_APP::WindowData::operator= (WindowData&& _Other) noexcept
{
	CloseMutex = _Other.CloseMutex;
	Close = _Other.Close;
	InputMutex = _Other.InputMutex;
	Focus = _Other.Focus;
	MinSizeMutex = _Other.MinSizeMutex;
	MinWidth = _Other.MinWidth;
	MinHeight = _Other.MinHeight;
	ImageMutex = _Other.ImageMutex;
	Image = _Other.Image;

	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
		_Other.Keys[_Index] = false;
	}

	_Other.CloseMutex = nullptr;
	_Other.Close = false;
	_Other.InputMutex = nullptr;
	_Other.Focus = false;
	_Other.MinSizeMutex = nullptr;
	_Other.MinWidth = 700;
	_Other.MinHeight = 400;
	_Other.ImageMutex = nullptr;
	_Other.Image = BSR::Image::Image();
}

bool BSR_APP::InitWindowThread(void* _UserData)
{
	return true;
}

bool BSR_APP::InitWindow(BSR::Window* _Wnd)
{
	return true;
}

LRESULT CALLBACK BSR_APP::WindowProcedure(_In_ HWND _hWnd, _In_ UINT _Msg, _In_ WPARAM _wParam, _In_ LPARAM _lParam)
{
	BSR::Window* _Wnd = BSR::Window::GetWindowPtr(_hWnd);

	BSR_ASSERT(_Wnd != nullptr);

	WindowData* _WndData = (WindowData*)(_Wnd->GetUserData());

	BSR_ASSERT(_WndData != nullptr);

	switch (_Msg)
	{
	case WM_CLOSE:
	{
		_WndData->CloseMutex->lock();

		_WndData->Close = true;

		_WndData->CloseMutex->unlock();

		break;
	}
	case WM_KEYDOWN:
	{
		_WndData->InputMutex->lock();

		_WndData->Keys[_wParam] = true;

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_KEYUP:
	{
		_WndData->InputMutex->lock();

		_WndData->Keys[_wParam] = false;

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_SETFOCUS:
	{
		_WndData->InputMutex->lock();

		_WndData->Focus = true;

		for (size_t _Index = 0; _Index < 256; _Index++)
		{
			_WndData->Keys[_Index] = false;
		}

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_KILLFOCUS:
	{
		_WndData->InputMutex->lock();

		_WndData->Focus = false;

		for (size_t _Index = 0; _Index < 256; _Index++)
		{
			_WndData->Keys[_Index] = false;
		}

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_ERASEBKGND:
	{
		break;
	}
	case WM_PAINT:
	{
		int32_t _Width = 0, _Height = 0;

		_Wnd->GetClientSize(_Width, _Height);

		PAINTSTRUCT _PaintStruct = { 0 };

		HDC _WndDC = BeginPaint(_hWnd, &_PaintStruct);

		_WndData->ImageMutex->lock();

		HBITMAP _BackBmp = CreateCompatibleBitmap(_WndDC, (int)(_WndData->Image.Width), (int)(_WndData->Image.Height));
		HDC _BackDC = CreateCompatibleDC(_WndDC);
		HBITMAP _OldBmp = (HBITMAP)(SelectObject(_BackDC, _BackBmp));

		uint8_t* _Image = new uint8_t[(size_t)(_WndData->Image.Width) * (size_t)(_WndData->Image.Height) * 4];

		for (size_t _Y = 0; _Y < _WndData->Image.Height; _Y++)
		{
			for (size_t _X = 0; _X < _WndData->Image.Width; _X++)
			{
				_Image[(_X + _Y * _WndData->Image.Width) * 4 + 0] = _WndData->Image.Data[(_X + (_WndData->Image.Height - 1 - _Y) * _WndData->Image.Width) * 3 + 2];
				_Image[(_X + _Y * _WndData->Image.Width) * 4 + 1] = _WndData->Image.Data[(_X + (_WndData->Image.Height - 1 - _Y) * _WndData->Image.Width) * 3 + 1];
				_Image[(_X + _Y * _WndData->Image.Width) * 4 + 2] = _WndData->Image.Data[(_X + (_WndData->Image.Height - 1 - _Y) * _WndData->Image.Width) * 3 + 0];
				_Image[(_X + _Y * _WndData->Image.Width) * 4 + 3] = 0;
			}
		}

		SetBitmapBits(_BackBmp, (DWORD)(_WndData->Image.Width) * (DWORD)(_WndData->Image.Height) * 4, _Image);

		delete[] _Image;

		StretchBlt(_WndDC, 0, 0, _Width, _Height, _BackDC, 0, 0, (int)(_WndData->Image.Width), (int)(_WndData->Image.Height), SRCCOPY);

		SelectObject(_BackDC, _OldBmp);
		DeleteDC(_BackDC);
		DeleteObject(_BackBmp);

		_WndData->ImageMutex->unlock();

		EndPaint(_hWnd, &_PaintStruct);

		break;
	}
	case WM_NCHITTEST:
	{
		LRESULT _Result = DefWindowProc(_hWnd, _Msg, _wParam, _lParam);

		if (_Result != HTCLIENT)
		{
			return _Result;
		}

		return HTCAPTION;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO _MinMaxInfo = (LPMINMAXINFO)(_lParam);

		_WndData->MinSizeMutex->lock();

		_MinMaxInfo->ptMinTrackSize.x = _WndData->MinWidth;
		_MinMaxInfo->ptMinTrackSize.y = _WndData->MinHeight;

		_WndData->MinSizeMutex->unlock();

		break;
	}
	default:
	{
		return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
	}
	}

	return 0;
}
