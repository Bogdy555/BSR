#include "..\Headers\BSR_APP.hpp"



BSR_APP::Application::Application() : BSR::Application()
{

}

BSR_APP::Application::~Application()
{

}

void BSR_APP::Application::Setup()
{
	SetCurrentMenu(_MainMenu);
	TurnOn();
}

void BSR_APP::Application::Update()
{
	switch (GetCurrentMenu())
	{
	case _MainMenu:
	{
		std::unique_ptr<BSR::Menu> _Menu = std::make_unique<BSR_APP::MainMenu>();
		_Menu->Run(this);
		break;
	}
	default:
	{
		BSR_DEBUG_BREAK();
		Close(BSR::MultiProcessing::_ReturnError);
		break;
	}
	}
}

void BSR_APP::Application::Stop()
{

}
