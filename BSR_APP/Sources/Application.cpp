#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::Application::Application() : BSR::RunTime::Application()
{

}

BSR_APP::RunTime::Application::Application(Application&& _Other) noexcept : BSR::RunTime::Application(std::move(_Other))
{

}

BSR_APP::RunTime::Application::~Application()
{

}

void BSR_APP::RunTime::Application::operator= (Application&& _Other) noexcept
{
	(BSR::RunTime::Application&)(*this) = std::move((BSR::RunTime::Application&)(_Other));
}

void BSR_APP::RunTime::Application::Setup()
{
	SetCurrentMenu(_MainMenu);
	TurnOn();
}

void BSR_APP::RunTime::Application::Update()
{
	switch (GetCurrentMenu())
	{
	case _MainMenu:
	{
		MainMenu _Menu;
		_Menu.Run(this);
		break;
	}
	case BSR::RunTime::_NullMenu:
	{
		BSR_DEBUG_BREAK();
		Close(BSR::MultiProcessing::_ReturnError);
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

void BSR_APP::RunTime::Application::Stop()
{

}
