#ifndef AssetManager_hpp

#define AssetManager_hpp



#include "Main.hpp"



struct Asset
{
	void* Data;
	const wchar_t* Name;
};

class AssetManager
{

public:

	AssetManager();
	~AssetManager();

	bool AddAsset(void* _Data, const wchar_t* _Name);

	void RemoveAsset(const wchar_t* _Name);
	void RemoveAsset(const size_t _Index);
	void RemoveAllAssets();

	const size_t GetAssetsCount() const;

	void* GetAssetData(const wchar_t* _Name);
	const void* GetAssetData(const wchar_t* _Name) const;

	Asset& operator[] (const size_t _Index);
	const Asset& operator[] (const size_t _Index) const;

	const size_t GetAssetIndex(const wchar_t* _Name) const;

private:

	Vector<Asset> Assets;

};



#endif
