#include "..\Headers\BSR.hpp"



BSR::RunTime::Application::Application() :
	On(false), ReturnValue(MultiProcessing::_ReturnError),
	CurrentMenu(_NullMenu),
	SharedInstanceMemory(), SharedInstanceMutex(),
	FrameTime(), LagTime(1.0f / 10.0f), SimulationSpeed(1.0f), Sync(60),
	InstanceHandle(NULL), CmdLine(nullptr), ShowCmd(SW_HIDE)
{

}

BSR::RunTime::Application::Application(Application&& _Other) noexcept :
	On(_Other.On), ReturnValue(_Other.ReturnValue),
	CurrentMenu(_Other.CurrentMenu),
	SharedInstanceMemory(std::move(_Other.SharedInstanceMemory)), SharedInstanceMutex(std::move(_Other.SharedInstanceMutex)),
	FrameTime(), LagTime(_Other.LagTime), SimulationSpeed(_Other.SimulationSpeed), Sync(_Other.Sync),
	InstanceHandle(_Other.InstanceHandle), CmdLine(_Other.CmdLine), ShowCmd(_Other.ShowCmd)
{
	FrameTime[_Previous] = std::move(_Other.FrameTime[_Previous]);
	FrameTime[_Current] = std::move(_Other.FrameTime[_Current]);

	_Other.On = false;
	_Other.ReturnValue = MultiProcessing::_ReturnError;
	_Other.CurrentMenu = _NullMenu;
	_Other.LagTime = 1.0f / 10.0f;
	_Other.SimulationSpeed = 1.0f;
	_Other.Sync = 60;
	_Other.InstanceHandle = NULL;
	_Other.CmdLine = nullptr;
	_Other.ShowCmd = SW_HIDE;
}

BSR::RunTime::Application::~Application()
{
	BSR_ASSERT_MSG(On == false, BSR_STRING_TYPE("Can not delete an application object that is running."));
}

int32_t BSR::RunTime::Application::Run(const HINSTANCE _InstanceHandle, const LPWSTR _CmdLine, const int32_t _ShowCmd, const wchar_t* _SharedMemoryName, const wchar_t* _SharedMutexName)
{
	if (!SharedInstanceMemory.Create(_SharedMemoryName, 8))
	{
		return MultiProcessing::_ReturnError;
	}

	if (!SharedInstanceMutex.Create(_SharedMutexName))
	{
		SharedInstanceMemory.Destroy();

		return MultiProcessing::_ReturnError;
	}

	SharedInstanceMutex.Lock();
	(*(uint64_t*)(SharedInstanceMemory.GetData()))++;
	SharedInstanceMutex.Unlock();

	InstanceHandle = _InstanceHandle;
	CmdLine = _CmdLine;
	ShowCmd = _ShowCmd;

	Setup();
	while (On)
	{
		Update();
	}
	Stop();

	CurrentMenu = _NullMenu;

	FrameTime[_Previous].Restart();
	FrameTime[_Current].Restart();
	LagTime = 1.0f / 10.0f;
	SimulationSpeed = 1.0f;
	Sync = 60;

	InstanceHandle = NULL;
	CmdLine = nullptr;
	ShowCmd = SW_HIDE;

	SharedInstanceMutex.Lock();
	(*(uint64_t*)(SharedInstanceMemory.GetData()))--;
	SharedInstanceMutex.Unlock();

	SharedInstanceMutex.Destroy();
	SharedInstanceMemory.Destroy();

	int32_t _ReturnValue = ReturnValue;
	ReturnValue = MultiProcessing::_ReturnError;

	return _ReturnValue;
}

void BSR::RunTime::Application::TurnOn()
{
	On = true;
}

void BSR::RunTime::Application::Close(const int32_t _ReturnValue)
{
	On = false;
	ReturnValue = _ReturnValue;
}

void BSR::RunTime::Application::SetCurrentMenu(const uint64_t _CurrentMenu)
{
	CurrentMenu = _CurrentMenu;
}

void BSR::RunTime::Application::UpdateCurrentFrameTime()
{
	FrameTime[_Current].Stop();
}

void BSR::RunTime::Application::SetLagTime(const float _LagTime)
{
	LagTime = _LagTime;
}

void BSR::RunTime::Application::SetSimulationSpeed(const float _SimulationSpeed)
{
	SimulationSpeed = _SimulationSpeed;
}

void BSR::RunTime::Application::SetSync(const uint64_t _Sync)
{
	Sync = _Sync;
}

const bool BSR::RunTime::Application::CheckOn() const
{
	return On;
}

const int32_t BSR::RunTime::Application::GetReturnValue() const
{
	return ReturnValue;
}

const uint64_t BSR::RunTime::Application::GetCurrentMenu() const
{
	return CurrentMenu;
}

const BSR::MultiProcessing::SharedMemory& BSR::RunTime::Application::GetSharedInstanceMemory() const
{
	return SharedInstanceMemory;
}

const BSR::MultiProcessing::SharedMutex& BSR::RunTime::Application::GetSharedInstanceMutex() const
{
	return SharedInstanceMutex;
}

BSR::Time::Timer& BSR::RunTime::Application::GetFrameTime(const size_t _Index)
{
	return FrameTime[_Index];
}

const BSR::Time::Timer& BSR::RunTime::Application::GetFrameTime(const size_t _Index) const
{
	return FrameTime[_Index];
}

const float BSR::RunTime::Application::GetTimeStep() const
{
	if (LagTime != 0.0f && FrameTime[_Previous] >= LagTime)
	{
		return LagTime * SimulationSpeed;
	}

	return FrameTime[_Previous] * SimulationSpeed;
}

const float BSR::RunTime::Application::GetLagTime() const
{
	return LagTime;
}

const float BSR::RunTime::Application::GetSimulationSpeed() const
{
	return SimulationSpeed;
}

const uint64_t BSR::RunTime::Application::GetSync() const
{
	return Sync;
}

const HINSTANCE BSR::RunTime::Application::GetInstanceHandle() const
{
	return InstanceHandle;
}

const LPWSTR BSR::RunTime::Application::GetCmdLine() const
{
	return CmdLine;
}

const int32_t BSR::RunTime::Application::GetShowCmd() const
{
	return ShowCmd;
}

const size_t BSR::RunTime::Application::GetArgC() const
{
	return __argc;
}

const wchar_t* BSR::RunTime::Application::GetArgV(const size_t _Index) const
{
	return __wargv[_Index];
}

void BSR::RunTime::Application::operator= (Application&& _Other) noexcept
{
	if (On)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not move an application object to one that is running."));
	}

	On = _Other.On;
	ReturnValue = _Other.ReturnValue;
	CurrentMenu = _Other.CurrentMenu;
	SharedInstanceMemory = std::move(_Other.SharedInstanceMemory);
	SharedInstanceMutex = std::move(_Other.SharedInstanceMutex);
	FrameTime[_Previous] = std::move(_Other.FrameTime[_Previous]);
	FrameTime[_Current] = std::move(_Other.FrameTime[_Current]);
	LagTime = _Other.LagTime;
	SimulationSpeed = _Other.SimulationSpeed;
	Sync = _Other.Sync;
	InstanceHandle = _Other.InstanceHandle;
	CmdLine = _Other.CmdLine;
	ShowCmd = _Other.ShowCmd;

	_Other.On = false;
	_Other.ReturnValue = MultiProcessing::_ReturnError;
	_Other.CurrentMenu = _NullMenu;
	_Other.LagTime = 1.0f / 10.0f;
	_Other.SimulationSpeed = 1.0f;
	_Other.Sync = 60;
	_Other.InstanceHandle = NULL;
	_Other.CmdLine = nullptr;
	_Other.ShowCmd = SW_HIDE;
}



BSR::RunTime::Menu::Menu() : On(false), NextMenu(_NullMenu), ApplicationObj(nullptr)
{

}

BSR::RunTime::Menu::Menu(Menu&& _Other) noexcept : On(_Other.On), NextMenu(_Other.NextMenu), ApplicationObj(_Other.ApplicationObj)
{
	_Other.On = false;
	_Other.NextMenu = _NullMenu;
	_Other.ApplicationObj = nullptr;
}

BSR::RunTime::Menu::~Menu()
{
	BSR_ASSERT_MSG(On == false, BSR_STRING_TYPE("Can not delete a menu object that is running."));
}

void BSR::RunTime::Menu::Run(Application* _ApplicationObj)
{
	if (!_ApplicationObj)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("An application is required to run a menu."));
		return;
	}

	ApplicationObj = _ApplicationObj;

	Setup();
	while (On)
	{
		GetFrameTime(_Current).Start();

		Update();

		if (GetSync())
		{
			GetFrameTime(_Current).Stop();

			if (GetFrameTime(_Current) < 1.0f / (float)(GetSync()))
			{
				std::this_thread::sleep_for(std::chrono::microseconds((uint64_t)(floorf((1.0f / (float)(GetSync()) - GetFrameTime(_Current)) * 1000000.0f))));
			}
		}

		GetFrameTime(_Current).Stop();

		GetFrameTime(_Previous) = GetFrameTime(_Current);

		if (!ApplicationObj->CheckOn())
		{
			Close(_NullMenu);
		}
	}
	Stop();

	ApplicationObj->SetCurrentMenu(NextMenu);
	NextMenu = _NullMenu;

	ApplicationObj = nullptr;
}

void BSR::RunTime::Menu::operator= (Menu&& _Other) noexcept
{
	if (On)
	{
		BSR_DEBUG_BREAK_MSG(BSR_STRING_TYPE("Can not move a menu object to one that is running."));
	}

	On = _Other.On;
	NextMenu = _Other.NextMenu;
	ApplicationObj = _Other.ApplicationObj;

	_Other.On = false;
	_Other.NextMenu = _NullMenu;
	_Other.ApplicationObj = nullptr;
}

void BSR::RunTime::Menu::TurnOn()
{
	On = true;
}

void BSR::RunTime::Menu::Close(const uint64_t _NextMenu)
{
	On = false;
	NextMenu = _NextMenu;
}

void BSR::RunTime::Menu::UpdateCurrentFrameTime()
{
	ApplicationObj->UpdateCurrentFrameTime();
}

void BSR::RunTime::Menu::SetLagTime(const float _LagTime)
{
	ApplicationObj->SetLagTime(_LagTime);
}

void BSR::RunTime::Menu::SetSimulationSpeed(const float _SimulationSpeed)
{
	ApplicationObj->SetSimulationSpeed(_SimulationSpeed);
}

void BSR::RunTime::Menu::SetSync(const uint64_t _Sync)
{
	ApplicationObj->SetSync(_Sync);
}

const bool BSR::RunTime::Menu::CheckOn() const
{
	return On;
}

const uint64_t BSR::RunTime::Menu::GetNextMenu() const
{
	return NextMenu;
}

BSR::RunTime::Application* BSR::RunTime::Menu::GetApplicationObj()
{
	return ApplicationObj;
}

const BSR::RunTime::Application* BSR::RunTime::Menu::GetApplicationObj() const
{
	return ApplicationObj;
}

BSR::Time::Timer& BSR::RunTime::Menu::GetFrameTime(const size_t _Index)
{
	return ApplicationObj->GetFrameTime(_Index);
}

const BSR::Time::Timer& BSR::RunTime::Menu::GetFrameTime(const size_t _Index) const
{
	return ApplicationObj->GetFrameTime(_Index);
}

const float BSR::RunTime::Menu::GetTimeStep() const
{
	return ApplicationObj->GetTimeStep();
}

const float BSR::RunTime::Menu::GetLagTime() const
{
	return ApplicationObj->GetLagTime();
}

const float BSR::RunTime::Menu::GetSimulationSpeed() const
{
	return ApplicationObj->GetSimulationSpeed();
}

const uint64_t BSR::RunTime::Menu::GetSync() const
{
	return ApplicationObj->GetSync();
}
