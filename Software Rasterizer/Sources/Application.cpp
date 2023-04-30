#include "..\Headers\BSR.hpp"



BSR::Application::Application() :
	On(false), ReturnValue(MultiProcessing::_ReturnError),
	CurrentMenu(_NullMenu), CurrentMenuObj(nullptr),
	SharedInstanceMemory(), SharedInstanceMutex(),
	FrameTime(), TimeStep(1.0f / 60.0f), LagTime(1.0f / 10.0f), Sync(60),
	hInstance(NULL), CmdLine(nullptr), ShowCmd(SW_HIDE)
{

}

BSR::Application::~Application()
{

}

const bool BSR::Application::CheckOn() const
{
	return On;
}

const int32_t BSR::Application::GetReturnValue() const
{
	return ReturnValue;
}

const uint64_t BSR::Application::GetCurrentMenu() const
{
	return CurrentMenu;
}

BSR::Menu* BSR::Application::GetCurrentMenuObj()
{
	return CurrentMenuObj;
}

const BSR::Menu* BSR::Application::GetCurrentMenuObj() const
{
	return CurrentMenuObj;
}

BSR::MultiProcessing::SharedMemory& BSR::Application::GetSharedInstanceMemory()
{
	return SharedInstanceMemory;
}

const BSR::MultiProcessing::SharedMemory& BSR::Application::GetSharedInstanceMemory() const
{
	return SharedInstanceMemory;
}

BSR::MultiProcessing::SharedMutex& BSR::Application::GetSharedInstanceMutex()
{
	return SharedInstanceMutex;
}

const BSR::MultiProcessing::SharedMutex& BSR::Application::GetSharedInstanceMutex() const
{
	return SharedInstanceMutex;
}

BSR::Time::Timer& BSR::Application::GetFrameTime(const size_t _Index)
{
	return FrameTime[_Index];
}

const BSR::Time::Timer& BSR::Application::GetFrameTime(const size_t _Index) const
{
	return FrameTime[_Index];
}

const float BSR::Application::GetTimeStep() const
{
	return TimeStep;
}

const float BSR::Application::GetTimeStepRatio() const
{
	if (LagTime != 0.0f && FrameTime[_Previous] >= LagTime)
	{
		return 1.0f;
	}
	return FrameTime[_Previous] / TimeStep;
}

const float BSR::Application::GetLagTime() const
{
	return LagTime;
}

const uint64_t BSR::Application::GetSync() const
{
	return Sync;
}

const HINSTANCE BSR::Application::GetHInstance() const
{
	return hInstance;
}

const LPWSTR BSR::Application::GetCmdLine() const
{
	return CmdLine;
}

const int BSR::Application::GetShowCmd() const
{
	return ShowCmd;
}

const size_t BSR::Application::GetArgC() const
{
	return __argc;
}

const wchar_t* BSR::Application::GetWArgV(const size_t _Index) const
{
	return __wargv[_Index];
}

void BSR::Application::TurnOn()
{
	On = true;
}

void BSR::Application::Close(const int32_t _ReturnValue)
{
	On = false;
	ReturnValue = _ReturnValue;
}

void BSR::Application::SetCurrentMenu(const uint64_t _CurrentMenu)
{
	CurrentMenu = _CurrentMenu;
}

void BSR::Application::SetCurrentMenuObj(Menu* _CurrentMenuObj)
{
	CurrentMenuObj = _CurrentMenuObj;
}

void BSR::Application::UpdateCurrentFrameTime()
{
	FrameTime[_Current].Stop();
}

void BSR::Application::SetTimeStep(const float _TimeStep)
{
	TimeStep = _TimeStep;
}

void BSR::Application::SetLagTime(const float _LagTime)
{
	LagTime = _LagTime;
}

void BSR::Application::SetSync(const uint64_t _Sync)
{
	Sync = _Sync;
}

int32_t BSR::Application::Run(const HINSTANCE _hInstance, const LPWSTR _CmdLine, const int _ShowCmd, const wchar_t* _SharedMemoryName, const wchar_t* _SharedMutexName)
{
	if (!_SharedMemoryName || !_SharedMutexName)
	{
		MessageBox(NULL, L"An unexpected error occurred!", L"Error", MB_OK | MB_ICONERROR);
		return MultiProcessing::_ReturnError;
	}

	if (!SharedInstanceMutex.Create(_SharedMutexName))
	{
		MessageBox(NULL, L"An unexpected error occurred!", L"Error", MB_OK | MB_ICONERROR);
		return MultiProcessing::_ReturnError;
	}

	if (!SharedInstanceMemory.Create(_SharedMemoryName, 1))
	{
		SharedInstanceMutex.Destroy();
		MessageBox(NULL, L"An unexpected error occurred!", L"Error", MB_OK | MB_ICONERROR);
		return MultiProcessing::_ReturnError;
	}

	SharedInstanceMutex.Lock();
	*SharedInstanceMemory = *SharedInstanceMemory + 1;
	SharedInstanceMutex.Unlock();

	On = false;
	ReturnValue = MultiProcessing::_ReturnError;
	CurrentMenu = _NullMenu;
	CurrentMenuObj = nullptr;
	FrameTime[_Previous].Restart();
	FrameTime[_Current].Restart();
	TimeStep = 1.0f / 60.0f;
	LagTime = 1.0f / 10.0f;
	Sync = 60;
	hInstance = _hInstance;
	CmdLine = _CmdLine;
	ShowCmd = _ShowCmd;

	Setup();
	while (On)
	{
		Update();
	}
	Stop();

	On = false;
	CurrentMenu = _NullMenu;
	CurrentMenuObj = nullptr;
	FrameTime[_Previous].Restart();
	FrameTime[_Current].Restart();
	TimeStep = 1.0f / 60.0f;
	LagTime = 1.0f / 10.0f;
	Sync = 60;
	hInstance = NULL;
	CmdLine = nullptr;
	ShowCmd = SW_HIDE;

	SharedInstanceMutex.Lock();
	*SharedInstanceMemory = *SharedInstanceMemory - 1;
	SharedInstanceMutex.Unlock();

	SharedInstanceMemory.Destroy();
	SharedInstanceMutex.Destroy();

	if (ReturnValue == MultiProcessing::_ReturnError)
	{
		MessageBox(NULL, L"An unexpected error occurred!", L"Error", MB_OK | MB_ICONERROR);
	}

	return ReturnValue;
}
