#include "..\Headers\Main.hpp"



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	Rasterizer::Model _Model;

	if (!_Model.Load(L".\\Cubes.wfobj"))
	{
		std::wcout << L"Could not load the 3D model!\n";
		return -1;
	}

	std::wcout << L"The 3D model was loaded.\n";

	if (!_Model.Save(L".\\Cubes.obj"))
	{
		std::wcout << L"Could not save the 3D model!\n";
		return -1;
	}

	std::wcout << L"The 3D model was saved.\n";

	return 0;
}
