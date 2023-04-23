#include "..\Headers\Main.hpp"



constexpr size_t Width = 1920;
constexpr size_t Height = 1080;



#define MAX_LOD_ROUGHNESS 4

const Math::Vec2f SampleEquirectangularMap(const Math::Vec3f& _Dir)
{
	return Math::Vec2f(atan2(_Dir.z, _Dir.x), asin(_Dir.y)) * Math::Vec2f(0.1591f, 0.3183f) + Math::Vec2f(0.5f, 0.5f);
}

const Math::Vec3f ReverseSampleEquirectangularMap(const Math::Vec2f& _UV)
{
	Math::Vec2f _NewUV = (_UV - Math::Vec2f(0.5f, 0.5f)) / Math::Vec2f(0.1591f, 0.3183f);
	return Math::Vec3f(cosf(_NewUV.x), sinf(_NewUV.y), sinf(_NewUV.x)).Normalized();
}

const Math::Vec3f SampleNormal(const Rasterizer::Material& _Material, const Math::Vec2f& _TextureCoords, const Math::Vec3f& _TrueNormal, const Math::Vec3f& _TrueTangent)
{
	if (_TrueTangent == Math::Vec3f(0.0f, 0.0f, 0.0f))
	{
		return _TrueNormal.Normalized();
	}

	if (_Material.NormalMap == nullptr)
	{
		return _TrueNormal.Normalized();
	}

	Math::Vec3f _N = _TrueNormal.Normalized();
	Math::Vec3f _T = _TrueTangent.Normalized();
	_T = (_T - _N * Math::Vec3f::Dot(_N, _T)).Normalized();
	Math::Vec3f _B = Math::Vec3f::Cross(_N, _T);

	Math::Mat3f _TBN;

	_TBN[0][0] = _T.x; _TBN[0][1] = _B.x; _TBN[0][2] = _N.x;
	_TBN[1][0] = _T.y; _TBN[1][1] = _B.y; _TBN[1][2] = _N.y;
	_TBN[2][0] = _T.z; _TBN[2][1] = _B.z; _TBN[2][2] = _N.z;

	Math::Vec3f _NMap = _Material.NormalMap->SampleRGB(_TextureCoords) * _Material.NormalMapMultiplier * 2.0f - 1.0f;

	return (_TBN * _NMap).Normalized();
}

const float NormalDistributionFunctionGGX(const Math::Vec3f& _Normal, const Math::Vec3f& _HalfWayVec, const float _Roughness)
{
	float _AlphaSquared = _Roughness * _Roughness;
	_AlphaSquared *= _AlphaSquared;

	float _DotNormalHalfWayVecSquare = Math::Max(Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f);
	_DotNormalHalfWayVecSquare *= _DotNormalHalfWayVecSquare;

	float _Numerator = _AlphaSquared;
	float _Denominator = _DotNormalHalfWayVecSquare * (_AlphaSquared - 1.0f) + 1.0f;
	_Denominator = Math::fPi * _Denominator * _Denominator;

	return _Numerator / _Denominator;
}

const float GeometrySchlickGGX(const Math::Vec3f& _Normal, const Math::Vec3f& _Vec, float _Roughness)
{
	float _K = _Roughness + 1.0f;
	_K *= _K / 8.0f;

	float _Numerator = Math::Max(Math::Vec3f::Dot(_Normal, _Vec), 0.0f);
	float _Denominator = _Numerator * (1.0f - _K) + _K;

	return _Numerator / _Denominator;
}

const float GeometrySmith(const Math::Vec3f& _Normal, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _PositionToLight, const float _Roughness)
{
	return GeometrySchlickGGX(_Normal, _PositionToCamera, _Roughness) * GeometrySchlickGGX(_Normal, _PositionToLight, _Roughness);
}

const Math::Vec3f FresnelSchlick(const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _HalfWayVec, const Math::Vec3f& _BaseReflectivity)
{
	return _BaseReflectivity + (Math::Vec3f(1.0f, 1.0f, 1.0f) - _BaseReflectivity) * powf(Math::Clamp(1.0f - Math::Max(Math::Vec3f::Dot(_PositionToCamera, _HalfWayVec), 0.0f), 0.0f, 1.0f), 5.0f);
}

const Math::Vec3f FresnelSchlickRoughness(const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _HalfWayVec, const Math::Vec3f& _BaseReflectivity, const float _Roughness)
{
	return _BaseReflectivity + (Math::Vec3f::Max(Math::Vec3f(1.0f - _Roughness, 1.0f - _Roughness, 1.0f - _Roughness), _BaseReflectivity) - _BaseReflectivity) * powf(Math::Clamp(1.0f - Math::Max(Math::Vec3f::Dot(_PositionToCamera, _HalfWayVec), 0.0f), 0.0f, 1.0f), 5.0f);
}

const Math::Vec3f DirectionalLightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

const Math::Vec3f PointLightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

const Math::Vec3f SpotLightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}

const Math::Vec3f ImageBasedLightCalculation(const Rasterizer::Texture_Float_RGB& _EnvironmentTexture, const Rasterizer::Texture_Float_RGB& _IradianceTexture, const Rasterizer::Texture_RG& _BRDFLookUpTexture, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	Math::Vec3f _FresnelSchlickRoughness = FresnelSchlickRoughness(_Normal, _PositionToCamera, _BaseReflectivity, _Roughness);
	Math::Vec3f _kDiffuse = (Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlickRoughness) * (1.0f - _Metalness);

	Math::Vec3f _Iradiance = _IradianceTexture.SampleRGB(SampleEquirectangularMap(_Normal));
	Math::Vec3f _Environment = _EnvironmentTexture.SampleRGB(SampleEquirectangularMap(Math::Vec3f::Reflect(-_PositionToCamera, _Normal)), _Roughness * (float)(MAX_LOD_ROUGHNESS));
	Math::Vec2f _BRDFLookUp = _BRDFLookUpTexture.SampleRG(Math::Vec2f(Math::Max(Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f), _Roughness));

	Math::Vec3f _Diffuse = _kDiffuse * _Iradiance * _Albedo;
	Math::Vec3f _Specular = _Environment * (_FresnelSchlickRoughness * _BRDFLookUp.x + _BRDFLookUp.y);

	return (_Diffuse + _Specular) * _AmbientOcclusion;
}

const Math::Vec3f LightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	switch (_Light.Type)
	{
	case Rasterizer::_DirectionalLight:
	{
		return DirectionalLightCalculation(_Light, _PositionToCamera, _Position, _Normal, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _BaseReflectivity);
	}
	case Rasterizer::_PointLight:
	{
		return PointLightCalculation(_Light, _PositionToCamera, _Position, _Normal, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _BaseReflectivity);
	}
	case Rasterizer::_SpotLight:
	{
		return SpotLightCalculation(_Light, _PositionToCamera, _Position, _Normal, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _BaseReflectivity);
	}
	}

	return Math::Vec3f(0.0f, 0.0f, 0.0f);
}



AssetManager SceneAssets;

void CleanUpAssets()
{
	delete (Rasterizer::Model*)(SceneAssets.GetAssetData(L"Model"));
	SceneAssets.RemoveAsset(L"Model");



	ASSERT(dynamic_cast<Rasterizer::Texture_Float_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Environment texture"))) || SceneAssets.GetAssetData(L"Environment texture") == nullptr);
	delete (Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Environment texture"));
	SceneAssets.RemoveAsset(L"Environment texture");

	ASSERT(dynamic_cast<Rasterizer::Texture_Float_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iradiance texture"))) || SceneAssets.GetAssetData(L"Iradiance texture") == nullptr);
	delete (Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Iradiance texture"));
	SceneAssets.RemoveAsset(L"Iradiance texture");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum albedo"))) || SceneAssets.GetAssetData(L"Aluminum albedo") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum albedo"));
	SceneAssets.RemoveAsset(L"Aluminum albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum metalness"))) || SceneAssets.GetAssetData(L"Aluminum metalness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum metalness"));
	SceneAssets.RemoveAsset(L"Aluminum metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum normal"))) || SceneAssets.GetAssetData(L"Aluminum normal") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum normal"));
	SceneAssets.RemoveAsset(L"Aluminum normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Aluminum roughness"))) || SceneAssets.GetAssetData(L"Aluminum roughness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum roughness"));
	SceneAssets.RemoveAsset(L"Aluminum roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container albedo"))) || SceneAssets.GetAssetData(L"Container albedo") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container albedo"));
	SceneAssets.RemoveAsset(L"Container albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container ambient occlusion"))) || SceneAssets.GetAssetData(L"Container ambient occlusion") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container ambient occlusion"));
	SceneAssets.RemoveAsset(L"Container ambient occlusion");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container metalness"))) || SceneAssets.GetAssetData(L"Container metalness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container metalness"));
	SceneAssets.RemoveAsset(L"Container metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container normal"))) || SceneAssets.GetAssetData(L"Container normal") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container normal"));
	SceneAssets.RemoveAsset(L"Container normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Container roughness"))) || SceneAssets.GetAssetData(L"Container roughness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container roughness"));
	SceneAssets.RemoveAsset(L"Container roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold albedo"))) || SceneAssets.GetAssetData(L"Gold albedo") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold albedo"));
	SceneAssets.RemoveAsset(L"Gold albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold metalness"))) || SceneAssets.GetAssetData(L"Gold metalness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold metalness"));
	SceneAssets.RemoveAsset(L"Gold metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold normal"))) || SceneAssets.GetAssetData(L"Gold normal") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold normal"));
	SceneAssets.RemoveAsset(L"Gold normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Gold roughness"))) || SceneAssets.GetAssetData(L"Gold roughness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold roughness"));
	SceneAssets.RemoveAsset(L"Gold roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron albedo"))) || SceneAssets.GetAssetData(L"Iron albedo") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron albedo"));
	SceneAssets.RemoveAsset(L"Iron albedo");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron metalness"))) || SceneAssets.GetAssetData(L"Iron metalness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron metalness"));
	SceneAssets.RemoveAsset(L"Iron metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron normal"))) || SceneAssets.GetAssetData(L"Iron normal") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron normal"));
	SceneAssets.RemoveAsset(L"Iron normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Iron roughness"))) || SceneAssets.GetAssetData(L"Iron roughness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron roughness"));
	SceneAssets.RemoveAsset(L"Iron roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic albedo green"))) || SceneAssets.GetAssetData(L"Plastic albedo green") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo green"));
	SceneAssets.RemoveAsset(L"Plastic albedo green");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic albedo red"))) || SceneAssets.GetAssetData(L"Plastic albedo red") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo red"));
	SceneAssets.RemoveAsset(L"Plastic albedo red");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"))) || SceneAssets.GetAssetData(L"Plastic ambient occlusion") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"));
	SceneAssets.RemoveAsset(L"Plastic ambient occlusion");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic metalness"))) || SceneAssets.GetAssetData(L"Plastic metalness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic metalness"));
	SceneAssets.RemoveAsset(L"Plastic metalness");

	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic normal"))) || SceneAssets.GetAssetData(L"Plastic normal") == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic normal"));
	SceneAssets.RemoveAsset(L"Plastic normal");

	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"Plastic roughness"))) || SceneAssets.GetAssetData(L"Plastic roughness") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic roughness"));
	SceneAssets.RemoveAsset(L"Plastic roughness");



	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"White Texture_R"))) || SceneAssets.GetAssetData(L"White Texture_R") == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"White Texture_R"));
	SceneAssets.RemoveAsset(L"White Texture_R");



	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 0"));
	SceneAssets.RemoveAsset(L"Material 0");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 1"));
	SceneAssets.RemoveAsset(L"Material 1");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 2"));
	SceneAssets.RemoveAsset(L"Material 2");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 3"));
	SceneAssets.RemoveAsset(L"Material 3");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 4"));
	SceneAssets.RemoveAsset(L"Material 4");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 5"));
	SceneAssets.RemoveAsset(L"Material 5");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 6"));
	SceneAssets.RemoveAsset(L"Material 6");

	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(L"Material 7"));
	SceneAssets.RemoveAsset(L"Material 7");



	ASSERT(dynamic_cast<Rasterizer::Texture_RG*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(L"BRDF lookup"))) || SceneAssets.GetAssetData(L"BRDF lookup") == nullptr);
	delete (Rasterizer::Texture_RG*)(SceneAssets.GetAssetData(L"BRDF lookup"));
	SceneAssets.RemoveAsset(L"BRDF lookup");



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

		_Image.Data = Image::LoadHdr(L".\\Environments\\ParkingLot\\Environment.hdr", _Image.Width, _Image.Height);

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
		Image::ImageFloat _Image;

		_Image.Data = Image::LoadHdr(L".\\Environments\\ParkingLot\\Iradiance.hdr", _Image.Width, _Image.Height);

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

		if (!SceneAssets.AddAsset(_Texture, L"Iradiance texture"))
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

	{
		uint32_t _White = 0xFFFFFFFF;

		Image::Image _Image;

		_Image.Width = 1;
		_Image.Height = 1;
		_Image.Data = (uint8_t*)(&_White);

		Rasterizer::Texture_R* _Texture = new Rasterizer::Texture_R;

		if (!_Texture)
		{
			CleanUpAssets();
			return false;
		}

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"White Texture_R"))
		{
			delete _Texture;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container ambient occlusion"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 0"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Gold roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"White Texture_R"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Gold normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 1"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Aluminum roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"White Texture_R"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Aluminum normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 2"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo green"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 3"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Iron roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"White Texture_R"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Iron normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 4"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic albedo red"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Plastic ambient occlusion"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Plastic normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 5"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container ambient occlusion"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 6"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Rasterizer::Material* _Material = new Rasterizer::Material;

		if (!_Material)
		{
			CleanUpAssets();
			return false;
		}

		_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container albedo"));
		_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container metalness"));
		_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container roughness"));
		_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(L"Container ambient occlusion"));
		_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(L"Container normal"));

		if (!SceneAssets.AddAsset(_Material, L"Material 7"))
		{
			delete _Material;
			CleanUpAssets();
			return false;
		}
	}

	{
		Image::Image _Image;

		_Image.Data = Image::LoadBmp(L".\\Intern\\BRDFLookUp.bmp", _Image.Width, _Image.Height);

		if (!_Image.Data)
		{
			CleanUpAssets();
			return false;
		}

		Rasterizer::Texture_RG* _Texture = new Rasterizer::Texture_RG;

		if (!_Texture)
		{
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		_Texture->SetWrapType(Rasterizer::_WrapClamp);

		if (!_Texture->AddMip(_Image))
		{
			delete _Texture;
			delete[] _Image.Data;
			CleanUpAssets();
			return false;
		}

		if (!SceneAssets.AddAsset(_Texture, L"BRDF lookup"))
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
	const Rasterizer::Texture_Float_RGB* EnvironmentTexture = nullptr;
	float Exposure = 1.0f;
};

struct CubeMapLerpers
{
	Math::Vec3f Position;
};

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
	float Exposure = 1.0f;

	Math::Mat4f Mvp;
	Math::Mat4f Model;
	Math::Mat4f View;
	Math::Mat4f Projection;
	Math::Mat4f ModelInversedTransposed;

	Rasterizer::Camera Camera;
	Math::Vec3f CameraForwardVector;

	Rasterizer::Transform Transform;

	Rasterizer::Material Material;

	const Rasterizer::Texture_Float_RGB* Environment = nullptr;
	const Rasterizer::Texture_Float_RGB* Iradiance = nullptr;
	const Rasterizer::Texture_RG* BRDFLookUp = nullptr;

	//Lights vector
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



	Math::Vec3f _Albedo = Math::Vec3f::Pow(_PBRUniforms->Material.Albedo->SampleRGB(_PBRLerpers->TextureCoords) * _PBRUniforms->Material.AlbedoMultiplier, Math::Vec3f(2.2f, 2.2f, 2.2f));
	Math::Vec3f _Normal = SampleNormal(_PBRUniforms->Material, _PBRLerpers->TextureCoords, _PBRLerpers->Normal, _PBRLerpers->Tangent);
	float _Metalness = _PBRUniforms->Material.Metalness->SampleR(_PBRLerpers->TextureCoords) * _PBRUniforms->Material.MetalnessMultiplier;
	float _Roughness = _PBRUniforms->Material.Roughness->SampleR(_PBRLerpers->TextureCoords) * _PBRUniforms->Material.RoughnessMultiplier;
	float _AmbientOcclusion = _PBRUniforms->Material.AmbientOcclusion->SampleR(_PBRLerpers->TextureCoords) * _PBRUniforms->Material.AmbientOcclusionMultiplier;
	Math::Vec3f _BaseReflectivity = Math::Vec3f::Mix(Math::Vec3f(0.04f, 0.04f, 0.04f), _Albedo, _Metalness);
	Math::Vec3f _Position = _PBRLerpers->Position;
	Math::Vec3f _PositionToCamera = _PBRUniforms->Camera.Perspective ? (_PBRUniforms->Camera.Position - _Position).Normalized() : -_PBRUniforms->CameraForwardVector;



	Math::Vec3f _Result = Math::Vec3f(0.0f, 0.0f, 0.0f);

	//Math::Vec3f _Result = Math::Vec3f(0.0f, 0.0f, 0.0f);

	//for ()
	//{
	//	_Result += LightCalculation(, _PositionToCamera, _Position, _Normal, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _BaseReflectivity);
	//}

	_Result += ImageBasedLightCalculation(*_PBRUniforms->Environment, *_PBRUniforms->Iradiance, *_PBRUniforms->BRDFLookUp, _PositionToCamera, _Position, _Normal, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _BaseReflectivity);



	_Result = Math::Vec3f(1.0f, 1.0f, 1.0f) - Math::Vec3f::Exp(-_Result * _PBRUniforms->Exposure);
	_Result = Math::Vec3f::Pow(_Result, Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	_Result = Rasterizer::Context::Blend(Math::Vec3f(0.0f, 0.0f, 0.0f), Math::Vec4f(_Result, 1.0f), _BlendingType);

	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 0] = (uint8_t)(_Result.x * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 1] = (uint8_t)(_Result.y * 255.0f);
	_TrueFrameBuffer->ColorBuffer->Data[(_X + _Y * _TrueFrameBuffer->ColorBuffer->Width) * 4 + 2] = (uint8_t)(_Result.z * 255.0f);
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

	float _Exposure = 2.5f;

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

		_Uniforms.Exposure = _Exposure;
		_Uniforms.Model = _Transform.GetModelMatrix();
		_Uniforms.View = _Camera.GetViewMatrix();
		_Uniforms.Projection = _Camera.GetProjectionMatrix(_AspectRatio);
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;
		_Uniforms.ModelInversedTransposed = _Uniforms.Model.Inversed().Transposed();
		_Uniforms.Camera = _Camera;
		_Uniforms.CameraForwardVector = _Camera.GetForwardVector();
		_Uniforms.Transform = _Transform;
		std::wstring _MaterialName(L"Material 0");
		_MaterialName[9] += (wchar_t)(_Index);
		_Uniforms.Material = *(Rasterizer::Material*)(SceneAssets.GetAssetData(_MaterialName.c_str()));
		_Uniforms.Environment = (const Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Environment texture"));
		_Uniforms.Iradiance = (const Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(L"Iradiance texture"));
		_Uniforms.BRDFLookUp = (const Rasterizer::Texture_RG*)(SceneAssets.GetAssetData(L"BRDF lookup"));

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
