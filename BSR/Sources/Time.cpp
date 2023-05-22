#include "../Headers/BSR.hpp"



BSR::Time::Timer::Timer() : Begin(), End()
{

}

BSR::Time::Timer::Timer(const Timer& _Other) : Begin(_Other.Begin), End(_Other.End)
{

}

BSR::Time::Timer::Timer(Timer&& _Other) noexcept : Begin(std::move(_Other.Begin)), End(std::move(_Other.End))
{

}

BSR::Time::Timer::~Timer()
{

}

void BSR::Time::Timer::Start()
{
	Begin = std::chrono::system_clock::now();
}

void BSR::Time::Timer::Stop()
{
	End = std::chrono::system_clock::now();
}

void BSR::Time::Timer::Restart()
{
	Begin = std::chrono::system_clock::now();
	End = Begin;
}

BSR::Time::Timer::operator const float() const
{
	return std::chrono::duration<float>(End - Begin).count();
}

void BSR::Time::Timer::operator= (const Timer& _Other)
{
	Begin = _Other.Begin;
	End = _Other.End;
}

void BSR::Time::Timer::operator= (Timer&& _Other) noexcept
{
	Begin = std::move(_Other.Begin);
	End = std::move(_Other.End);
}



static TIMECAPS TimeCaps = { 0 };
static bool Inited = false;



bool BSR::Time::Init()
{
	if (Inited)
	{
		return true;
	}

	if (timeGetDevCaps(&TimeCaps, sizeof(TIMECAPS)) != MMSYSERR_NOERROR)
	{
		return false;
	}

	if (timeBeginPeriod(TimeCaps.wPeriodMin) != TIMERR_NOERROR)
	{
		TimeCaps = { 0 };
		return false;
	}

	Inited = true;

	return true;
}

void BSR::Time::Stop()
{
	if (!Inited)
	{
		return;
	}

	timeEndPeriod(TimeCaps.wPeriodMin);

	TimeCaps = { 0 };
	Inited = false;
}
