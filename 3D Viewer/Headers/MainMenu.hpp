#ifndef MainMenu_hpp

#define MainMenu_hpp



#include "BSR_APP.hpp"



namespace BSR_APP
{

	enum MenuTypes : const uint64_t
	{
		_MainMenu = 1
	};

	class MainMenu : public BSR::Menu
	{

	public:

		MainMenu();
		MainMenu(const MainMenu& _Other) = delete;
		MainMenu(MainMenu&& _Other) noexcept = delete;
		~MainMenu();

		const uint64_t GetMenuType() const override;

		void Setup() override;
		void Update() override;
		void Stop() override;

		void operator= (const MainMenu& _Other) = delete;
		void operator= (MainMenu&& _Other) noexcept = delete;

	private:

		void Input() override;
		void DeleteInputs() override;
		void Keys() override;
		void Mouse() override;
		void Engine() override;
		void Animations() override;
		void FrameBuild() override;

	};

}



#endif
