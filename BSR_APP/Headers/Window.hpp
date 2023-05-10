#ifndef BSR_APP_Window_hpp

#define BSR_APP_Window_hpp



#include "BSR_APP.hpp"



namespace BSR_APP
{

	struct WindowData
	{

		std::mutex* PlacementMutex;
		WINDOWPLACEMENT Placement;

		std::mutex* RectMutex;
		RECT Rect;

		std::mutex* FullScreenMutex;
		bool FullScreen;

		std::mutex* CloseMutex;
		bool Close;

		std::mutex* InputMutex;
		bool Focus;
		bool Keys[256];

		std::mutex* MinSizeMutex;
		int32_t MinWidth;
		int32_t MinHeight;

		std::mutex* ImageMutex;
		BSR::Image::Image Image;
		float* Depth;

		WindowData();
		WindowData(const WindowData& _Other);
		WindowData(WindowData&& _Other) noexcept;
		~WindowData();

		void operator= (const WindowData& _Other);
		void operator= (WindowData&& _Other) noexcept;

	};

	bool InitWindowThread(void* _UserData);
	bool InitWindow(BSR::Window* _Wnd);

	LRESULT CALLBACK WindowProcedure(_In_ HWND _hWnd, _In_ UINT _Msg, _In_ WPARAM _wParam, _In_ LPARAM _lParam);

}



#endif
