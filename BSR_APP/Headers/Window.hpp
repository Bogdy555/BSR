#ifndef BSR_APP_Window_hpp

#define BSR_APP_Window_hpp



#include "BSR_APP.hpp"



namespace BSR_APP
{

	struct WindowData
	{
		std::mutex* CloseMutex = nullptr;
		bool Close = false;

		std::mutex* InputMutex = nullptr;
		bool Focus = false;
		bool Keys[256];

		std::mutex* MinSizeMutex = nullptr;
		int32_t MinWidth = 700;
		int32_t MinHeight = 400;
	};

	bool InitWindowThread(void* _UserData);
	bool InitWindow(BSR::Window* _Wnd);

	LRESULT CALLBACK WindowProcedure(_In_ HWND _hWnd, _In_ UINT _Msg, _In_ WPARAM _wParam, _In_ LPARAM _lParam);

}



#endif
