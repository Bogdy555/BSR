#include "..\Headers\BSR_APP.hpp"



BSR_APP::RunTime::MainMenu::MainMenu() : BSR::RunTime::Menu(), Keys()
{

}

BSR_APP::RunTime::MainMenu::~MainMenu()
{

}

const uint64_t BSR_APP::RunTime::MainMenu::GetType() const
{
	return _MainMenu;
}

void BSR_APP::RunTime::MainMenu::Setup()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = false;
		Keys[BSR::_Current][_Index] = false;
	}

	TurnOn();
}

void BSR_APP::RunTime::MainMenu::Update()
{
	Input();
	Engine();
	FrameBuild();
}

void BSR_APP::RunTime::MainMenu::Stop()
{

}

void BSR_APP::RunTime::MainMenu::Input()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	_ApplicationObj->GetMainWindowData().InputMutex->lock();

	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = Keys[BSR::_Current][_Index];
		Keys[BSR::_Current][_Index] = _ApplicationObj->GetMainWindowData().Keys[_Index];
	}

	_ApplicationObj->GetMainWindowData().InputMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::Engine()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	if (Keys[BSR::_Current][VK_ESCAPE])
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	_ApplicationObj->SetSync(_ApplicationObj->GetMainWindow().GetRefreshRate());

	_ApplicationObj->GetMainWindowData().CloseMutex->lock();

	if (_ApplicationObj->GetMainWindowData().Close)
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	_ApplicationObj->GetMainWindowData().CloseMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::FrameBuild()
{

}
