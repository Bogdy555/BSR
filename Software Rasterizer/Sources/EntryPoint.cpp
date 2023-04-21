#include "..\Headers\Main.hpp"



constexpr size_t Width = 1920;
constexpr size_t Height = 1080;



AssetManager SceneAssets;



void CleanUpAssets()
{
	delete (Rasterizer::Model*)(SceneAssets.GetAssetData(L"Model"));
	SceneAssets.RemoveAsset(L"Model");



	ASSERT(dynamic_cast<Rasterizer::Texture_Float_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Environment texture"))));
	delete (Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Environment texture"));
	SceneAssets.RemoveAsset(L"Environment texture");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum albedo"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum albedo"));
	SceneAssets.RemoveAsset(L"Aluminum albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum metalness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum metalness"));
	SceneAssets.RemoveAsset(L"Aluminum metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum normal"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum normal"));
	SceneAssets.RemoveAsset(L"Aluminum normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum roughness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum roughness"));
	SceneAssets.RemoveAsset(L"Aluminum roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container albedo"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container albedo"));
	SceneAssets.RemoveAsset(L"Container albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container ambient occlusion"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container ambient occlusion"));
	SceneAssets.RemoveAsset(L"Container ambient occlusion");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container metalness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container metalness"));
	SceneAssets.RemoveAsset(L"Container metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container normal"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container normal"));
	SceneAssets.RemoveAsset(L"Container normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container roughness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container roughness"));
	SceneAssets.RemoveAsset(L"Container roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold albedo"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold albedo"));
	SceneAssets.RemoveAsset(L"Gold albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold metalness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold metalness"));
	SceneAssets.RemoveAsset(L"Gold metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold normal"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold normal"));
	SceneAssets.RemoveAsset(L"Gold normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold roughness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold roughness"));
	SceneAssets.RemoveAsset(L"Gold roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron albedo"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron albedo"));
	SceneAssets.RemoveAsset(L"Iron albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron metalness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron metalness"));
	SceneAssets.RemoveAsset(L"Iron metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron normal"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron normal"));
	SceneAssets.RemoveAsset(L"Iron normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron roughness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron roughness"));
	SceneAssets.RemoveAsset(L"Iron roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic albedo green"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo green"));
	SceneAssets.RemoveAsset(L"Plastic albedo green");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic albedo red"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo red"));
	SceneAssets.RemoveAsset(L"Plastic albedo red");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"));
	SceneAssets.RemoveAsset(L"Plastic ambient occlusion");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic metalness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic metalness"));
	SceneAssets.RemoveAsset(L"Plastic metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic normal"))));
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic normal"));
	SceneAssets.RemoveAsset(L"Plastic normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic roughness"))));
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic roughness"));
	SceneAssets.RemoveAsset(L"Plastic roughness");



	ASSERT(SceneAssets.GetAssetsCount() == 0);
}



bool LoadAssets()
{
	{
		Rasterizer::Model* _Model = new Rasterizer::Model;

		if (!_Model)
		{
			CleanUpAssets();
			return false;
		}

		if (!_Model->Load(L".\\3D Models\\Objects.wfobj"))
		{
			delete _Model;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Model, L"Model"))
		{
			delete _Model;
			CleanUpAssets();
			return false;
		}
	}

	{
		Image::ImageFloat _Image;

		_Image.Data = Image::LoadHdr(L".\\Environments\\ParkingLot.hdr", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_Float_RGB* _Texture = new Rasterizer::Texture_Float_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Environment texture"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Aluminum\\Albedo.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Aluminum albedo"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Aluminum\\Metalness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Aluminum metalness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Aluminum\\Normal.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Aluminum normal"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Aluminum\\Roughness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Aluminum roughness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Container\\Albedo.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Container albedo"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Container\\AmbientOcclusion.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Container ambient occlusion"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Container\\Metalness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Container metalness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Container\\Normal.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Container normal"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Container\\Roughness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Container roughness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Gold\\Albedo.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Gold albedo"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Gold\\Metalness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Gold metalness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Gold\\Normal.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Gold normal"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Gold\\Roughness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Gold roughness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Iron\\Albedo.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Iron albedo"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Iron\\Metalness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Iron metalness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Iron\\Normal.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Iron normal"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Iron\\Roughness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Iron roughness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\Albedo Green.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic albedo green"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\Albedo Red.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic albedo red"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\AmbientOcclusion.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic ambient occlusion"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\Metalness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic metalness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\Normal.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RGB* _Texture = new Rasterizer::Texture_RGB;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic normal"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Materials\\Plastic\\Roughness.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"Plastic roughness"))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		delete[] _Image.Data;
	}

	return true;
}



struct FrameBuffer
{
	Image::Image* ColorBuffer = nullptr;
	Image::ImageFloat DepthBuffer;
};

struct CubeMapUniforms
{
	Math::Mat4f Mvp;
	Rasterizer::Texture_Float_RGB* EnvironmentTexture = nullptr;
	float Exposure = 1.0f;
};

struct CubeMapLerpers
{
	Math::Vec3f Position;
};

Math::Vec2f SampleEquirectangularMap(Math::Vec3f _Dir)
{
	return Math::Vec2f(atan2(_Dir.z, _Dir.x), asin(_Dir.y)) * Math::Vec2f(0.1591f, 0.3183f) + Math::Vec2f(0.5f, 0.5f);
}

const Math::Vec4f CubeMapVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const Rasterizer::VertexData* _VertexData = (const Rasterizer::VertexData*)(_Vertex);

	const CubeMapUniforms* _CubeMapUniforms = (const CubeMapUniforms*)(_Uniforms);

	CubeMapLerpers* _Lerpers = (CubeMapLerpers*)(_OutLerpers);

	_Lerpers->Position = _VertexData->Position;

	return _CubeMapUniforms->Mvp * Math::Vec4f(_VertexData->Position, 1.0f);
}

void CubeMapFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const CubeMapLerpers* _CubeMapLerpers = (const CubeMapLerpers*)(_Lerpers);

	const CubeMapUniforms* _CubeMapUniforms = (const CubeMapUniforms*)(_Uniforms);

	FrameBuffer* _TrueFrameBuffer = (FrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer->DepthBuffer.Data[_X + _Y * _TrueFrameBuffer->DepthBuffer.Width] = 1.0f;

	Math::Vec3f _Color = _CubeMapUniforms->EnvironmentTexture->SampleRGB(SampleEquirectangularMap(_CubeMapLerpers->Position.Normalized()));

	_Color = Math::Vec3f(1.0f, 1.0f, 1.0f) - Math::Vec3f::Exp(-_Color * _CubeMapUniforms->Exposure);

	_Color = Math::Vec3f::Pow(_Color, Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 0] = (uint8_t)(_Color.x * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 1] = (uint8_t)(_Color.y * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 2] = (uint8_t)(_Color.z * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 3] = 255;
}

struct PBRUniforms
{
	Math::Mat4f Mvp;
	Math::Mat4f Model;
	Math::Mat4f View;
	Math::Mat4f Projection;
	Math::Mat4f ModelInversedTransposed;

	Rasterizer::Camera Camera;
	Math::Vec3f CameraForwardVector;

	Rasterizer::Transform Transform;

	//Material
	//Iradiance and radiance map
	//Lights vector
	//LUT texture
};

struct PBRLerpers
{
	Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
	Math::Vec3f Normal = Math::Vec3f(0.0f, 0.0f, 1.0f);
	Math::Vec3f Tangent = Math::Vec3f(1.0f, 0.0f, 0.0f);
	Math::Vec2f TextureCoords = Math::Vec2f(0.0f, 0.0f);
};

const Math::Vec4f PBRVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const Rasterizer::VertexData* _VertexData = (const Rasterizer::VertexData*)(_Vertex);

	const PBRUniforms* _PBRUniforms = (const PBRUniforms*)(_Uniforms);

	PBRLerpers* _Lerpers = (PBRLerpers*)(_OutLerpers);

	_Lerpers->Position = Math::Vec3f(_PBRUniforms->Model * Math::Vec4f(_VertexData->Position, 1.0f));
	_Lerpers->Normal = Math::Vec3f(_PBRUniforms->ModelInversedTransposed * Math::Vec4f(_VertexData->Normal, 1.0f));
	_Lerpers->Tangent = Math::Vec3f(_PBRUniforms->ModelInversedTransposed * Math::Vec4f(_VertexData->Tangent, 1.0f));
	_Lerpers->TextureCoords = _VertexData->TextureCoords;

	return _PBRUniforms->Mvp * Math::Vec4f(_VertexData->Position, 1.0f);
}

void PBRFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRLerpers* _PBRLerpers = (const PBRLerpers*)(_Lerpers);

	const PBRUniforms* _PBRUniforms = (const PBRUniforms*)(_Uniforms);

	FrameBuffer* _TrueFrameBuffer = (FrameBuffer*)(_FrameBuffer);

	if (!Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer->DepthBuffer.Data[_X + _Y * _TrueFrameBuffer->DepthBuffer.Width], _DepthTestingType))
	{
		return;
	}

	_TrueFrameBuffer->DepthBuffer.Data[_X + _Y * _TrueFrameBuffer->DepthBuffer.Width] = _FragCoord.z;

	Math::Vec4f _Color = Math::Vec4f(Math::Vec3f::Dot(Math::Vec3f(0.0f, 0.0f, 1.0f), _PBRLerpers->Normal.Normalized()), Math::Vec3f::Dot(Math::Vec3f(0.0f, 0.0f, 1.0f), _PBRLerpers->Normal.Normalized()), Math::Vec3f::Dot(Math::Vec3f(0.0f, 0.0f, 1.0f), _PBRLerpers->Normal.Normalized()), 1.0f);

	_Color = Math::Vec4f(Rasterizer::Context::Blend(Math::Vec3f(0.0f, 0.0f, 0.0f), _Color, _BlendingType), 1.0f);

	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 0] = (uint8_t)(_Color.x * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 1] = (uint8_t)(_Color.y * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 2] = (uint8_t)(_Color.z * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 3] = 255;
}

bool RenderScene(Image::Image& _RenderResult)
{
	if (!_RenderResult.Width || !_RenderResult.Height)
	{
		return false;
	}

	FrameBuffer _FrameBuffer;

	_FrameBuffer.ColorBuffer = &_RenderResult;

	_FrameBuffer.DepthBuffer.Width = _FrameBuffer.ColorBuffer->Width;
	_FrameBuffer.DepthBuffer.Height = _FrameBuffer.ColorBuffer->Height;
	_FrameBuffer.DepthBuffer.Data = new float[_FrameBuffer.DepthBuffer.Width * _FrameBuffer.DepthBuffer.Height];

	if (!_FrameBuffer.DepthBuffer.Data)
	{
		return false;
	}

	float _Exposure = 1.0f;

	Rasterizer::Camera _Camera;

	Rasterizer::Transform _Transform;

	_Transform.Position = Math::Vec3f(0.0f, 0.0f, -3.0f);

	Rasterizer::Context _Context;

	_Context.SetViewPort(0, 0, _RenderResult.Width, _RenderResult.Height);

	float _AspectRatio = (float)(_RenderResult.Width) / (float)(_RenderResult.Height);

	{
		Rasterizer::Camera _CubeMapCamera = _Camera;

		_CubeMapCamera.Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
		_CubeMapCamera.NearPlane = 0.1f;
		_CubeMapCamera.FarPlane = 2.0f;

		Rasterizer::Mesh _Cube;

		Rasterizer::Mesh::GenerateCube(_Cube);

		CubeMapUniforms _Uniforms;

		_Uniforms.Mvp = _CubeMapCamera.GetProjectionMatrix(_AspectRatio) * _CubeMapCamera.GetViewMatrix();
		_Uniforms.EnvironmentTexture = (Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Environment texture"));
		_Uniforms.Exposure = _Exposure;

		_Context.SetCullingType(Rasterizer::_CounterClockWiseCulling);

		if (!_Context.RenderMesh(_Cube.VBO.GetData(), _Cube.VBO.GetSize(), sizeof(Rasterizer::VertexData), _Cube.IBO.GetData(), 0, _Cube.IBO.GetSize() * 3, &_Uniforms, sizeof(CubeMapLerpers) / sizeof(float), sizeof(CubeMapLerpers) / sizeof(float), CubeMapVertexShader, nullptr, CubeMapFragmentShader, &_FrameBuffer))
		{
			delete[] _FrameBuffer.DepthBuffer.Data;
			return false;
		}

		_Context.SetCullingType(Rasterizer::_ClockWiseCulling);
	}

	Rasterizer::Model* _Model = (Rasterizer::Model*)(SceneAssets.GetAssetData(L"Model"));

	if (!_Model)
	{
		delete[] _FrameBuffer.DepthBuffer.Data;
		return false;
	}

	if (_Model->GetSize() != 8)
	{
		delete[] _FrameBuffer.DepthBuffer.Data;
		return false;
	}

	for (size_t _Index = 0; _Index < 8; _Index++)
	{
		const Rasterizer::Mesh& _Mesh = _Model->operator[](_Index);

		PBRUniforms _Uniforms;

		_Uniforms.Model = _Transform.GetModelMatrix();
		_Uniforms.View = _Camera.GetViewMatrix();
		_Uniforms.Projection = _Camera.GetProjectionMatrix(_AspectRatio);
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;
		_Uniforms.ModelInversedTransposed = _Uniforms.Model.Inversed().Transposed();
		_Uniforms.Camera = _Camera;
		_Uniforms.CameraForwardVector = _Camera.GetForwardVector();
		_Uniforms.Transform = _Transform;

		if (!_Context.RenderMesh(_Mesh.VBO.GetData(), _Mesh.VBO.GetSize(), sizeof(Rasterizer::VertexData), _Mesh.IBO.GetData(), 0, _Mesh.IBO.GetSize() * 3, &_Uniforms, sizeof(PBRLerpers) / sizeof(float), sizeof(PBRLerpers) / sizeof(float), PBRVertexShader, nullptr, PBRFragmentShader, &_FrameBuffer))
		{
			delete[] _FrameBuffer.DepthBuffer.Data;
			return false;
		}
	}

	delete[] _FrameBuffer.DepthBuffer.Data;

	return true;
}



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	LOG_LINE(STRING_TYPE("Loading the assets"));

	if (!LoadAssets())
	{
		LOG_LINE(STRING_TYPE("Failed"));
		return -1;
	}

	LOG_LINE(STRING_TYPE("Rendering the scene"));

	Image::Image _RenderResult;

	_RenderResult.Width = Width;
	_RenderResult.Height = Height;
	_RenderResult.Data = new uint8_t[_RenderResult.Width * _RenderResult.Height * 4];

	if (!_RenderResult.Data)
	{
		LOG_LINE(STRING_TYPE("Failed"));
		CleanUpAssets();
		return -1;
	}

	if (!RenderScene(_RenderResult))
	{
		LOG_LINE(STRING_TYPE("Failed"));
		delete[] _RenderResult.Data;
		CleanUpAssets();
		return -1;
	}

	LOG_LINE(STRING_TYPE("Saving the result"));

	if (!Image::SaveBmp(L".\\Render.bmp", _RenderResult.Data, _RenderResult.Width, _RenderResult.Height))
	{
		LOG_LINE(STRING_TYPE("Failed"));
		delete[] _RenderResult.Data;
		CleanUpAssets();
		return -1;
	}

	delete[] _RenderResult.Data;

	CleanUpAssets();

	ShellExecute(NULL, nullptr, L".\\Render.bmp", nullptr, nullptr, SW_SHOW);

	return 0;
}
