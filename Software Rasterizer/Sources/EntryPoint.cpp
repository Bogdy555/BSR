#include "..\Headers\Main.hpp"



constexpr size_t Width = 1920;
constexpr size_t Height = 1080;



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
	Math::Vec3f _PositionToLight = -_Light.Direction;
	Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	Math::Vec3f _Radiance = _Light.Color * _Light.Intensity;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _BaseReflectivity);
	Math::Vec3f _kDiffuse = (Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	Math::Vec3f _Diffuse = _kDiffuse * _Albedo / Math::fPi;
	Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

const Math::Vec3f PointLightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Position - _Light.Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _BaseReflectivity);
	Math::Vec3f _kDiffuse = (Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	Math::Vec3f _Diffuse = _kDiffuse * _Albedo / Math::fPi;
	Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

const Math::Vec3f SpotLightCalculation(const Rasterizer::Light& _Light, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Position - _Light.Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _BaseReflectivity);
	Math::Vec3f _kDiffuse = (Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	Math::Vec3f _Diffuse = _kDiffuse * _Albedo / Math::fPi;
	Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	Math::Vec3f _Result = (_Diffuse + _Specular) * _Radiance * Math::Max(Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);

	float _CosTheta = cosf(_Light.Theta);
	float _CosThetaPlusThetaFade = cosf(_Light.Theta + _Light.ThetaFade);
	float _Cos = Math::Vec3f::Dot(_Light.Direction, -_PositionToLight);
	float _MixT = Math::Clamp((_Cos - _CosThetaPlusThetaFade) / (_CosTheta - _CosThetaPlusThetaFade), 0.0f, 1.0f);

	return Math::Vec3f::Mix(Math::Vec3f(0.0f, 0.0f, 0.0f), _Result, _MixT);
}

const Math::Vec3f ImageBasedLightCalculation(const Rasterizer::Texture_Float_RGB& _EnvironmentTexture, const Rasterizer::Texture_Float_RGB& _IradianceTexture, const Rasterizer::Texture_RG& _BRDFLookUpTexture, const Math::Vec3f& _PositionToCamera, const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const Math::Vec3f& _BaseReflectivity)
{
	Math::Vec3f _FresnelSchlickRoughness = FresnelSchlickRoughness(_Normal, _PositionToCamera, _BaseReflectivity, _Roughness);
	Math::Vec3f _kDiffuse = (Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlickRoughness) * (1.0f - _Metalness);

	Math::Vec3f _Iradiance = _IradianceTexture.SampleRGB(SampleEquirectangularMap(_Normal));
	Math::Vec3f _Environment = _EnvironmentTexture.SampleRGB(SampleEquirectangularMap(Math::Vec3f::Reflect(-_PositionToCamera, _Normal)));
	_Environment = Math::Vec3f::Mix(_Environment, _Iradiance, _Roughness);
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

void CleanUpModel(const wchar_t* _AssetName)
{
	delete (Rasterizer::Model*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpTexture_Float_RGB(const wchar_t* _AssetName)
{
	ASSERT(dynamic_cast<Rasterizer::Texture_Float_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (Rasterizer::Texture_Float_RGB*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpTexture_RGB(const wchar_t* _AssetName)
{
	ASSERT(dynamic_cast<Rasterizer::Texture_RGB*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpTexture_R(const wchar_t* _AssetName)
{
	ASSERT(dynamic_cast<Rasterizer::Texture_R*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpMaterial(const wchar_t* _AssetName)
{
	delete (Rasterizer::Material*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpTexture_RG(const wchar_t* _AssetName)
{
	ASSERT(dynamic_cast<Rasterizer::Texture_RG*>((Rasterizer::Texture*)(SceneAssets.GetAssetData(_AssetName))) || SceneAssets.GetAssetData(_AssetName) == nullptr);
	delete (Rasterizer::Texture_RG*)(SceneAssets.GetAssetData(_AssetName));
	SceneAssets.RemoveAsset(_AssetName);
}

void CleanUpAssets()
{
	CleanUpModel(L"Model");

	CleanUpTexture_Float_RGB(L"Environment texture");
	CleanUpTexture_Float_RGB(L"Iradiance texture");

	CleanUpTexture_RGB(L"Aluminum albedo");
	CleanUpTexture_R(L"Aluminum metalness");
	CleanUpTexture_RGB(L"Aluminum normal");
	CleanUpTexture_R(L"Aluminum roughness");

	CleanUpTexture_RGB(L"Container albedo");
	CleanUpTexture_R(L"Container ambient occlusion");
	CleanUpTexture_R(L"Container metalness");
	CleanUpTexture_RGB(L"Container normal");
	CleanUpTexture_R(L"Container roughness");

	CleanUpTexture_RGB(L"Gold albedo");
	CleanUpTexture_R(L"Gold metalness");
	CleanUpTexture_RGB(L"Gold normal");
	CleanUpTexture_R(L"Gold roughness");

	CleanUpTexture_RGB(L"Iron albedo");
	CleanUpTexture_R(L"Iron metalness");
	CleanUpTexture_RGB(L"Iron normal");
	CleanUpTexture_R(L"Iron roughness");

	CleanUpTexture_RGB(L"Plastic albedo green");
	CleanUpTexture_RGB(L"Plastic albedo red");
	CleanUpTexture_R(L"Plastic ambient occlusion");
	CleanUpTexture_R(L"Plastic metalness");
	CleanUpTexture_RGB(L"Plastic normal");
	CleanUpTexture_R(L"Plastic roughness");

	CleanUpTexture_R(L"White Texture_R");

	CleanUpMaterial(L"Material 0");
	CleanUpMaterial(L"Material 1");
	CleanUpMaterial(L"Material 2");
	CleanUpMaterial(L"Material 3");
	CleanUpMaterial(L"Material 4");
	CleanUpMaterial(L"Material 5");
	CleanUpMaterial(L"Material 6");
	CleanUpMaterial(L"Material 7");

	CleanUpTexture_RG(L"BRDF lookup");

	ASSERT(SceneAssets.GetAssetsCount() == 0);
}

bool LoadModel(const wchar_t* _FilePath, const wchar_t* _AssetName)
{
	Rasterizer::Model* _Model = new Rasterizer::Model;

	if (!_Model)
	{
		CleanUpAssets();
		return false;
	}

	if (!_Model->Load(_FilePath))
	{
		delete _Model;
		CleanUpAssets();
		return false;
	}

	if (!SceneAssets.AddAsset(_Model, _AssetName))
	{
		delete _Model;
		CleanUpAssets();
		return false;
	}

	return true;
}

bool LoadTexture_Float_RGB(const wchar_t* _FilePath, const wchar_t* _AssetName)
{
	Image::ImageFloat _Image;

	_Image.Data = Image::LoadHdr(_FilePath, _Image.Width, _Image.Height);

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

	_Texture->SetWrapType(Rasterizer::_WrapClamp);

	if (!_Texture->AddMip(_Image))
	{
		delete _Texture;
		delete[] _Image.Data;
		CleanUpAssets();
		return false;
	}

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		CleanUpAssets();
		return false;
	}

	delete[] _Image.Data;

	return true;
}

bool LoadTexture_RGB(const wchar_t* _FilePath, const wchar_t* _AssetName)
{
	Image::Image _Image;

	_Image.Data = Image::LoadBmp(_FilePath, _Image.Width, _Image.Height);

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

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		CleanUpAssets();
		return false;
	}

	delete[] _Image.Data;

	return true;
}

bool LoadTexture_R(const wchar_t* _FilePath, const wchar_t* _AssetName)
{
	Image::Image _Image;

	_Image.Data = Image::LoadBmp(_FilePath, _Image.Width, _Image.Height);

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

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		CleanUpAssets();
		return false;
	}

	delete[] _Image.Data;

	return true;
}

bool LoadTexture_RG(const wchar_t* _FilePath, const wchar_t* _AssetName)
{
	Image::Image _Image;

	_Image.Data = Image::LoadBmp(_FilePath, _Image.Width, _Image.Height);

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

	if (!SceneAssets.AddAsset(_Texture, _AssetName))
	{
		delete _Texture;
		delete[] _Image.Data;
		CleanUpAssets();
		return false;
	}

	delete[] _Image.Data;

	return true;
}

bool GenerateMaterialAsset(const wchar_t* _AssetName, const wchar_t* _Albedo, const wchar_t* _Metalness, const wchar_t* _Roughness, const wchar_t* _AmbientOcclusion, const wchar_t* _NormalMap)
{
	Rasterizer::Material* _Material = new Rasterizer::Material;

	if (!_Material)
	{
		CleanUpAssets();
		return false;
	}

	_Material->Albedo = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_Albedo));
	_Material->Metalness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_Metalness));
	_Material->Roughness = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_Roughness));
	_Material->AmbientOcclusion = (Rasterizer::Texture_R*)(SceneAssets.GetAssetData(_AmbientOcclusion));
	_Material->NormalMap = (Rasterizer::Texture_RGB*)(SceneAssets.GetAssetData(_NormalMap));

	if (!SceneAssets.AddAsset(_Material, _AssetName))
	{
		delete _Material;
		CleanUpAssets();
		return false;
	}

	return true;
}

bool LoadAssets()
{
	if (!LoadModel(L".\\3D Models\\Objects.wfobj", L"Model"))
	{
		return false;
	}

	if (!LoadTexture_Float_RGB(L".\\Environments\\ParkingLot\\Environment.hdr", L"Environment texture"))
	{
		return false;
	}

	if (!LoadTexture_Float_RGB(L".\\Environments\\ParkingLot\\Iradiance.hdr", L"Iradiance texture"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Aluminum\\Albedo.bmp", L"Aluminum albedo"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Aluminum\\Metalness.bmp", L"Aluminum metalness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Aluminum\\Normal.bmp", L"Aluminum normal"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Aluminum\\Roughness.bmp", L"Aluminum roughness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Container\\Albedo.bmp", L"Container albedo"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\AmbientOcclusion.bmp", L"Container ambient occlusion"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\Metalness.bmp", L"Container metalness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Container\\Normal.bmp", L"Container normal"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Container\\Roughness.bmp", L"Container roughness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Gold\\Albedo.bmp", L"Gold albedo"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Gold\\Metalness.bmp", L"Gold metalness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Gold\\Normal.bmp", L"Gold normal"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Gold\\Roughness.bmp", L"Gold roughness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Iron\\Albedo.bmp", L"Iron albedo"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Iron\\Metalness.bmp", L"Iron metalness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Iron\\Normal.bmp", L"Iron normal"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Iron\\Roughness.bmp", L"Iron roughness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Albedo Green.bmp", L"Plastic albedo green"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Albedo Red.bmp", L"Plastic albedo red"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\AmbientOcclusion.bmp", L"Plastic ambient occlusion"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\Metalness.bmp", L"Plastic metalness"))
	{
		return false;
	}

	if (!LoadTexture_RGB(L".\\Materials\\Plastic\\Normal.bmp", L"Plastic normal"))
	{
		return false;
	}

	if (!LoadTexture_R(L".\\Materials\\Plastic\\Roughness.bmp", L"Plastic roughness"))
	{
		return false;
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

	if (!GenerateMaterialAsset(L"Material 0", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 1", L"Gold albedo", L"Gold metalness", L"Gold roughness", L"White Texture_R", L"Gold normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 2", L"Aluminum albedo", L"Aluminum metalness", L"Aluminum roughness", L"White Texture_R", L"Aluminum normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 3", L"Plastic albedo green", L"Plastic metalness", L"Plastic roughness", L"Plastic ambient occlusion", L"Plastic normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 4", L"Iron albedo", L"Iron metalness", L"Iron roughness", L"White Texture_R", L"Iron normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 5", L"Plastic albedo red", L"Plastic metalness", L"Plastic roughness", L"Plastic ambient occlusion", L"Plastic normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 6", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!GenerateMaterialAsset(L"Material 7", L"Container albedo", L"Container metalness", L"Container roughness", L"Container ambient occlusion", L"Container normal"))
	{
		return false;
	}

	if (!LoadTexture_RG(L".\\Intern\\BRDFLookUp.bmp", L"BRDF lookup"))
	{
		return false;
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
	Time::Timer _ProfilerTimer;

	LOG_LINE(STRING_TYPE("Loading the assets"));

	_ProfilerTimer.Start();

	if (!LoadAssets())
	{
		LOG_LINE(STRING_TYPE("Failed"));
		return -1;
	}

	_ProfilerTimer.Stop();

	LOG(STRING_TYPE("Time: "));
	LOG_LINE(_ProfilerTimer);

	LOG_LINE(STRING_TYPE("Rendering the scene"));

	_ProfilerTimer.Start();

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

	_ProfilerTimer.Stop();

	LOG(STRING_TYPE("Time: "));
	LOG_LINE(_ProfilerTimer);

	LOG(STRING_TYPE("FPS: "));
	LOG_LINE(1.0f / _ProfilerTimer);

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
