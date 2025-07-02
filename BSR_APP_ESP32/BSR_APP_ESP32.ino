#include "BSR_APP_ESP32.hpp"



void setup()
{
	BSR_APP_ESP32::RunTime::SetUp();
}

void loop()
{
	BSR_APP_ESP32::RunTime::Update();
}
