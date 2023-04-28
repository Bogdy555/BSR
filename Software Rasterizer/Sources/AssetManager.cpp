#include "..\Headers\BSR.hpp"



BSR::AssetManager::AssetManager() : Assets()
{

}

BSR::AssetManager::AssetManager(AssetManager&& _Other) noexcept : Assets(std::move(Assets))
{

}

BSR::AssetManager::~AssetManager()
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
}

bool BSR::AssetManager::AddAsset(void* _Data, const wchar_t* _Name)
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

void BSR::AssetManager::RemoveAsset(const wchar_t* _Name)
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

void BSR::AssetManager::RemoveAsset(const size_t _Index)
{
	delete[] Assets[_Index].Name;
	Assets.erase(Assets.begin() + _Index);
}

void BSR::AssetManager::RemoveAllAssets()
{
	for (size_t _Index = 0; _Index < Assets.size(); _Index++)
	{
		delete[] Assets[_Index].Name;
	}
	Assets.clear();
}

const size_t BSR::AssetManager::GetAssetsCount() const
{
	return Assets.size();
}

const size_t BSR::AssetManager::GetAssetIndex(const wchar_t* _Name) const
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

void* BSR::AssetManager::GetAssetData(const wchar_t* _Name)
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

const void* BSR::AssetManager::GetAssetData(const wchar_t* _Name) const
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

BSR::Asset& BSR::AssetManager::operator[] (const size_t _Index)
{
	return Assets[_Index];
}

const BSR::Asset& BSR::AssetManager::operator[] (const size_t _Index) const
{
	return Assets[_Index];
}

void BSR::AssetManager::operator= (AssetManager&& _Other) noexcept
{
	RemoveAllAssets();

	Assets = std::move(_Other.Assets);
}
