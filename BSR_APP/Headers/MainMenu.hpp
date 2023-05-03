#ifndef BSR_APP_MainMenu_hpp

#define BSR_APP_MainMenu_hpp



#include "..\Headers\BSR_APP.hpp"



namespace BSR_APP
{

	namespace RunTime
	{

		enum MenuTypes : const uint64_t
		{
			_MainMenu = 1
		};

		class MainMenu : public BSR::RunTime::Menu
		{

		public:

			MainMenu();
			MainMenu(const MainMenu& _Other) = delete;
			MainMenu(MainMenu&& _Other) noexcept;
			~MainMenu();

			void operator= (const MainMenu& _Other) = delete;
			void operator= (MainMenu&& _Other) noexcept;

		private:

			const uint64_t GetType() const override;

			void Setup() override;
			void Update() override;
			void Stop() override;

		};

	}

}



#endif
