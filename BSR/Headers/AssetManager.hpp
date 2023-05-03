#ifndef BSR_AssetManager_hpp

#define BSR_AssetManager_hpp



#include "BSR.hpp"



namespace BSR
{

	struct Asset
	{
		void* Data = nullptr;
		const wchar_t* Name = nullptr;
	};

	class AssetManager
	{

	public:

		AssetManager();
		AssetManager(const AssetManager& _Other) = delete;
		AssetManager(AssetManager&& _Other) noexcept;
		~AssetManager();

		bool AddAsset(void* _Data, const wchar_t* _Name);

		void RemoveAsset(const wchar_t* _Name);
		void RemoveAsset(const size_t _Index);
		void RemoveAllAssets();

		const size_t GetAssetsCount() const;
		const size_t GetAssetIndex(const wchar_t* _Name) const;

		void* GetAssetData(const wchar_t* _Name);
		const void* GetAssetData(const wchar_t* _Name) const;

		Asset& operator[] (const size_t _Index);
		const Asset& operator[] (const size_t _Index) const;

		void operator= (const AssetManager& _Other) = delete;
		void operator= (AssetManager&& _Other) noexcept;

	private:

		std::vector<Asset> Assets;

	};

}



#endif
