#include "..\Headers\BSR.hpp"



BSR::Menu::Menu() : On(false), NextMenu(_NullMenu), ApplicationObj(nullptr)
{

}

BSR::Menu::~Menu()
{

}

const uint64_t BSR::Menu::GetMenuType() const
{
	return _NullMenu;
}

const bool BSR::Menu::CheckOn() const
{
	return On;
}

const uint64_t BSR::Menu::GetNextMenu() const
{
	return NextMenu;
}

BSR::Application* BSR::Menu::GetApplicationObj()
{
	return ApplicationObj;
}

const BSR::Application* BSR::Menu::GetApplicationObj() const
{
	return ApplicationObj;
}

BSR::Time::Timer& BSR::Menu::GetFrameTime(const size_t _Index)
{
	return ApplicationObj->GetFrameTime(_Index);
}

const BSR::Time::Timer& BSR::Menu::GetFrameTime(const size_t _Index) const
{
	return ApplicationObj->GetFrameTime(_Index);
}

const float BSR::Menu::GetTimeStep() const
{
	return ApplicationObj->GetTimeStep();
}

const float BSR::Menu::GetTimeStepRatio() const
{
	return ApplicationObj->GetTimeStepRatio();
}

const float BSR::Menu::GetLagTime() const
{
	return ApplicationObj->GetLagTime();
}

const uint64_t BSR::Menu::GetSync() const
{
	return ApplicationObj->GetSync();
}

void BSR::Menu::TurnOn()
{
	On = true;
}

void BSR::Menu::Close(const uint64_t _NextMenu)
{
	On = false;
	NextMenu = _NextMenu;
}

void BSR::Menu::BindApplication(Application* _ApplicationObj)
{
	ApplicationObj = _ApplicationObj;
}

void BSR::Menu::UpdateCurrentFrameTime()
{
	ApplicationObj->UpdateCurrentFrameTime();
}

void BSR::Menu::SetTimeStep(const float _TimeStep)
{
	ApplicationObj->SetTimeStep(_TimeStep);
}

void BSR::Menu::SetLagTime(const float _LagTime)
{
	ApplicationObj->SetLagTime(_LagTime);
}

void BSR::Menu::SetSync(const uint64_t _Sync)
{
	ApplicationObj->SetSync(_Sync);
}

void BSR::Menu::Run(Application* _ApplicationObj)
{
	if (!_ApplicationObj)
	{
		return;
	}

	ApplicationObj = _ApplicationObj;
	ApplicationObj->SetCurrentMenuObj(this);

	Setup();
	while (On)
	{
		GetFrameTime(_Current).Start();

		Update();

		if (GetSync())
		{
			GetFrameTime(_Current).Stop();

			float _ExpectedTime = 1000000.0f / (float)(GetSync());
			float _RealTime = GetFrameTime(_Current) * 1000000.0f;

			if (_RealTime < _ExpectedTime)
			{
				std::this_thread::sleep_for(std::chrono::microseconds((uint64_t)(_ExpectedTime) - (uint64_t)(_RealTime)));
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
	ApplicationObj = nullptr;
}
