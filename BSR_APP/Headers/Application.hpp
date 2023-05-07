#ifndef BSR_APP_Application_hpp

#define BSR_APP_Application_hpp



#include "..\Headers\BSR_APP.hpp"



namespace BSR_APP
{

	namespace RunTime
	{

		class Application : public BSR::RunTime::Application
		{

		public:

			Application();
			Application(const Application& _Other) = delete;
			Application(Application&& _Other) noexcept = delete;
			~Application();

			BSR::Window& GetMainWindow();
			const BSR::Window& GetMainWindow() const;

			WindowData& GetMainWindowData();
			const WindowData& GetMainWindowData() const;

			void operator= (const Application& _Other) = delete;
			void operator= (Application&& _Other) noexcept = delete;

		private:

			BSR::Window MainWindow;
			WindowData MainWindowData;
			std::mutex CloseMutex;
			std::mutex MinSizeMutex;
			std::mutex InputMutex;

			bool InitInstance();

			bool InitMainWindow();
			void CleanUpMainWindow();

			void Setup() override;
			void Update() override;
			void Stop() override;

		};

	}

}



#endif
