#include "..\Headers\BSR.hpp"



const BSR::Math::Vec3f SampleNormal(const BSR::Rasterizer::Texture_RGB& _NormalMap, const BSR::Math::Vec3f& _NormalMapMultiplier, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Tangent, const BSR::Math::Vec2f& _TextureCoords)
{
	if (_Normal == BSR::Math::Vec3f(0.0f, 0.0f, 0.0f))
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	if (_Tangent == BSR::Math::Vec3f(0.0f, 0.0f, 0.0f))
	{
		return _Normal.Normalized();
	}

	BSR::Math::Vec3f _N = _Normal.Normalized();
	BSR::Math::Vec3f _T = _Tangent.Normalized();
	_T = (_T - _N * BSR::Math::Vec3f::Dot(_N, _T)).Normalized();
	BSR::Math::Vec3f _B = BSR::Math::Vec3f::Cross(_N, _T);

	BSR::Math::Mat3f _TBN;

	_TBN[0][0] = _T.x; _TBN[0][1] = _B.x; _TBN[0][2] = _N.x;
	_TBN[1][0] = _T.y; _TBN[1][1] = _B.y; _TBN[1][2] = _N.y;
	_TBN[2][0] = _T.z; _TBN[2][1] = _B.z; _TBN[2][2] = _N.z;

	BSR::Math::Vec3f _NFromMap = _NormalMap.SampleRGB(_TextureCoords) * _NormalMapMultiplier * 2.0f - 1.0f;

	return _TBN * _NFromMap.Normalized();
}

const BSR::Math::Vec2f SampleEquirectangularMap(const BSR::Math::Vec3f& _Dir)
{
	return BSR::Math::Vec2f(atan2(_Dir.z, _Dir.x), asin(_Dir.y)) * BSR::Math::Vec2f(0.1591f, 0.3183f) + BSR::Math::Vec2f(0.5f, 0.5f);
}

const BSR::Math::Vec3f ReverseSampleEquirectangularMap(const BSR::Math::Vec2f& _UV)
{
	BSR::Math::Vec2f _NewUV = (_UV - BSR::Math::Vec2f(0.5f, 0.5f)) / BSR::Math::Vec2f(0.1591f, 0.3183f);
	return BSR::Math::Vec3f(cosf(_NewUV.x), sinf(_NewUV.y), sinf(_NewUV.x)).Normalized();
}

const float NormalDistributionFunctionGGX(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _HalfWayVec, const float _Roughness)
{
	float _AlphaSquared = _Roughness * _Roughness;
	_AlphaSquared *= _AlphaSquared;

	float _DotNormalHalfWayVecSquare = BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f);
	_DotNormalHalfWayVecSquare *= _DotNormalHalfWayVecSquare;

	float _Numerator = _AlphaSquared;
	float _Denominator = _DotNormalHalfWayVecSquare * (_AlphaSquared - 1.0f) + 1.0f;
	_Denominator = BSR::Math::fPi * _Denominator * _Denominator;

	return _Numerator / _Denominator;
}

const float GeometrySchlickGGX(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Vec, const float _Roughness)
{
	float _K = _Roughness + 1.0f;
	_K = _K * _K / 8.0f;

	float _Numerator = BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _Vec), 0.0f);
	float _Denominator = _Numerator * (1.0f - _K) + _K;

	return _Numerator / _Denominator;
}

const float GeometrySmith(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _PositionToLight, const float _Roughness)
{
	return GeometrySchlickGGX(_Normal, _PositionToCamera, _Roughness) * GeometrySchlickGGX(_Normal, _PositionToLight, _Roughness);
}

const BSR::Math::Vec3f FresnelSchlick(const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _HalfWayVec, const BSR::Math::Vec3f& _Reflectivity)
{
	return _Reflectivity + (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _Reflectivity) * powf(BSR::Math::Clamp(1.0f - BSR::Math::Max(BSR::Math::Vec3f::Dot(_PositionToCamera, _HalfWayVec), 0.0f), 0.0f, 1.0f), 5.0f);
}

const BSR::Math::Vec3f FresnelSchlickRoughness(const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Reflectivity, const float _Roughness)
{
	return _Reflectivity + (BSR::Math::Vec3f::Max(BSR::Math::Vec3f(1.0f - _Roughness, 1.0f - _Roughness, 1.0f - _Roughness), _Reflectivity) - _Reflectivity) * powf(BSR::Math::Clamp(1.0f - BSR::Math::Max(BSR::Math::Vec3f::Dot(_PositionToCamera, _Normal), 0.0f), 0.0f, 1.0f), 5.0f);
}

const BSR::Math::Vec3f DirectionalLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = -_Light.Direction;
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::fPi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

const BSR::Math::Vec3f PointLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Position - _Light.Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::fPi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

const BSR::Math::Vec3f SpotLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Position - _Light.Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = NormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = GeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = FresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::fPi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	BSR::Math::Vec3f _Result = (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);

	float _CosTheta = cosf(_Light.Theta);
	float _CosThetaPlusThetaFade = cosf(_Light.Theta + _Light.ThetaFade);
	float _Cos = BSR::Math::Vec3f::Dot(_Light.Direction, -_PositionToLight);
	float _MixT = BSR::Math::Clamp((_Cos - _CosThetaPlusThetaFade) / (_CosTheta - _CosThetaPlusThetaFade), 0.0f, 1.0f);

	return BSR::Math::Vec3f::Mix(BSR::Math::Vec3f(0.0f, 0.0f, 0.0f), _Result, _MixT);
}

const BSR::Math::Vec3f ImageBasedLightCalculation(const BSR::Rasterizer::Texture_Float_RGB& _EnvironmentTexture, const BSR::Rasterizer::Texture_Float_RGB& _IradianceTexture, const BSR::Rasterizer::Texture_RG& _BRDFLookUpTexture, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _FresnelSchlickRoughness = FresnelSchlickRoughness(_Normal, _PositionToCamera, _Reflectivity, _Roughness);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlickRoughness) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Iradiance = _IradianceTexture.SampleRGB(SampleEquirectangularMap(_Normal));
	BSR::Math::Vec3f _Environment = _EnvironmentTexture.SampleRGB(SampleEquirectangularMap(BSR::Math::Vec3f::Reflect(-_PositionToCamera, _Normal)));
	_Environment = BSR::Math::Vec3f::Mix(_Environment, _Iradiance, _Roughness);
	BSR::Math::Vec2f _BRDFLookUp = _BRDFLookUpTexture.SampleRG(BSR::Math::Vec2f(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f), _Roughness));

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Iradiance * _Albedo;
	BSR::Math::Vec3f _Specular = _Environment * (_FresnelSchlickRoughness * _BRDFLookUp.x + _BRDFLookUp.y);

	return (_Diffuse + _Specular) * _AmbientOcclusion;
}

const BSR::Math::Vec3f LightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	switch (_Light.Type)
	{
	case BSR::Renderer::_DirectionalLight:
	{
		return DirectionalLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_PointLight:
	{
		return PointLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_SpotLight:
	{
		return SpotLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	default:
	{
		BSR_DEBUG_BREAK();
		break;
	}
	}

	return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
}



struct DeferredUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	uint64_t MeshId = 0;
	BSR::Renderer::Material Material;
	BSR::Renderer::Transform Transform;

	BSR::Math::Mat4f Model;
	BSR::Math::Mat3f ModelInversedTransposed;
	BSR::Math::Mat4f View;
	BSR::Math::Mat4f Projection;
	BSR::Math::Mat4f Mvp;
};

struct DeferredLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	BSR::Math::Vec3f Normal = BSR::Math::Vec3f(0.0f, 0.0f, 1.0f);
	BSR::Math::Vec3f Tangent = BSR::Math::Vec3f(1.0f, 0.0f, 0.0f);
	BSR::Math::Vec2f TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
};

const BSR::Math::Vec4f DeferredVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const DeferredUniforms& _TrueUniforms = *(const DeferredUniforms*)(_Uniforms);
	DeferredLerpers& _TrueLerpers = *(DeferredLerpers*)(_OutLerpers);

	_TrueLerpers.Position = BSR::Math::Vec3f(_TrueUniforms.Model * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f));
	_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;
	_TrueLerpers.Tangent = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Tangent;
	_TrueLerpers.TextureCoords = _TrueVertex.TextureCoords;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

void DeferredFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const DeferredLerpers& _TrueLerpers = *(const DeferredLerpers*)(_Lerpers);
	const DeferredUniforms& _TrueUniforms = *(const DeferredUniforms*)(_Uniforms);
	BSR::Renderer::FrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::FrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	if (_FrontFacing)
	{
		_TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Albedo->SampleRGB(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.AlbedoMultiplier;
		_TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Metalness->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.MetalnessMultiplier;
		_TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Roughness->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.RoughnessMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusion->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.AmbientOcclusionMultiplier;
		_TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width] = SampleNormal(*_TrueUniforms.Material.NormalMap, _TrueUniforms.Material.NormalMapMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Emission->SampleRGB(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.EmissionMultiplier;
	}
	else
	{
		_TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AlbedoBack->SampleRGB(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.AlbedoBackMultiplier;
		_TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.MetalnessBack->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.MetalnessBackMultiplier;
		_TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.RoughnessBack->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.RoughnessBackMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusionBack->SampleR(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.AmbientOcclusionBackMultiplier;
		_TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width] = -SampleNormal(*_TrueUniforms.Material.NormalMapBack, _TrueUniforms.Material.NormalMapBackMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.EmissionBack->SampleRGB(_TrueLerpers.TextureCoords) * _TrueUniforms.Material.EmissionBackMultiplier;
	}

	_TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width] = _TrueLerpers.Position;
	_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] = _FragCoord.z;
	_TrueFrameBuffer.Stencil[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.MeshId;
}



struct CubeMapUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	const BSR::Rasterizer::Texture_Float_RGB* Environment = nullptr;

	BSR::Math::Mat4f Mvp;
};

struct CubeMapLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
};

const BSR::Math::Vec4f CubeMapVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const CubeMapUniforms& _TrueUniforms = *(const CubeMapUniforms*)(_Uniforms);
	CubeMapLerpers& _TrueLerpers = *(CubeMapLerpers*)(_OutLerpers);

	_TrueLerpers.Position = _TrueVertex.Position;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

void CubeMapFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const CubeMapLerpers& _TrueLerpers = *(const CubeMapLerpers*)(_Lerpers);
	const CubeMapUniforms& _TrueUniforms = *(const CubeMapUniforms*)(_Uniforms);
	BSR::Renderer::FrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::FrameBuffer*)(_FrameBuffer);

	if (_TrueUniforms.Camera.Perspective)
	{
		_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Environment->SampleRGB(SampleEquirectangularMap(_TrueLerpers.Position.Normalized()));
	}
	else
	{
		_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Environment->SampleRGB(SampleEquirectangularMap(_TrueUniforms.CameraForwardVector));
	}
}



struct CompositUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	float FogStart = Camera.FarPlane;
	float FogEnd = Camera.FarPlane + 1.0f;
	BSR::Math::Vec3f FogColor = BSR::Math::Vec3f(0.8f, 0.8f, 0.8f);
	bool FogDepthBased = false;

	const BSR::Rasterizer::Texture_Float_RGB* Environment = nullptr;
	const BSR::Rasterizer::Texture_Float_RGB* Iradiance = nullptr;
	const BSR::Rasterizer::Texture_RG* BRDFLookUp = nullptr;
	const std::vector<BSR::Renderer::Light>* Lights = nullptr;
};

const BSR::Math::Vec4f CompositVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const CompositUniforms& _TrueUniforms = *(const CompositUniforms*)(_Uniforms);

	return BSR::Math::Vec4f(_TrueVertex.Position * 2.0f, 1.0f);
}

void CompositFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const CompositUniforms& _TrueUniforms = *(const CompositUniforms*)(_Uniforms);
	BSR::Renderer::FrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::FrameBuffer*)(_FrameBuffer);

	if (_TrueFrameBuffer.Stencil[_X + _Y * _TrueFrameBuffer.Width] != 0)
	{
		BSR::Math::Vec3f _PositionToCamera;

		if (_TrueUniforms.Camera.Perspective)
		{
			_PositionToCamera = (_TrueUniforms.Camera.Position - _TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width]).Normalized();
		}
		else
		{
			_PositionToCamera = -_TrueUniforms.CameraForwardVector;
		}

		BSR::Math::Vec3f _Reflectivity = BSR::Math::Vec3f::Mix(BSR::Math::Vec3f(0.04f, 0.04f, 0.04f), _TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width]);

		BSR::Math::Vec3f _Result = _TrueFrameBuffer.Emission[_X + _Y * _TrueFrameBuffer.Width];

		for (size_t _Index = 0; _Index < _TrueUniforms.Lights->size(); _Index++)
		{
			_Result += LightCalculation((*_TrueUniforms.Lights)[_Index], _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width]);
		}

		_Result += ImageBasedLightCalculation(*_TrueUniforms.Environment, *_TrueUniforms.Iradiance, *_TrueUniforms.BRDFLookUp, _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width]);

		float _Distance;

		if (_TrueUniforms.FogDepthBased)
		{
			if (_TrueUniforms.Camera.Perspective)
			{
				_Distance = 2.0f * _TrueUniforms.Camera.FarPlane * _TrueUniforms.Camera.NearPlane / (_TrueUniforms.Camera.FarPlane + _TrueUniforms.Camera.NearPlane - (_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * 2.0f - 1.0f) * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane));
			}
			else
			{
				_Distance = _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane) + _TrueUniforms.Camera.NearPlane;
			}
		}
		else
		{
			_Distance = (_TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width] - _TrueUniforms.Camera.Position).Magnitude();
		}

		if (_Distance > _TrueUniforms.FogStart && _Distance < _TrueUniforms.FogEnd)
		{
			_Result = BSR::Math::Vec3f::Mix(_Result, _TrueUniforms.FogColor, (_Distance - _TrueUniforms.FogStart) / (_TrueUniforms.FogEnd - _TrueUniforms.FogStart));
		}
		else if (_Distance >= _TrueUniforms.FogEnd)
		{
			_Result = _TrueUniforms.FogColor;
		}

		_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = _Result;
	}
}



struct GammaAndHDRUniforms
{
	float Exposure = 1.0f;
};

const BSR::Math::Vec4f GammaAndHDRVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const GammaAndHDRUniforms& _TrueUniforms = *(const GammaAndHDRUniforms*)(_Uniforms);

	return BSR::Math::Vec4f(_TrueVertex.Position * 2.0f, 1.0f);
}

void GammaAndHDRFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const GammaAndHDRUniforms& _TrueUniforms = *(const GammaAndHDRUniforms*)(_Uniforms);
	BSR::Renderer::FrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::FrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - BSR::Math::Vec3f::Exp(-_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] * _TrueUniforms.Exposure);
	_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f::Pow(_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width], BSR::Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
}



BSR::Renderer::Instance::Instance() : TargetFrameBuffer(), TargetCamera(), TargetExposure(1.0f), TargetFogStart(TargetCamera.FarPlane), TargetFogEnd(TargetCamera.FarPlane + 1.0f), TargetFogColor(0.8f, 0.8f, 0.8f), TargetFogDepthBased(false), TargetEnvironment(nullptr), TargetIradiance(nullptr), TargetBRDFLookUp(nullptr), TargetMeshes(), TargetMaterials(), TargetTransforms(), TargetLights()
{

}

BSR::Renderer::Instance::~Instance()
{
	BSR_ASSERT(TargetFrameBuffer == FrameBuffer());
}

bool BSR::Renderer::Instance::StartScene(FrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const bool _TargetFogDepthBased, const Rasterizer::Texture_Float_RGB& _TargetEnvironment, const Rasterizer::Texture_Float_RGB& _TargetIradiance, const Rasterizer::Texture_RG& _TargetBRDFLookUp)
{
	TargetMeshes.clear();
	TargetMaterials.clear();
	TargetTransforms.clear();
	TargetLights.clear();

	if (!_TargetFrameBuffer.Valid())
	{
		TargetFrameBuffer = FrameBuffer();
		TargetCamera = Camera();
		TargetExposure = 1.0f;
		TargetFogStart = TargetCamera.FarPlane;
		TargetFogEnd = TargetCamera.FarPlane + 1.0f;
		TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
		TargetFogDepthBased = false;
		TargetEnvironment = nullptr;
		TargetIradiance = nullptr;
		TargetBRDFLookUp = nullptr;
		return false;
	}

	TargetFrameBuffer = _TargetFrameBuffer;
	TargetCamera = _TargetCamera;
	TargetExposure = _TargetExposure;
	TargetFogStart = _TargetFogStart;
	TargetFogEnd = _TargetFogEnd;
	TargetFogColor = _TargetFogColor;
	TargetFogDepthBased = _TargetFogDepthBased;
	TargetEnvironment = &_TargetEnvironment;
	TargetIradiance = &_TargetIradiance;
	TargetBRDFLookUp = &_TargetBRDFLookUp;

	return true;
}

bool BSR::Renderer::Instance::FlushScene()
{
	if (!TargetFrameBuffer.Valid())
	{
		return false;
	}

	BSR_ASSERT(TargetMeshes.size() == TargetMaterials.size() && TargetMaterials.size() == TargetTransforms.size());
	BSR_ASSERT(TargetFogStart < TargetFogEnd);

	Mesh _Cube;
	Mesh::GenerateCube(_Cube);
	Mesh _Quad;
	Mesh::GenerateQuad(_Quad);

	Rasterizer::Context _Context;

	for (size_t _Y = 0; _Y < TargetFrameBuffer.Height; _Y++)
	{
		for (size_t _X = 0; _X < TargetFrameBuffer.Width; _X++)
		{
			TargetFrameBuffer.Albedo[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Metalness[_X + _Y * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.Roughness[_X + _Y * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.AmbientOcclusion[_X + _Y * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.NormalMap[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Emission[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Position[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Depth[_X + _Y * TargetFrameBuffer.Width] = 1.0f;
			TargetFrameBuffer.Stencil[_X + _Y * TargetFrameBuffer.Width] = 0;
			TargetFrameBuffer.Result[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
		}
	}

	_Context.SetViewPort(0, 0, TargetFrameBuffer.Width, TargetFrameBuffer.Height);
	_Context.SetBlendingType(Rasterizer::_NoBlending);

	_Context.SetDepthTestingType(Rasterizer::_LowerOrEqualDepthTesting);

	for (size_t _Index = 0; _Index < TargetMeshes.size(); _Index++)
	{
		const Mesh& _CurrentMesh = *TargetMeshes[_Index];
		const Material& _CurrentMaterial = TargetMaterials[_Index];
		const Transform& _CurrentTransform = TargetTransforms[_Index];

		DeferredUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = _Uniforms.Camera.GetForwardVector();
		_Uniforms.MeshId = (uint64_t)(_Index) + 1;
		_Uniforms.Material = _CurrentMaterial;
		_Uniforms.Transform = _CurrentTransform;
		_Uniforms.Model = _Uniforms.Transform.GetModelMatrix();
		_Uniforms.ModelInversedTransposed = Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
		_Uniforms.View = _Uniforms.Camera.GetViewMatrix();
		_Uniforms.Projection = _Uniforms.Camera.GetProjectionMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;

		_Context.SetCullingType(_CurrentMaterial.GetCullingType());

		if (!_Context.RenderMesh(_CurrentMesh.VBO.GetData(), _CurrentMesh.VBO.GetSize(), sizeof(VertexData), _CurrentMesh.IBO.GetData(), 0, _CurrentMesh.IBO.GetSize() * 3, &_Uniforms, sizeof(DeferredLerpers), sizeof(DeferredLerpers), DeferredVertexShader, nullptr, DeferredFragmentShader, &TargetFrameBuffer))
		{
			TargetCamera = Camera();
			TargetFrameBuffer = FrameBuffer();
			TargetExposure = 1.0f;
			TargetFogStart = TargetCamera.FarPlane;
			TargetFogEnd = TargetCamera.FarPlane + 1.0f;
			TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
			TargetFogDepthBased = false;
			TargetEnvironment = nullptr;
			TargetIradiance = nullptr;
			TargetBRDFLookUp = nullptr;
			TargetMeshes.clear();
			TargetMaterials.clear();
			TargetTransforms.clear();
			TargetLights.clear();
			return false;
		}
	}

	_Context.SetDepthTestingType(Rasterizer::_NoDepthTesting);
	_Context.SetCullingType(Rasterizer::_CounterClockWiseCulling);

	{
		CubeMapUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = TargetCamera.GetForwardVector();
		_Uniforms.Environment = TargetEnvironment;
		_Uniforms.Mvp = TargetCamera.GetCubeMapMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));

		if (!_Context.RenderMesh(_Cube.VBO.GetData(), _Cube.VBO.GetSize(), sizeof(VertexData), _Cube.IBO.GetData(), 0, _Cube.IBO.GetSize() * 3, &_Uniforms, sizeof(CubeMapLerpers) / sizeof(float), sizeof(CubeMapLerpers) / sizeof(float), CubeMapVertexShader, nullptr, CubeMapFragmentShader, &TargetFrameBuffer))
		{
			TargetCamera = Camera();
			TargetFrameBuffer = FrameBuffer();
			TargetExposure = 1.0f;
			TargetFogStart = TargetCamera.FarPlane;
			TargetFogEnd = TargetCamera.FarPlane + 1.0f;
			TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
			TargetFogDepthBased = false;
			TargetEnvironment = nullptr;
			TargetIradiance = nullptr;
			TargetBRDFLookUp = nullptr;
			TargetMeshes.clear();
			TargetMaterials.clear();
			TargetTransforms.clear();
			TargetLights.clear();
			return false;
		}
	}

	_Context.SetDepthTestingType(Rasterizer::_NoDepthTesting);
	_Context.SetCullingType(Rasterizer::_ClockWiseCulling);

	{
		CompositUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = TargetCamera.GetForwardVector();
		_Uniforms.FogStart = TargetFogStart;
		_Uniforms.FogEnd = TargetFogEnd;
		_Uniforms.FogColor = TargetFogColor;
		_Uniforms.FogDepthBased = TargetFogDepthBased;
		_Uniforms.Environment = TargetEnvironment;
		_Uniforms.Iradiance = TargetIradiance;
		_Uniforms.BRDFLookUp = TargetBRDFLookUp;
		_Uniforms.Lights = &TargetLights;

		if (!_Context.RenderMesh(_Quad.VBO.GetData(), _Quad.VBO.GetSize(), sizeof(VertexData), _Quad.IBO.GetData(), 0, _Quad.IBO.GetSize() * 3, &_Uniforms, 0, 0, CompositVertexShader, nullptr, CompositFragmentShader, &TargetFrameBuffer))
		{
			TargetCamera = Camera();
			TargetFrameBuffer = FrameBuffer();
			TargetExposure = 1.0f;
			TargetFogStart = TargetCamera.FarPlane;
			TargetFogEnd = TargetCamera.FarPlane + 1.0f;
			TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
			TargetFogDepthBased = false;
			TargetEnvironment = nullptr;
			TargetIradiance = nullptr;
			TargetBRDFLookUp = nullptr;
			TargetMeshes.clear();
			TargetMaterials.clear();
			TargetTransforms.clear();
			TargetLights.clear();
			return false;
		}
	}

	_Context.SetDepthTestingType(Rasterizer::_NoDepthTesting);
	_Context.SetCullingType(Rasterizer::_ClockWiseCulling);

	{
		GammaAndHDRUniforms _Uniforms;

		_Uniforms.Exposure = TargetExposure;

		if (!_Context.RenderMesh(_Quad.VBO.GetData(), _Quad.VBO.GetSize(), sizeof(VertexData), _Quad.IBO.GetData(), 0, _Quad.IBO.GetSize() * 3, &_Uniforms, 0, 0, GammaAndHDRVertexShader, nullptr, GammaAndHDRFragmentShader, &TargetFrameBuffer))
		{
			TargetCamera = Camera();
			TargetFrameBuffer = FrameBuffer();
			TargetExposure = 1.0f;
			TargetFogStart = TargetCamera.FarPlane;
			TargetFogEnd = TargetCamera.FarPlane + 1.0f;
			TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
			TargetFogDepthBased = false;
			TargetEnvironment = nullptr;
			TargetIradiance = nullptr;
			TargetBRDFLookUp = nullptr;
			TargetMeshes.clear();
			TargetMaterials.clear();
			TargetTransforms.clear();
			TargetLights.clear();
			return false;
		}
	}

	TargetCamera = Camera();
	TargetFrameBuffer = FrameBuffer();
	TargetExposure = 1.0f;
	TargetFogStart = TargetCamera.FarPlane;
	TargetFogEnd = TargetCamera.FarPlane + 1.0f;
	TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
	TargetFogDepthBased = false;
	TargetEnvironment = nullptr;
	TargetIradiance = nullptr;
	TargetBRDFLookUp = nullptr;
	TargetMeshes.clear();
	TargetMaterials.clear();
	TargetTransforms.clear();
	TargetLights.clear();

	return true;
}

void BSR::Renderer::Instance::SubmitModel(const Mesh& _TargetMesh, const Material& _TargetMaterial, const Transform& _TargetTransform)
{
	BSR_ASSERT(TargetFrameBuffer.Valid());

	TargetMeshes.push_back(&_TargetMesh);
	TargetMaterials.push_back(_TargetMaterial);
	TargetTransforms.push_back(_TargetTransform);
}

void BSR::Renderer::Instance::SubmitLight(const Light& _TargetLight)
{
	BSR_ASSERT(TargetFrameBuffer.Valid());

	TargetLights.push_back(_TargetLight);
}
