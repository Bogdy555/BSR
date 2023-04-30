#include "..\Headers\BSR_APP.hpp"



BSR_APP::MainMenu::MainMenu() : BSR::Menu()
{

}

BSR_APP::MainMenu::~MainMenu()
{

}

const uint64_t BSR_APP::MainMenu::GetMenuType() const
{
	return _MainMenu;
}

void BSR_APP::MainMenu::Setup()
{
	TurnOn();
}

void BSR_APP::MainMenu::Update()
{
	Input();
	Keys();
	Mouse();
	Engine();
	Animations();
	FrameBuild();
}

void BSR_APP::MainMenu::Stop()
{

}

void BSR_APP::MainMenu::Input()
{

}

void BSR_APP::MainMenu::DeleteInputs()
{

}

void BSR_APP::MainMenu::Keys()
{

}

void BSR_APP::MainMenu::Mouse()
{

}

void BSR_APP::MainMenu::Engine()
{

}

void BSR_APP::MainMenu::Animations()
{

}

void BSR_APP::MainMenu::FrameBuild()
{

}
