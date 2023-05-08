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
	DeleteInput();

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

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	_MainWindowData.InputMutex->lock();

	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = Keys[BSR::_Current][_Index];
		Keys[BSR::_Current][_Index] = _MainWindowData.Keys[_Index];
	}

	_MainWindowData.InputMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::DeleteInput()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[BSR::_Previous][_Index] = false;
		Keys[BSR::_Current][_Index] = false;
	}
}

void BSR_APP::RunTime::MainMenu::Engine()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	if (Keys[BSR::_Current][VK_ESCAPE])
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	_ApplicationObj->SetSync(_MainWindow.GetRefreshRate());

	_MainWindowData.CloseMutex->lock();

	if (_MainWindowData.Close)
	{
		_ApplicationObj->Close(BSR::MultiProcessing::_ReturnNoError);
	}

	_MainWindowData.CloseMutex->unlock();
}

void BSR_APP::RunTime::MainMenu::FrameBuild()
{
	Application* _ApplicationObj = (Application*)(GetApplicationObj());

	BSR::Window& _MainWindow = _ApplicationObj->GetMainWindow();

	WindowData& _MainWindowData = _ApplicationObj->GetMainWindowData();

	_MainWindowData.ImageMutex->lock();

	{
		delete[] _MainWindowData.Image.Data;

		int32_t _NewWidth = 0, _NewHeight = 0;
		_MainWindow.GetClientSize(_NewWidth, _NewHeight);

		_MainWindowData.Image.Width = _NewWidth / 10;
		_MainWindowData.Image.Height = _NewHeight / 10;
		_MainWindowData.Image.Data = new uint8_t[_MainWindowData.Image.Width * _MainWindowData.Image.Height * 3];
	}

	for (size_t _Y = 0; _Y < _MainWindowData.Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _MainWindowData.Image.Width; _X++)
		{
			_MainWindowData.Image.Data[(_X + _Y * _MainWindowData.Image.Width) * 3 + 0] = (uint8_t)(((float)(_X) / (float)(_MainWindowData.Image.Width) + 0.5f / (float)(_MainWindowData.Image.Width)) * 255.0f);
			_MainWindowData.Image.Data[(_X + _Y * _MainWindowData.Image.Width) * 3 + 1] = (uint8_t)(((float)(_Y) / (float)(_MainWindowData.Image.Height) + 0.5f / (float)(_MainWindowData.Image.Height)) * 255.0f);
			_MainWindowData.Image.Data[(_X + _Y * _MainWindowData.Image.Width) * 3 + 2] = 0;
		}
	}

	_MainWindowData.ImageMutex->unlock();

	_MainWindow.UpdateContent();
}
