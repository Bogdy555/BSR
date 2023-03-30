#include "..\Headers\Main.hpp"



AssetManager::AssetManager() : Assets()
{

}

AssetManager::~AssetManager()
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
	Assets.Clear();
}

bool AssetManager::AddAsset(void* _Data, const wchar_t* _Name)
{
	if (!_Data || !_Name)
	{
		return false;
	}

	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		if (wcscmp(_Name, Assets[_Index].Name) == 0)
		{
			return false;
		}
	}

	wchar_t* _NameCpy = new wchar_t[wcslen(_Name) + 1];

	if (!_NameCpy)
	{
		return false;
	}

	wcscpy(_NameCpy, _Name);

	Asset _Asset;

	_Asset.Data = _Data;
	_Asset.Name = _NameCpy;

	Assets.PushBack(_Asset);

	return true;
}

void AssetManager::RemoveAsset(const wchar_t* _Name)
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		if (wcscmp(_Name, Assets[_Index].Name) == 0)
		{
			delete[] Assets[_Index].Name;
			Assets.Erase(_Index);
			return;
		}
	}
}

void AssetManager::RemoveAsset(const size_t _Index)
{
	delete[] Assets[_Index].Name;
	Assets.Erase(_Index);
}

void AssetManager::RemoveAllAssets()
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
	Assets.Clear();
}

const size_t AssetManager::GetAssetsCount() const
{
	return Assets.GetSize();
}

void* AssetManager::GetAssetData(const wchar_t* _Name)
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		if (wcscmp(_Name, Assets[_Index].Name) == 0)
		{
			return Assets[_Index].Data;
		}
	}

	return nullptr;
}

const void* AssetManager::GetAssetData(const wchar_t* _Name) const
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		if (wcscmp(_Name, Assets[_Index].Name) == 0)
		{
			return Assets[_Index].Data;
		}
	}

	return nullptr;
}

Asset& AssetManager::operator[] (const size_t _Index)
{
	return Assets[_Index];
}

const Asset& AssetManager::operator[] (const size_t _Index) const
{
	return Assets[_Index];
}

const size_t AssetManager::GetAssetIndex(const wchar_t* _Name) const
{
	for (size_t _Index = 0; _Index < Assets.GetSize(); _Index++)
	{
		if (wcscmp(_Name, Assets[_Index].Name) == 0)
		{
			return _Index;
		}
	}

	return (size_t)(-1);
}
