#include "..\Headers\Main.hpp"



AssetManager::AssetManager() : Assets()
{

}

AssetManager::AssetManager(AssetManager&& _Other) noexcept : Assets(std::move(Assets))
{

}

AssetManager::~AssetManager()
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
	Assets.clear();
}

bool AssetManager::AddAsset(void* _Data, const wchar_t* _Name)
{
	if (!_Data || !_Name)
	{
		return false;
	}

	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		if (String::TheSame(_Name, Assets[_Index].Name))
		{
			return false;
		}
	}

	wchar_t* _NameCpy = new wchar_t[String::Length(_Name) + 1];

	if (!_NameCpy)
	{
		return false;
	}

	wcscpy_s(_NameCpy, String::Length(_Name) + 1, _Name);

	Asset _Asset;

	_Asset.Data = _Data;
	_Asset.Name = _NameCpy;

	Assets.push_back(_Asset);

	return true;
}

void AssetManager::RemoveAsset(const wchar_t* _Name)
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		if (String::TheSame(_Name, Assets[_Index].Name))
		{
			delete[] Assets[_Index].Name;
			Assets.erase(Assets.begin() + _Index);
			return;
		}
	}
}

void AssetManager::RemoveAsset(const size_t _Index)
{
	delete[] Assets[_Index].Name;
	Assets.erase(Assets.begin() + _Index);
}

void AssetManager::RemoveAllAssets()
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
	Assets.clear();
}

const size_t AssetManager::GetAssetsCount() const
{
	return Assets.size();
}

const size_t AssetManager::GetAssetIndex(const wchar_t* _Name) const
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		if (String::TheSame(_Name, Assets[_Index].Name))
		{
			return _Index;
		}
	}

	return (size_t)(-1);
}

void* AssetManager::GetAssetData(const wchar_t* _Name)
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		if (String::TheSame(_Name, Assets[_Index].Name))
		{
			return Assets[_Index].Data;
		}
	}

	return nullptr;
}

const void* AssetManager::GetAssetData(const wchar_t* _Name) const
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		if (String::TheSame(_Name, Assets[_Index].Name))
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

void AssetManager::operator= (AssetManager&& _Other) noexcept
{
	Assets = std::move(_Other.Assets);
}
