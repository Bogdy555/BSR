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

			Application(const Application& _Other) = delete;
			Application(Application&& _Other) noexcept = delete;
			~Application();

			BSR::Window& GetMainWindow();
			const BSR::Window& GetMainWindow() const;

			WindowData& GetMainWindowData();
			const WindowData& GetMainWindowData() const;

			BSR::AssetManager& GetSceneAssets();
			const BSR::AssetManager& GetSceneAssets() const;

			void UpdateFullScreen();

			static Application* GetInstance();

			void operator= (const Application& _Other) = delete;
			void operator= (Application&& _Other) noexcept = delete;

		private:

			Application();

			BSR::Window MainWindow;
			WindowData MainWindowData;
			std::mutex PlacementMutex;
			std::mutex RectMutex;
			std::mutex FullScreenMutex;
			std::mutex CloseMutex;
			std::mutex MinSizeMutex;
			std::mutex InputMutex;
			std::mutex ImageMutex;

			BSR::AssetManager SceneAssets;

			bool InitInstance();

			bool InitMainWindow();
			void CleanUpMainWindow();

			bool LoadModel(const wchar_t* _Path, const wchar_t* _AssetName);
			void CleanUpModel(const wchar_t* _AssetName);

			bool LoadTexture_R(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType);
			void CleanUpTexture_R(const wchar_t* _AssetName);

			bool LoadTexture_RG(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType);
			void CleanUpTexture_RG(const wchar_t* _AssetName);

			bool LoadTexture_RGB(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType);
			void CleanUpTexture_RGB(const wchar_t* _AssetName);

			bool LoadTexture_Float_RGB(const wchar_t* _Path, const wchar_t* _AssetName, const uint8_t _LerpType, const uint8_t _WrapType);
			void CleanUpTexture_Float_RGB(const wchar_t* _AssetName);

			bool GenerateWhiteTextures();
			void CleanUpWhiteTextures();

			bool GenerateMaterial(const wchar_t* _AssetName, const wchar_t* _Albedo, const wchar_t* _Metalness, const wchar_t* _Roughness, const wchar_t* _AmbientOcclusion, const wchar_t* _NormalMap);
			void CleanUpMaterial(const wchar_t* _AssetName);

			bool InitSceneAssets();
			void CleanUpSceneAssets();

			void Setup() override;
			void Update() override;
			void Stop() override;

		};

	}

}



#endif
