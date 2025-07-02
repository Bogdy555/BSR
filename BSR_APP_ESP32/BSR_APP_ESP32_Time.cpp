#include "BSR_APP_ESP32.hpp"



BSR_APP_ESP32::Time::Timer::Timer() : Begin(), End()
{
	Begin = (size_t)(millis());
	End = Begin;
}

BSR_APP_ESP32::Time::Timer::Timer(const Timer& _Other) : Begin(_Other.Begin), End(_Other.End)
{

}

BSR_APP_ESP32::Time::Timer::Timer(Timer&& _Other) noexcept : Begin(_Other.Begin), End(_Other.End)
{
	_Other.Begin = (size_t)(millis());
	_Other.End = _Other.Begin;
}

BSR_APP_ESP32::Time::Timer::~Timer()
{

}

void BSR_APP_ESP32::Time::Timer::Start()
{
	Begin = (size_t)(millis());
	End = Begin;
}

void BSR_APP_ESP32::Time::Timer::Stop()
{
	End = (size_t)(millis());
}

const float BSR_APP_ESP32::Time::Timer::GetTime() const
{
	return (float)(End - Begin) / 1000.0f;
}

BSR_APP_ESP32::Time::Timer::operator const float () const
{
	return (float)(End - Begin) / 1000.0f;
}

BSR_APP_ESP32::Time::Timer& BSR_APP_ESP32::Time::Timer::operator= (const Timer& _Other)
{
	Begin = _Other.Begin;
	End = _Other.End;

	return *this;
}

BSR_APP_ESP32::Time::Timer& BSR_APP_ESP32::Time::Timer::operator= (Timer&& _Other) noexcept
{
	Begin = _Other.Begin;
	End = _Other.End;

	_Other.Begin = (size_t)(millis());
	_Other.End = _Other.Begin;

	return *this;
}
