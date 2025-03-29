#ifndef BSR_Window_hpp

#define BSR_Window_hpp



#include "BSR.hpp"



namespace BSR
{

	struct WindowCreationDescriptor
	{
		DWORD dwExStyle = NULL;
		LPCWSTR lpClassName = nullptr;
		LPCWSTR lpWindowName = nullptr;
		DWORD dwStyle = NULL;
		int32_t X = 0;
		int32_t Y = 0;
		int32_t nWidth = 0;
		int32_t nHeight = 0;
		HWND HandleParent = NULL;
		HMENU hMenu = NULL;
		HINSTANCE hInstance = NULL;
		LPVOID lpParam = nullptr;
		HACCEL hAccel = NULL;
		int32_t ShowCmd = SW_HIDE;
		bool (*ThreadInitFnc)(void* _UserData) = nullptr;
		bool (*WndInitFunc)(Window* _Wnd) = nullptr;
		void* UserData = nullptr;
	};



	class Window
	{

	public:

		Window();
		Window(const Window& _Other) = delete;
		Window(Window&& _Other) noexcept;
		~Window();

		bool Create(const WindowCreationDescriptor* _Descriptor);
		void Destroy();
		bool Show(const int32_t _ShowCmd);
		bool UpdateContent();

		const bool CheckOn() const;
		const HWND GetHandle() const;
		void* GetUserData();
		const void* GetUserData() const;
		const bool GetClientSize(int32_t& _Width, int32_t& _Height) const;
		const bool GetWindowSize(int32_t& _Width, int32_t& _Height) const;
		const size_t GetRefreshRate() const;

		operator const HWND() const;

		void operator= (const Window& _Other) = delete;
		void operator= (Window&& _Other) noexcept;

		static Window* GetWindowPtr(const HWND _Handle);

	private:

		static void WndThreadFunc(bool* _Done, bool* _Fail, Window* _Wnd, const WindowCreationDescriptor* _Descriptor);

		static Window* LastWnd;

		HWND Handle;
		std::thread WndThread;
		void* UserData;

	};

}



#endif
