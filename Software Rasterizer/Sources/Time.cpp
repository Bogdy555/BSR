#include "..\Headers\Main.hpp"



Time::Timer::Timer() : Begin(), End()
{

}

Time::Timer::Timer(const Timer& _Other) : Begin(_Other.Begin), End(_Other.End)
{

}

Time::Timer::Timer(Timer&& _Other) noexcept : Begin(std::move(_Other.Begin)), End(std::move(_Other.End))
{

}

Time::Timer::~Timer()
{

}

void Time::Timer::Start()
{
	Begin = std::chrono::system_clock::now();
}

void Time::Timer::Stop()
{
	End = std::chrono::system_clock::now();
}

void Time::Timer::Restart()
{
	Begin = std::chrono::system_clock::now();
	End = Begin;
}

Time::Timer::operator const float() const
{
	return std::chrono::duration<float>(End - Begin).count();
}

void Time::Timer::operator= (const Timer& _Other)
{
	Begin = _Other.Begin;
	End = _Other.End;
}

void Time::Timer::operator= (Timer&& _Other) noexcept
{
	Begin = std::move(_Other.Begin);
	End = std::move(_Other.End);
}



static TIMECAPS TimeCaps = { 0 };
static bool Inited = false;



bool Time::Init()
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

void Time::Stop()
{
	if (!Inited)
	{
		return;
	}

	timeEndPeriod(TimeCaps.wPeriodMin);

	TimeCaps = { 0 };
	Inited = false;
}
