#include "../Headers/BSR_APP.hpp"



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	if (CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED) != S_OK)
	{
		MessageBox(NULL, BSR_STRING_TYPE("Couldn't initialize multithreading!"), BSR_STRING_TYPE("Error!"), MB_OK | MB_ICONERROR);
		return BSR::MultiProcessing::_ReturnError;
	}

	if (!BSR::Time::Init())
	{
		MessageBox(NULL, BSR_STRING_TYPE("Couldn't initialize timer!"), BSR_STRING_TYPE("Error!"), MB_OK | MB_ICONERROR);
		CoUninitialize();
		return BSR::MultiProcessing::_ReturnError;
	}

	BSR_APP::RunTime::Application* _Application = BSR_APP::RunTime::Application::GetInstance();

	int32_t _ReturnValue = _Application->Run(_hInstance, _CmdLine, _ShowCmd, L"BSR_APP_SharedInstanceMemory", L"BSR_APP_SharedInstanceMutex");

	if (_ReturnValue != BSR::MultiProcessing::_ReturnNoError)
	{
		MessageBox(NULL, BSR_STRING_TYPE("BSR returned error!"), BSR_STRING_TYPE("Error!"), MB_OK | MB_ICONERROR);
	}

	BSR::Time::Stop();

	CoUninitialize();

	return _ReturnValue;
}
