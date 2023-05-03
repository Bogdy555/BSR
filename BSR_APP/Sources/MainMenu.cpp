#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::MainMenu::MainMenu() : BSR::RunTime::Menu()
{

}

BSR_APP::RunTime::MainMenu::MainMenu(MainMenu&& _Other) noexcept : BSR::RunTime::Menu(std::move(_Other))
{

}

BSR_APP::RunTime::MainMenu::~MainMenu()
{

}

void BSR_APP::RunTime::MainMenu::operator= (MainMenu&& _Other) noexcept
{
	(BSR::RunTime::Menu&)(*this) = std::move((BSR::RunTime::Menu&)(_Other));
}

const uint64_t BSR_APP::RunTime::MainMenu::GetType() const
{
	return _MainMenu;
}

void BSR_APP::RunTime::MainMenu::Setup()
{
	TurnOn();
}

void BSR_APP::RunTime::MainMenu::Update()
{

}

void BSR_APP::RunTime::MainMenu::Stop()
{

}
