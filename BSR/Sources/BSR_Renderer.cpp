#include "../Headers/BSR.hpp"



static const BSR::Math::Vec3f SampleNormal(const BSR::Rasterizer::TextureSDR& _NormalMap, const BSR::Math::Vec3f& _NormalMapMultiplier, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Tangent, const BSR::Math::Vec2f& _TextureCoords)
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

	BSR::Math::Vec3f _NFromMap = BSR::Math::Vec3f(_NormalMap.Sample(_TextureCoords)) * _NormalMapMultiplier * 2.0f - 1.0f;

	return _TBN * _NFromMap.Normalized();
}

static const BSR::Math::Vec2f SampleEquirectangularMap(const BSR::Math::Vec3f& _Dir)
{
	return BSR::Math::Vec2f(atan2(_Dir.z, _Dir.x), asin(_Dir.y)) * BSR::Math::Vec2f(0.1591f, 0.3183f) + BSR::Math::Vec2f(0.5f, 0.5f);
}



struct PBRDeferredUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	uint64_t MeshId = 0;
	BSR::Renderer::PBRMaterial Material;
	BSR::Renderer::Transform Transform;

	BSR::Math::Mat4f Model;
	BSR::Math::Mat3f ModelInversedTransposed;
	BSR::Math::Mat4f View;
	BSR::Math::Mat4f Projection;
	BSR::Math::Mat4f Mvp;
};

struct PBRDeferredLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	BSR::Math::Vec3f Normal = BSR::Math::Vec3f(0.0f, 0.0f, 1.0f);
	BSR::Math::Vec3f Tangent = BSR::Math::Vec3f(1.0f, 0.0f, 0.0f);
	BSR::Math::Vec2f TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
};

static const BSR::Math::Vec4f PBRDeferredVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const PBRDeferredUniforms& _TrueUniforms = *(const PBRDeferredUniforms*)(_Uniforms);
	PBRDeferredLerpers& _TrueLerpers = *(PBRDeferredLerpers*)(_OutLerpers);

	_TrueLerpers.Position = BSR::Math::Vec3f(_TrueUniforms.Model * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f));
	_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;
	_TrueLerpers.Tangent = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Tangent;
	_TrueLerpers.TextureCoords = _TrueVertex.TextureCoords;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

static void PBRDeferredFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRDeferredLerpers& _TrueLerpers = *(const PBRDeferredLerpers*)(_Lerpers);
	const PBRDeferredUniforms& _TrueUniforms = *(const PBRDeferredUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	if (_FrontFacing)
	{
		_TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.Albedo->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.AlbedoMultiplier;
		_TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Metalness->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.MetalnessMultiplier;
		_TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Roughness->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.RoughnessMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusion->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionMultiplier;
		_TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width] = SampleNormal(*_TrueUniforms.Material.NormalMap, _TrueUniforms.Material.NormalMapMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.Emission->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionMultiplier;
	}
	else
	{
		_TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.AlbedoBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.AlbedoBackMultiplier;
		_TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.MetalnessBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.MetalnessBackMultiplier;
		_TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.RoughnessBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.RoughnessBackMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusionBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionBackMultiplier;
		_TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width] = -SampleNormal(*_TrueUniforms.Material.NormalMapBack, _TrueUniforms.Material.NormalMapBackMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.EmissionBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionBackMultiplier;
	}

	_TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width] = _TrueLerpers.Position;
	_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] = _FragCoord.z;
	_TrueFrameBuffer.Stencil[_X + _Y * _TrueFrameBuffer.Width] = _TrueUniforms.MeshId;
}

struct PBRCubeMapUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	const BSR::Rasterizer::TextureHDR* Environment = nullptr;

	BSR::Math::Mat4f Mvp;
};

struct PBRCubeMapLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
};

static const BSR::Math::Vec4f PBRCubeMapVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const PBRCubeMapUniforms& _TrueUniforms = *(const PBRCubeMapUniforms*)(_Uniforms);
	PBRCubeMapLerpers& _TrueLerpers = *(PBRCubeMapLerpers*)(_OutLerpers);

	_TrueLerpers.Position = _TrueVertex.Position;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

static void PBRCubeMapFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRCubeMapLerpers& _TrueLerpers = *(const PBRCubeMapLerpers*)(_Lerpers);
	const PBRCubeMapUniforms& _TrueUniforms = *(const PBRCubeMapUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (_TrueUniforms.Camera.Perspective)
	{
		_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueLerpers.Position.Normalized())));
	}
	else
	{
		_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueUniforms.CameraForwardVector)));
	}
}

static const float PBRNormalDistributionFunctionGGX(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _HalfWayVec, const float _Roughness)
{
	float _AlphaSquared = _Roughness * _Roughness;
	_AlphaSquared *= _AlphaSquared;

	float _DotNormalHalfWayVecSquare = BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f);
	_DotNormalHalfWayVecSquare *= _DotNormalHalfWayVecSquare;

	float _Numerator = _AlphaSquared;
	float _Denominator = _DotNormalHalfWayVecSquare * (_AlphaSquared - 1.0f) + 1.0f;
	_Denominator = BSR::Math::Pi * _Denominator * _Denominator;

	return _Numerator / _Denominator;
}

static const float PBRGeometrySchlickGGX(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Vec, const float _Roughness)
{
	float _K = _Roughness * _Roughness + 1.0f;
	_K = _K * _K / 8.0f;

	float _Numerator = BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _Vec), 0.0f);
	float _Denominator = _Numerator * (1.0f - _K) + _K;

	return _Numerator / _Denominator;
}

static const float PBRGeometrySmith(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _PositionToLight, const float _Roughness)
{
	return PBRGeometrySchlickGGX(_Normal, _PositionToCamera, _Roughness) * PBRGeometrySchlickGGX(_Normal, _PositionToLight, _Roughness);
}

static const BSR::Math::Vec3f PBRFresnelSchlick(const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _HalfWayVec, const BSR::Math::Vec3f& _Reflectivity)
{
	return _Reflectivity + (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _Reflectivity) * powf(BSR::Math::Clamp(1.0f - BSR::Math::Max(BSR::Math::Vec3f::Dot(_PositionToCamera, _HalfWayVec), 0.0f), 0.0f, 1.0f), 5.0f);
}

static const BSR::Math::Vec3f PBRFresnelSchlickRoughness(const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const float _Roughness)
{
	return _Reflectivity + (BSR::Math::Vec3f::Max(BSR::Math::Vec3f(1.0f - _Roughness, 1.0f - _Roughness, 1.0f - _Roughness), _Reflectivity) - _Reflectivity) * powf(BSR::Math::Clamp(1.0f - BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f), 0.0f, 1.0f), 5.0f);
}

static const BSR::Math::Vec3f PBRDirectionalLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = -_Light.Direction;
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity;

	float _NormalDistributionFunctionGGX = PBRNormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = PBRGeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = PBRFresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::Pi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

static const BSR::Math::Vec3f PBRPointLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Light.Position - _Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = PBRNormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = PBRGeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = PBRFresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::Pi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	return (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
}

static const BSR::Math::Vec3f PBRSpotLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Light.Position - _Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	float _NormalDistributionFunctionGGX = PBRNormalDistributionFunctionGGX(_Normal, _HalfWayVec, _Roughness);
	float _GeometrySmith = PBRGeometrySmith(_Normal, _PositionToCamera, _PositionToLight, _Roughness);
	BSR::Math::Vec3f _FresnelSchlick = PBRFresnelSchlick(_PositionToCamera, _HalfWayVec, _Reflectivity);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlick) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Albedo / BSR::Math::Pi;
	BSR::Math::Vec3f _Specular = _FresnelSchlick * _NormalDistributionFunctionGGX * _GeometrySmith / (4.0f * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f) * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f) + 0.0001f);

	BSR::Math::Vec3f _Result = (_Diffuse + _Specular) * _Radiance * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);

	float _CosTheta = cosf(_Light.Theta);
	float _CosThetaPlusThetaFade = cosf(_Light.Theta + _Light.ThetaFade);
	float _Cos = BSR::Math::Vec3f::Dot(_Light.Direction, -_PositionToLight);

	if (_Cos >= _CosTheta)
	{
		return _Result;
	}

	if (_CosThetaPlusThetaFade >= _Cos)
	{
		return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	}

	float _MixT = BSR::Math::Clamp((_Cos - _CosThetaPlusThetaFade) / (_CosTheta - _CosThetaPlusThetaFade), 0.0f, 1.0f);

	return BSR::Math::Vec3f::Mix(BSR::Math::Vec3f(0.0f, 0.0f, 0.0f), _Result, _MixT);
}

static const BSR::Math::Vec3f PBRImageBasedLightCalculation(const BSR::Rasterizer::TextureHDR& _EnvironmentTexture, const BSR::Rasterizer::TextureHDR& _IrradianceTexture, const BSR::Rasterizer::TextureSDR& _BRDFLookUpTexture, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _FresnelSchlickRoughness = PBRFresnelSchlickRoughness(_Normal, _PositionToCamera, _Reflectivity, _Roughness);
	BSR::Math::Vec3f _kDiffuse = (BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - _FresnelSchlickRoughness) * (1.0f - _Metalness);

	BSR::Math::Vec3f _Irradiance = (BSR::Math::Vec3f)(_IrradianceTexture.Sample(SampleEquirectangularMap(_Normal)));
	BSR::Math::Vec3f _Environment = (BSR::Math::Vec3f)(_EnvironmentTexture.Sample(SampleEquirectangularMap(BSR::Math::Vec3f::Reflect(-_PositionToCamera, _Normal))));
	_Environment = BSR::Math::Vec3f::Mix(_Environment, _Irradiance, _Roughness);
	BSR::Math::Vec2f _BRDFLookUp = (BSR::Math::Vec2f)(_BRDFLookUpTexture.Sample(BSR::Math::Vec2f(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToCamera), 0.0f), _Roughness)));

	BSR::Math::Vec3f _Diffuse = _kDiffuse * _Irradiance * _Albedo;
	BSR::Math::Vec3f _Specular = _Environment * (_FresnelSchlickRoughness * _BRDFLookUp.x + _BRDFLookUp.y);

	return (_Diffuse + _Specular) * _AmbientOcclusion;
}

static const BSR::Math::Vec3f PBRLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Reflectivity, const BSR::Math::Vec3f& _Albedo, const float _Metalness, const float _Roughness, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	switch (_Light.Type)
	{
	case BSR::Renderer::_DirectionalLight:
	{
		return PBRDirectionalLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_PointLight:
	{
		return PBRPointLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_SpotLight:
	{
		return PBRSpotLightCalculation(_Light, _PositionToCamera, _Reflectivity, _Albedo, _Metalness, _Roughness, _AmbientOcclusion, _Normal, _Position);
	}
	default:
	{
		break;
	}
	}

	return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
}

struct PBRCompositUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	uint8_t FogType = BSR::Renderer::_NoFog;
	float FogStart = 0.0f;
	float FogEnd = 0.0f;
	BSR::Math::Vec3f FogColor = BSR::Math::Vec3f(0.8f, 0.8f, 0.8f);

	const BSR::Rasterizer::TextureHDR* Environment = nullptr;
	const BSR::Rasterizer::TextureHDR* Irradiance = nullptr;
	const BSR::Rasterizer::TextureSDR* BRDFLookUp = nullptr;
	const BSR::Vector<BSR::Renderer::Light>* Lights = nullptr;
};

static const BSR::Math::Vec4f PBRCompositVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const PBRCompositUniforms& _TrueUniforms = *(const PBRCompositUniforms*)(_Uniforms);

	return BSR::Math::Vec4f(_TrueVertex.Position * 2.0f, 1.0f);
}

static void PBRCompositFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRCompositUniforms& _TrueUniforms = *(const PBRCompositUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (_TrueFrameBuffer.Stencil[_X + _Y * _TrueFrameBuffer.Width] == 0)
	{
		return;
	}

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

	for (size_t _Index = 0; _Index < _TrueUniforms.Lights->GetSize(); _Index++)
	{
		_Result += PBRLightCalculation((*_TrueUniforms.Lights)[_Index], _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width]);
	}

	_Result += PBRImageBasedLightCalculation(*_TrueUniforms.Environment, *_TrueUniforms.Irradiance, *_TrueUniforms.BRDFLookUp, _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_X + _Y * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width]);

	switch (_TrueUniforms.FogType)
	{
	case BSR::Renderer::_NoFog:
	{
		break;
	}
	case BSR::Renderer::_DistanceBasedFog:
	{
		float _Distance = (_TrueFrameBuffer.Position[_X + _Y * _TrueFrameBuffer.Width] - _TrueUniforms.Camera.Position).Magnitude();

		if (_Distance > _TrueUniforms.FogStart && _Distance < _TrueUniforms.FogEnd)
		{
			_Result = BSR::Math::Vec3f::Mix(_Result, _TrueUniforms.FogColor, (_Distance - _TrueUniforms.FogStart) / (_TrueUniforms.FogEnd - _TrueUniforms.FogStart));
		}
		else if (_Distance >= _TrueUniforms.FogEnd)
		{
			_Result = _TrueUniforms.FogColor;
		}

		break;
	}
	case BSR::Renderer::_DepthBasedFog:
	{
		float _Distance = 0.0f;

		if (_TrueUniforms.Camera.Perspective)
		{
			_Distance = 2.0f * _TrueUniforms.Camera.FarPlane * _TrueUniforms.Camera.NearPlane / (_TrueUniforms.Camera.FarPlane + _TrueUniforms.Camera.NearPlane - (_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * 2.0f - 1.0f) * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane));
		}
		else
		{
			_Distance = _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane) + _TrueUniforms.Camera.NearPlane;
		}

		if (_Distance > _TrueUniforms.FogStart && _Distance < _TrueUniforms.FogEnd)
		{
			_Result = BSR::Math::Vec3f::Mix(_Result, _TrueUniforms.FogColor, (_Distance - _TrueUniforms.FogStart) / (_TrueUniforms.FogEnd - _TrueUniforms.FogStart));
		}
		else if (_Distance >= _TrueUniforms.FogEnd)
		{
			_Result = _TrueUniforms.FogColor;
		}

		break;
	}
	default:
	{
		break;
	}
	}

	_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = _Result;
}

struct PBRGammaAndHDRUniforms
{
	float Exposure = 1.0f;
};

static const BSR::Math::Vec4f PBRGammaAndHDRVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const PBRGammaAndHDRUniforms& _TrueUniforms = *(const PBRGammaAndHDRUniforms*)(_Uniforms);

	return BSR::Math::Vec4f(_TrueVertex.Position * 2.0f, 1.0f);
}

static void PBRGammaAndHDRFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRGammaAndHDRUniforms& _TrueUniforms = *(const PBRGammaAndHDRUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - BSR::Math::Vec3f::Exp(-_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] * _TrueUniforms.Exposure);
	_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f::Pow(_TrueFrameBuffer.Result[_X + _Y * _TrueFrameBuffer.Width], BSR::Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
}



struct BlinnPhongCubeMapUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);

	const BSR::Rasterizer::TextureHDR* Environment = nullptr;

	BSR::Math::Mat4f Mvp;
};

struct BlinnPhongCubeMapLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
};

static const BSR::Math::Vec4f BlinnPhongCubeMapVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const BlinnPhongCubeMapUniforms& _TrueUniforms = *(const BlinnPhongCubeMapUniforms*)(_Uniforms);
	BlinnPhongCubeMapLerpers& _TrueLerpers = *(BlinnPhongCubeMapLerpers*)(_OutLerpers);

	_TrueLerpers.Position = _TrueVertex.Position;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

static void BlinnPhongCubeMapFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongCubeMapLerpers& _TrueLerpers = *(const BlinnPhongCubeMapLerpers*)(_Lerpers);
	const BlinnPhongCubeMapUniforms& _TrueUniforms = *(const BlinnPhongCubeMapUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	if (_TrueUniforms.Camera.Perspective)
	{
		_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueLerpers.Position.Normalized())));
	}
	else
	{
		_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueUniforms.CameraForwardVector)));
	}
}

struct BlinnPhongUniforms
{
	BSR::Renderer::Camera Camera;
	BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	BSR::Renderer::BlinnPhongMaterial Material;
	BSR::Renderer::Transform Transform;

	BSR::Math::Mat4f Model;
	BSR::Math::Mat3f ModelInversedTransposed;
	BSR::Math::Mat4f View;
	BSR::Math::Mat4f Projection;
	BSR::Math::Mat4f Mvp;

	uint8_t FogType = BSR::Renderer::_NoFog;
	float FogStart = 0.0f;
	float FogEnd = 0.0f;
	BSR::Math::Vec3f FogColor = BSR::Math::Vec3f(0.8f, 0.8f, 0.8f);

	const BSR::Vector<BSR::Renderer::Light>* Lights = nullptr;
};

struct BlinnPhongLerpers
{
	BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	BSR::Math::Vec3f Normal = BSR::Math::Vec3f(0.0f, 0.0f, 1.0f);
	BSR::Math::Vec3f Tangent = BSR::Math::Vec3f(1.0f, 0.0f, 0.0f);
	BSR::Math::Vec2f TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
};

static const BSR::Math::Vec4f BlinnPhongVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const BlinnPhongUniforms& _TrueUniforms = *(const BlinnPhongUniforms*)(_Uniforms);
	BlinnPhongLerpers& _TrueLerpers = *(BlinnPhongLerpers*)(_OutLerpers);

	_TrueLerpers.Position = BSR::Math::Vec3f(_TrueUniforms.Model * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f));
	_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;
	_TrueLerpers.Tangent = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Tangent;
	_TrueLerpers.TextureCoords = _TrueVertex.TextureCoords;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

static void BlinnPhongFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongLerpers& _TrueLerpers = *(const BlinnPhongLerpers*)(_Lerpers);
	const BlinnPhongUniforms& _TrueUniforms = *(const BlinnPhongUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] = _FragCoord.z;

	BSR::Math::Vec3f _Color = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	BSR::Math::Vec3f _ColorSpecular = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	float _Shininess = 1.0f;
	float _AmbientOcclusion = 1.0f;
	BSR::Math::Vec3f _Normal = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	BSR::Math::Vec3f _Emission = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);

	if (_FrontFacing)
	{
		_Color = BSR::Math::Vec3f(_TrueUniforms.Material.Color->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorMultiplier;
		_ColorSpecular = BSR::Math::Vec3f(_TrueUniforms.Material.ColorSpecular->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorSpecularMultiplier;
		_Shininess = _TrueUniforms.Material.Shininess->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.ShininessMultiplier;
		_AmbientOcclusion = _TrueUniforms.Material.AmbientOcclusion->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionMultiplier;
		_Normal = SampleNormal(*_TrueUniforms.Material.NormalMap, _TrueUniforms.Material.NormalMapMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_Emission = BSR::Math::Vec3f(_TrueUniforms.Material.Emission->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionMultiplier;
	}
	else
	{
		_Color = BSR::Math::Vec3f(_TrueUniforms.Material.ColorBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorBackMultiplier;
		_ColorSpecular = BSR::Math::Vec3f(_TrueUniforms.Material.ColorSpecularBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorSpecularBackMultiplier;
		_Shininess = _TrueUniforms.Material.ShininessBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.ShininessBackMultiplier;
		_AmbientOcclusion = _TrueUniforms.Material.AmbientOcclusionBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionBackMultiplier;
		_Normal = -SampleNormal(*_TrueUniforms.Material.NormalMapBack, _TrueUniforms.Material.NormalMapBackMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_Emission = BSR::Math::Vec3f(_TrueUniforms.Material.EmissionBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionBackMultiplier;
	}

	BSR::Math::Vec3f _Position = _TrueLerpers.Position;

	BSR::Math::Vec3f _PositionToCamera;

	if (_TrueUniforms.Camera.Perspective)
	{
		_PositionToCamera = (_TrueUniforms.Camera.Position - _Position).Normalized();
	}
	else
	{
		_PositionToCamera = -_TrueUniforms.CameraForwardVector;
	}

	BSR::Math::Vec3f _Result = _Emission;

	for (size_t _Index = 0; _Index < _TrueUniforms.Lights->GetSize(); _Index++)
	{
		//_Result += BlinnPhongLightCalculation((*_TrueUniforms.Lights)[_Index], _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);
	}

	//_Result += BlinnPhongAmbientLightCalculation(_PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);

	switch (_TrueUniforms.FogType)
	{
	case BSR::Renderer::_NoFog:
	{
		break;
	}
	case BSR::Renderer::_DistanceBasedFog:
	{
		float _Distance = (_Position - _TrueUniforms.Camera.Position).Magnitude();

		if (_Distance > _TrueUniforms.FogStart && _Distance < _TrueUniforms.FogEnd)
		{
			_Result = BSR::Math::Vec3f::Mix(_Result, _TrueUniforms.FogColor, (_Distance - _TrueUniforms.FogStart) / (_TrueUniforms.FogEnd - _TrueUniforms.FogStart));
		}
		else if (_Distance >= _TrueUniforms.FogEnd)
		{
			_Result = _TrueUniforms.FogColor;
		}

		break;
	}
	case BSR::Renderer::_DepthBasedFog:
	{
		float _Distance = 0.0f;

		if (_TrueUniforms.Camera.Perspective)
		{
			_Distance = 2.0f * _TrueUniforms.Camera.FarPlane * _TrueUniforms.Camera.NearPlane / (_TrueUniforms.Camera.FarPlane + _TrueUniforms.Camera.NearPlane - (_TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * 2.0f - 1.0f) * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane));
		}
		else
		{
			_Distance = _TrueFrameBuffer.Depth[_X + _Y * _TrueFrameBuffer.Width] * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane) + _TrueUniforms.Camera.NearPlane;
		}

		if (_Distance > _TrueUniforms.FogStart && _Distance < _TrueUniforms.FogEnd)
		{
			_Result = BSR::Math::Vec3f::Mix(_Result, _TrueUniforms.FogColor, (_Distance - _TrueUniforms.FogStart) / (_TrueUniforms.FogEnd - _TrueUniforms.FogStart));
		}
		else if (_Distance >= _TrueUniforms.FogEnd)
		{
			_Result = _TrueUniforms.FogColor;
		}

		break;
	}
	default:
	{
		break;
	}
	}

	_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] = _Result;
}

struct BlinnPhongGammaAndHDRUniforms
{
	float Exposure = 1.0f;
};

static const BSR::Math::Vec4f BlinnPhongGammaAndHDRVertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const BlinnPhongGammaAndHDRUniforms& _TrueUniforms = *(const BlinnPhongGammaAndHDRUniforms*)(_Uniforms);

	return BSR::Math::Vec4f(_TrueVertex.Position * 2.0f, 1.0f);
}

static void BlinnPhongGammaAndHDRFragmentShader(const size_t _X, const size_t _Y, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongGammaAndHDRUniforms& _TrueUniforms = *(const BlinnPhongGammaAndHDRUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - BSR::Math::Vec3f::Exp(-_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] * _TrueUniforms.Exposure);
	_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width] = BSR::Math::Vec3f::Pow(_TrueFrameBuffer.Color[_X + _Y * _TrueFrameBuffer.Width], BSR::Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
}



BSR::Renderer::VertexData::VertexData(const Math::Vec3f& _Position, const Math::Vec3f& _Normal, const Math::Vec3f& _Tangent, const Math::Vec2f& _TextureCoords) : Position(_Position), Normal(_Normal), Tangent(_Tangent), TextureCoords(_TextureCoords)
{

}



BSR::Renderer::IndexData::IndexData(const size_t _IndexA, const size_t _IndexB, const size_t _IndexC) : IndexA(_IndexA), IndexB(_IndexB), IndexC(_IndexC)
{

}



void BSR::Renderer::Mesh::GenerateCube(Mesh& _Mesh)
{
	_Mesh.VBO.Clear();
	_Mesh.IBO.Clear();

	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, 0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, -0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, 0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, -0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, -0.5f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, 0.0f, -1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, -0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, -0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, 0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, -0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, 0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.5f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, -0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, 0.5f), Math::Vec3f(0.0f, -1.0f, 0.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, -0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, 0.5f), Math::Vec3f(0.0f, 1.0f, 0.0f), Math::Vec3f(-1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));

	_Mesh.IBO.PushBack(IndexData(0, 1, 2));
	_Mesh.IBO.PushBack(IndexData(3, 4, 5));
	_Mesh.IBO.PushBack(IndexData(6, 7, 8));
	_Mesh.IBO.PushBack(IndexData(9, 10, 11));
	_Mesh.IBO.PushBack(IndexData(12, 13, 14));
	_Mesh.IBO.PushBack(IndexData(15, 16, 17));
	_Mesh.IBO.PushBack(IndexData(18, 19, 20));
	_Mesh.IBO.PushBack(IndexData(21, 22, 23));
	_Mesh.IBO.PushBack(IndexData(24, 25, 26));
	_Mesh.IBO.PushBack(IndexData(27, 28, 29));
	_Mesh.IBO.PushBack(IndexData(30, 31, 32));
	_Mesh.IBO.PushBack(IndexData(33, 34, 35));
}

void BSR::Renderer::Mesh::GenerateQuad(Mesh& _Mesh)
{
	_Mesh.VBO.Clear();
	_Mesh.IBO.Clear();

	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, -0.5f, 0.0f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(-0.5f, 0.5f, 0.0f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, -0.5f, 0.0f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(Math::Vec3f(0.5f, 0.5f, 0.0f), Math::Vec3f(0.0f, 0.0f, 1.0f), Math::Vec3f(1.0f, 0.0f, 0.0f), Math::Vec2f(1.0f, 1.0f)));

	_Mesh.IBO.PushBack(IndexData(0, 2, 1));
	_Mesh.IBO.PushBack(IndexData(1, 2, 3));
}



//const bool BSR::Renderer::LoadModel(const char* _FileData, const size_t _FileSize, Model& _Model)
//{
//
//}

//char* BSR::Renderer::SaveModel(size_t& _FileSize, const Model& _Model)
//{
//
//}



const BSR::Math::Mat4f BSR::Renderer::Camera::GetViewMatrix() const
{
	return
		Math::Mat4f::GetRotation(-AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetRotation(-AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(-AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) *
		Math::Mat4f::GetTranslation(-Position);
}

const BSR::Math::Mat4f BSR::Renderer::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	if (Perspective)
	{
		return Math::Mat4f::GetPerspective(FieldOfView, _AspectRatio, NearPlane, FarPlane);
	}

	return Math::Mat4f::GetOrtho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f, -NearPlane, -FarPlane);
}

const BSR::Math::Mat4f BSR::Renderer::Camera::GetCubeMapMatrix(const float _AspectRatio) const
{
	Camera _CubeMapCamera;

	_CubeMapCamera.Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
	_CubeMapCamera.AngleFlat = AngleFlat;
	_CubeMapCamera.AngleVertical = AngleVertical;
	_CubeMapCamera.AngleTilt = AngleTilt;
	_CubeMapCamera.Perspective = true;
	if (Perspective)
	{
		_CubeMapCamera.FieldOfView = FieldOfView;
	}
	else
	{
		_CubeMapCamera.FieldOfView = 90.0f * Math::DegreesToRadians;
	}
	_CubeMapCamera.NearPlane = 0.1f;
	_CubeMapCamera.FarPlane = 1000.0f;

	return _CubeMapCamera.GetProjectionMatrix(_AspectRatio) * _CubeMapCamera.GetViewMatrix();
}

const BSR::Math::Vec3f BSR::Renderer::Camera::GetForwardVector() const
{
	return
		Math::Mat3f::GetRotation(AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) *
		Math::Mat3f::GetRotation(AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) *
		Math::Vec3f(0.0f, 0.0f, -1.0f);
}



const BSR::Math::Mat4f BSR::Renderer::Transform::GetModelMatrix() const
{
	return
		Math::Mat4f::GetTranslation(Position) *
		Math::Mat4f::GetRotation(AngleFlat, Math::Vec3f(0.0f, -1.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleVertical, Math::Vec3f(-1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetScale(Scale.x, Scale.y, Scale.z, 1.0f) *
		Math::Mat4f::GetShear(ShearXByY, 0, 1) *
		Math::Mat4f::GetShear(ShearXByZ, 0, 2) *
		Math::Mat4f::GetShear(ShearYByZ, 1, 2) *
		Math::Mat4f::GetShear(ShearYByX, 1, 0) *
		Math::Mat4f::GetShear(ShearZByX, 2, 0) *
		Math::Mat4f::GetShear(ShearZByY, 2, 1);
}



const bool BSR::Renderer::PBRMaterial::HasFrontFace() const
{
	return
		Albedo != nullptr &&
		Metalness != nullptr &&
		Roughness != nullptr &&
		AmbientOcclusion != nullptr &&
		NormalMap != nullptr &&
		Emission != nullptr;
}

const bool BSR::Renderer::PBRMaterial::HasBackFace() const
{
	return
		AlbedoBack != nullptr &&
		MetalnessBack != nullptr &&
		RoughnessBack != nullptr &&
		AmbientOcclusionBack != nullptr &&
		NormalMapBack != nullptr &&
		EmissionBack != nullptr;
}

const uint8_t BSR::Renderer::PBRMaterial::GetCullingType() const
{
	if (HasFrontFace())
	{
		if (HasBackFace())
		{
			return Rasterizer::_NoCulling;
		}

		return Rasterizer::_ClockWiseCulling;
	}

	if (HasBackFace())
	{
		return Rasterizer::_CounterClockWiseCulling;
	}

	return Rasterizer::_NoCulling;
}



const bool BSR::Renderer::PBRFrameBuffer::Valid() const
{
	return
		Width != 0 &&
		Height != 0 &&
		Albedo != nullptr &&
		Metalness != nullptr &&
		Roughness != nullptr &&
		AmbientOcclusion != nullptr &&
		NormalMap != nullptr &&
		Emission != nullptr &&
		Position != nullptr &&
		Depth != nullptr &&
		Stencil != nullptr &&
		Result != nullptr;
}



BSR::Renderer::PBRContext::PBRContext() : TargetFrameBuffer(), TargetCamera(), TargetExposure(1.0f), TargetFogType(_NoFog), TargetFogStart(0.0f), TargetFogEnd(0.0f), TargetFogColor(Math::Vec3f(0.8f, 0.8f, 0.8f)), TargetEnvironment(nullptr), TargetIrradiance(nullptr), TargetBRDFLookUp(nullptr), TargetMeshes(), TargetMaterials(), TargetTransforms(), TargetLights()
{

}

BSR::Renderer::PBRContext::PBRContext(PBRContext&& _Other) noexcept : TargetFrameBuffer(_Other.TargetFrameBuffer), TargetCamera(_Other.TargetCamera), TargetExposure(_Other.TargetExposure), TargetFogType(_Other.TargetFogType), TargetFogStart(_Other.TargetFogStart), TargetFogEnd(_Other.TargetFogEnd), TargetFogColor((Math::Vec3f&&)(_Other.TargetFogColor)), TargetEnvironment(_Other.TargetEnvironment), TargetIrradiance(_Other.TargetIrradiance), TargetBRDFLookUp(_Other.TargetBRDFLookUp), TargetMeshes((Vector<const Mesh*>&&)(_Other.TargetMeshes)), TargetMaterials((Vector<PBRMaterial>&&)(_Other.TargetMaterials)), TargetTransforms((Vector<Transform>&&)(_Other.TargetTransforms)), TargetLights((Vector<Light>&&)(_Other.TargetLights))
{
	_Other.TargetFrameBuffer = PBRFrameBuffer();
	_Other.TargetCamera = Camera();
	_Other.TargetExposure = 1.0f;
	_Other.TargetFogType = _NoFog;
	_Other.TargetFogStart = 0.0f;
	_Other.TargetFogEnd = 0.0f;
	_Other.TargetEnvironment = nullptr;
	_Other.TargetIrradiance = nullptr;
	_Other.TargetBRDFLookUp = nullptr;
}

BSR::Renderer::PBRContext::~PBRContext()
{

}

void BSR::Renderer::PBRContext::StartScene(PBRFrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const uint8_t _TargetFogType, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const Rasterizer::TextureHDR& _TargetEnvironment, const Rasterizer::TextureHDR& _TargetIrradiance, const Rasterizer::TextureSDR& _TargetBRDFLookUp)
{
	TargetFrameBuffer = PBRFrameBuffer();
	TargetCamera = Camera();
	TargetExposure = 1.0f;
	TargetFogType = _NoFog;
	TargetFogStart = 0.0f;
	TargetFogEnd = 0.0f;
	TargetFogColor = Math::Vec3f(8.0f, 8.0f, 8.0f);
	TargetEnvironment = nullptr;
	TargetIrradiance = nullptr;
	TargetBRDFLookUp = nullptr;
	TargetMeshes.Clear();
	TargetMaterials.Clear();
	TargetTransforms.Clear();
	TargetLights.Clear();

	if (!_TargetFrameBuffer.Valid())
	{
		return;
	}

	TargetFrameBuffer = _TargetFrameBuffer;
	TargetCamera = _TargetCamera;
	TargetExposure = _TargetExposure;
	TargetFogType = _TargetFogType;
	TargetFogStart = _TargetFogStart;
	TargetFogEnd = _TargetFogEnd;
	TargetFogColor = _TargetFogColor;
	TargetEnvironment = &_TargetEnvironment;
	TargetIrradiance = &_TargetIrradiance;
	TargetBRDFLookUp = &_TargetBRDFLookUp;
}

void BSR::Renderer::PBRContext::FlushScene()
{
	if (!TargetFrameBuffer.Valid())
	{
		return;
	}

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

	_Context.ViewPortX = 0;
	_Context.ViewPortY = 0;
	_Context.ViewPortWidth = TargetFrameBuffer.Width;
	_Context.ViewPortHeight = TargetFrameBuffer.Height;

	_Context.DepthTestingType = Rasterizer::_LowerOrEqualDepthTesting;

	for (size_t _Index = 0; _Index < TargetMeshes.GetSize(); _Index++)
	{
		PBRDeferredUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = _Uniforms.Camera.GetForwardVector();
		_Uniforms.MeshId = (uint64_t)(_Index) + 1;
		_Uniforms.Material = TargetMaterials[_Index];
		_Uniforms.Transform = TargetTransforms[_Index];
		_Uniforms.Model = _Uniforms.Transform.GetModelMatrix();
		_Uniforms.ModelInversedTransposed = Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
		_Uniforms.View = _Uniforms.Camera.GetViewMatrix();
		_Uniforms.Projection = _Uniforms.Camera.GetProjectionMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;

		_Context.CullingType = TargetMaterials[_Index].GetCullingType();

		_Context.VBO = TargetMeshes[_Index]->VBO.GetData();
		_Context.VBOSize = TargetMeshes[_Index]->VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(TargetMeshes[_Index]->IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = TargetMeshes[_Index]->IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = sizeof(PBRDeferredLerpers) / sizeof(float);
		_Context.LerpersCountGeomToFrag = sizeof(PBRDeferredLerpers) / sizeof(float);
		_Context.VertexShader = PBRDeferredVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = PBRDeferredFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	_Context.DepthTestingType = Rasterizer::_NoDepthTesting;
	_Context.CullingType = Rasterizer::_CounterClockWiseCulling;

	{
		PBRCubeMapUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = TargetCamera.GetForwardVector();
		_Uniforms.Environment = TargetEnvironment;
		_Uniforms.Mvp = TargetCamera.GetCubeMapMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));

		_Context.VBO = _Cube.VBO.GetData();
		_Context.VBOSize = _Cube.VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(_Cube.IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = _Cube.IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = sizeof(PBRCubeMapLerpers) / sizeof(float);
		_Context.LerpersCountGeomToFrag = sizeof(PBRCubeMapLerpers) / sizeof(float);
		_Context.VertexShader = PBRCubeMapVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = PBRCubeMapFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	_Context.DepthTestingType = Rasterizer::_NoDepthTesting;
	_Context.CullingType = Rasterizer::_ClockWiseCulling;

	{
		PBRCompositUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = TargetCamera.GetForwardVector();
		_Uniforms.FogType = TargetFogType;
		_Uniforms.FogStart = TargetFogStart;
		_Uniforms.FogEnd = TargetFogEnd;
		_Uniforms.FogColor = TargetFogColor;
		_Uniforms.Environment = TargetEnvironment;
		_Uniforms.Irradiance = TargetIrradiance;
		_Uniforms.BRDFLookUp = TargetBRDFLookUp;
		_Uniforms.Lights = &TargetLights;

		_Context.VBO = _Quad.VBO.GetData();
		_Context.VBOSize = _Quad.VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(_Quad.IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = _Quad.IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = 0;
		_Context.LerpersCountGeomToFrag = 0;
		_Context.VertexShader = PBRCompositVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = PBRCompositFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	_Context.DepthTestingType = Rasterizer::_NoDepthTesting;
	_Context.CullingType = Rasterizer::_ClockWiseCulling;

	{
		PBRGammaAndHDRUniforms _Uniforms;

		_Uniforms.Exposure = TargetExposure;

		_Context.VBO = _Quad.VBO.GetData();
		_Context.VBOSize = _Quad.VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(_Quad.IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = _Quad.IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = 0;
		_Context.LerpersCountGeomToFrag = 0;
		_Context.VertexShader = PBRGammaAndHDRVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = PBRGammaAndHDRFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	TargetFrameBuffer = PBRFrameBuffer();
	TargetCamera = Camera();
	TargetExposure = 1.0f;
	TargetFogType = _NoFog;
	TargetFogStart = 0.0f;
	TargetFogEnd = 0.0f;
	TargetFogColor = Math::Vec3f(8.0f, 8.0f, 8.0f);
	TargetEnvironment = nullptr;
	TargetIrradiance = nullptr;
	TargetBRDFLookUp = nullptr;
	TargetMeshes.Clear();
	TargetMaterials.Clear();
	TargetTransforms.Clear();
	TargetLights.Clear();
}

void BSR::Renderer::PBRContext::SubmitMesh(const Mesh& _TargetMesh, const PBRMaterial& _TargetMaterial, const Transform& _TargetTransform)
{
	TargetMeshes.PushBack(&_TargetMesh);
	TargetMaterials.PushBack(_TargetMaterial);
	TargetTransforms.PushBack(_TargetTransform);
}

void BSR::Renderer::PBRContext::SubmitLight(const Light& _TargetLight)
{
	TargetLights.PushBack(_TargetLight);
}

BSR::Renderer::PBRContext& BSR::Renderer::PBRContext::operator= (PBRContext&& _Other) noexcept
{
	TargetFrameBuffer = _Other.TargetFrameBuffer;
	TargetCamera = _Other.TargetCamera;
	TargetExposure = _Other.TargetExposure;
	TargetFogType = _Other.TargetFogType;
	TargetFogStart = _Other.TargetFogStart;
	TargetFogEnd = _Other.TargetFogEnd;
	TargetFogColor = (Math::Vec3f&&)(_Other.TargetFogColor);
	TargetEnvironment = _Other.TargetEnvironment;
	TargetIrradiance = _Other.TargetIrradiance;
	TargetBRDFLookUp = _Other.TargetBRDFLookUp;
	TargetMeshes = (Vector<const Mesh*>&&)(_Other.TargetMeshes);
	TargetMaterials = (Vector<PBRMaterial>&&)(_Other.TargetMaterials);
	TargetTransforms = (Vector<Transform>&&)(_Other.TargetTransforms);
	TargetLights = (Vector<Light>&&)(_Other.TargetLights);

	_Other.TargetFrameBuffer = PBRFrameBuffer();
	_Other.TargetCamera = Camera();
	_Other.TargetExposure = 1.0f;
	_Other.TargetFogType = _NoFog;
	_Other.TargetFogStart = 0.0f;
	_Other.TargetFogEnd = 0.0f;
	_Other.TargetEnvironment = nullptr;
	_Other.TargetIrradiance = nullptr;
	_Other.TargetBRDFLookUp = nullptr;

	return *this;
}



const bool BSR::Renderer::BlinnPhongMaterial::HasFrontFace() const
{
	return
		Color != nullptr &&
		ColorSpecular != nullptr &&
		Shininess != nullptr &&
		AmbientOcclusion != nullptr &&
		NormalMap != nullptr &&
		Emission != nullptr;
}

const bool BSR::Renderer::BlinnPhongMaterial::HasBackFace() const
{
	return
		Color != nullptr &&
		ColorSpecular != nullptr &&
		Shininess != nullptr &&
		AmbientOcclusionBack != nullptr &&
		NormalMapBack != nullptr &&
		EmissionBack != nullptr;
}

const uint8_t BSR::Renderer::BlinnPhongMaterial::GetCullingType() const
{
	if (HasFrontFace())
	{
		if (HasBackFace())
		{
			return Rasterizer::_NoCulling;
		}

		return Rasterizer::_ClockWiseCulling;
	}

	if (HasBackFace())
	{
		return Rasterizer::_CounterClockWiseCulling;
	}

	return Rasterizer::_NoCulling;
}



const bool BSR::Renderer::BlinnPhongFrameBuffer::Valid() const
{
	return
		Width != 0 &&
		Height != 0 &&
		Depth != nullptr &&
		Color != nullptr;
}



BSR::Renderer::BlinnPhongContext::BlinnPhongContext() : TargetFrameBuffer(), TargetCamera(), TargetExposure(1.0f), TargetFogType(_NoFog), TargetFogStart(0.0f), TargetFogEnd(0.0f), TargetFogColor(Math::Vec3f(0.8f, 0.8f, 0.8f)), TargetEnvironment(nullptr), TargetMeshes(), TargetMaterials(), TargetTransforms(), TargetLights()
{

}

BSR::Renderer::BlinnPhongContext::BlinnPhongContext(BlinnPhongContext&& _Other) noexcept : TargetFrameBuffer(_Other.TargetFrameBuffer), TargetCamera(_Other.TargetCamera), TargetExposure(_Other.TargetExposure), TargetFogType(_Other.TargetFogType), TargetFogStart(_Other.TargetFogStart), TargetFogEnd(_Other.TargetFogEnd), TargetFogColor((Math::Vec3f&&)(_Other.TargetFogColor)), TargetEnvironment(_Other.TargetEnvironment), TargetMeshes((Vector<const Mesh*>&&)(_Other.TargetMeshes)), TargetMaterials((Vector<BlinnPhongMaterial>&&)(_Other.TargetMaterials)), TargetTransforms((Vector<Transform>&&)(_Other.TargetTransforms)), TargetLights((Vector<Light>&&)(_Other.TargetLights))
{
	_Other.TargetFrameBuffer = BlinnPhongFrameBuffer();
	_Other.TargetCamera = Camera();
	_Other.TargetExposure = 1.0f;
	_Other.TargetFogType = _NoFog;
	_Other.TargetFogStart = 0.0f;
	_Other.TargetFogEnd = 0.0f;
	_Other.TargetEnvironment = nullptr;
}

BSR::Renderer::BlinnPhongContext::~BlinnPhongContext()
{

}

void BSR::Renderer::BlinnPhongContext::StartScene(BlinnPhongFrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const uint8_t _TargetFogType, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const Rasterizer::TextureHDR* _TargetEnvironment)
{
	TargetFrameBuffer = BlinnPhongFrameBuffer();
	TargetCamera = Camera();
	TargetExposure = 1.0f;
	TargetFogType = _NoFog;
	TargetFogStart = 0.0f;
	TargetFogEnd = 0.0f;
	TargetFogColor = Math::Vec3f(8.0f, 8.0f, 8.0f);
	TargetEnvironment = nullptr;
	TargetMeshes.Clear();
	TargetMaterials.Clear();
	TargetTransforms.Clear();
	TargetLights.Clear();

	if (!_TargetFrameBuffer.Valid())
	{
		return;
	}

	TargetFrameBuffer = _TargetFrameBuffer;
	TargetCamera = _TargetCamera;
	TargetExposure = _TargetExposure;
	TargetFogType = _TargetFogType;
	TargetFogStart = _TargetFogStart;
	TargetFogEnd = _TargetFogEnd;
	TargetFogColor = _TargetFogColor;
	TargetEnvironment = _TargetEnvironment;
}

void BSR::Renderer::BlinnPhongContext::FlushScene()
{
	if (!TargetFrameBuffer.Valid())
	{
		return;
	}

	Mesh _Cube;
	Mesh::GenerateCube(_Cube);
	Mesh _Quad;
	Mesh::GenerateQuad(_Quad);

	Rasterizer::Context _Context;

	for (size_t _Y = 0; _Y < TargetFrameBuffer.Height; _Y++)
	{
		for (size_t _X = 0; _X < TargetFrameBuffer.Width; _X++)
		{
			TargetFrameBuffer.Depth[_X + _Y * TargetFrameBuffer.Width] = 1.0f;
			TargetFrameBuffer.Color[_X + _Y * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
		}
	}

	_Context.ViewPortX = 0;
	_Context.ViewPortY = 0;
	_Context.ViewPortWidth = TargetFrameBuffer.Width;
	_Context.ViewPortHeight = TargetFrameBuffer.Height;

	_Context.DepthTestingType = Rasterizer::_NoDepthTesting;
	_Context.CullingType = Rasterizer::_CounterClockWiseCulling;

	if (TargetEnvironment != nullptr)
	{
		BlinnPhongCubeMapUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = TargetCamera.GetForwardVector();
		_Uniforms.Environment = TargetEnvironment;
		_Uniforms.Mvp = TargetCamera.GetCubeMapMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));

		_Context.VBO = _Cube.VBO.GetData();
		_Context.VBOSize = _Cube.VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(_Cube.IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = _Cube.IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = sizeof(BlinnPhongCubeMapLerpers) / sizeof(float);
		_Context.LerpersCountGeomToFrag = sizeof(BlinnPhongCubeMapLerpers) / sizeof(float);
		_Context.VertexShader = BlinnPhongCubeMapVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = BlinnPhongCubeMapFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	_Context.DepthTestingType = Rasterizer::_LowerOrEqualDepthTesting;

	for (size_t _Index = 0; _Index < TargetMeshes.GetSize(); _Index++)
	{
		BlinnPhongUniforms _Uniforms;

		_Uniforms.Camera = TargetCamera;
		_Uniforms.CameraForwardVector = _Uniforms.Camera.GetForwardVector();
		_Uniforms.Material = TargetMaterials[_Index];
		_Uniforms.Transform = TargetTransforms[_Index];
		_Uniforms.Model = _Uniforms.Transform.GetModelMatrix();
		_Uniforms.ModelInversedTransposed = Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
		_Uniforms.View = _Uniforms.Camera.GetViewMatrix();
		_Uniforms.Projection = _Uniforms.Camera.GetProjectionMatrix((float)(TargetFrameBuffer.Width) / (float)(TargetFrameBuffer.Height));
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;
		_Uniforms.FogType = TargetFogType;
		_Uniforms.FogStart = TargetFogStart;
		_Uniforms.FogEnd = TargetFogEnd;
		_Uniforms.FogColor = TargetFogColor;
		_Uniforms.Lights = &TargetLights;

		_Context.CullingType = TargetMaterials[_Index].GetCullingType();

		_Context.VBO = TargetMeshes[_Index]->VBO.GetData();
		_Context.VBOSize = TargetMeshes[_Index]->VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(TargetMeshes[_Index]->IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = TargetMeshes[_Index]->IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = sizeof(BlinnPhongLerpers) / sizeof(float);
		_Context.LerpersCountGeomToFrag = sizeof(BlinnPhongLerpers) / sizeof(float);
		_Context.VertexShader = BlinnPhongVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = BlinnPhongFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	_Context.DepthTestingType = Rasterizer::_NoDepthTesting;
	_Context.CullingType = Rasterizer::_ClockWiseCulling;

	{
		BlinnPhongGammaAndHDRUniforms _Uniforms;

		_Uniforms.Exposure = TargetExposure;

		_Context.VBO = _Quad.VBO.GetData();
		_Context.VBOSize = _Quad.VBO.GetSize();
		_Context.VBOStride = sizeof(VertexData);
		_Context.IBO = (size_t*)(_Quad.IBO.GetData());
		_Context.IBOBegin = 0;
		_Context.IBOEnd = _Quad.IBO.GetSize() * 3;
		_Context.Uniforms = &_Uniforms;
		_Context.LerpersCountVertToGeom = 0;
		_Context.LerpersCountGeomToFrag = 0;
		_Context.VertexShader = BlinnPhongGammaAndHDRVertexShader;
		_Context.GeometryShader = nullptr;
		_Context.FragmentShader = BlinnPhongGammaAndHDRFragmentShader;
		_Context.FrameBuffer = &TargetFrameBuffer;
		_Context.DrawCall();
	}

	TargetFrameBuffer = BlinnPhongFrameBuffer();
	TargetCamera = Camera();
	TargetExposure = 1.0f;
	TargetFogType = _NoFog;
	TargetFogStart = 0.0f;
	TargetFogEnd = 0.0f;
	TargetFogColor = Math::Vec3f(8.0f, 8.0f, 8.0f);
	TargetEnvironment = nullptr;
	TargetMeshes.Clear();
	TargetMaterials.Clear();
	TargetTransforms.Clear();
	TargetLights.Clear();
}

void BSR::Renderer::BlinnPhongContext::SubmitMesh(const Mesh& _TargetMesh, const BlinnPhongMaterial& _TargetMaterial, const Transform& _TargetTransform)
{
	TargetMeshes.PushBack(&_TargetMesh);
	TargetMaterials.PushBack(_TargetMaterial);
	TargetTransforms.PushBack(_TargetTransform);
}

void BSR::Renderer::BlinnPhongContext::SubmitLight(const Light& _TargetLight)
{
	TargetLights.PushBack(_TargetLight);
}

BSR::Renderer::BlinnPhongContext& BSR::Renderer::BlinnPhongContext::operator= (BlinnPhongContext&& _Other) noexcept
{
	TargetFrameBuffer = _Other.TargetFrameBuffer;
	TargetCamera = _Other.TargetCamera;
	TargetExposure = _Other.TargetExposure;
	TargetFogType = _Other.TargetFogType;
	TargetFogStart = _Other.TargetFogStart;
	TargetFogEnd = _Other.TargetFogEnd;
	TargetFogColor = (Math::Vec3f&&)(_Other.TargetFogColor);
	TargetEnvironment = _Other.TargetEnvironment;
	TargetMeshes = (Vector<const Mesh*>&&)(_Other.TargetMeshes);
	TargetMaterials = (Vector<BlinnPhongMaterial>&&)(_Other.TargetMaterials);
	TargetTransforms = (Vector<Transform>&&)(_Other.TargetTransforms);
	TargetLights = (Vector<Light>&&)(_Other.TargetLights);

	_Other.TargetFrameBuffer = BlinnPhongFrameBuffer();
	_Other.TargetCamera = Camera();
	_Other.TargetExposure = 1.0f;
	_Other.TargetFogType = _NoFog;
	_Other.TargetFogStart = 0.0f;
	_Other.TargetFogEnd = 0.0f;
	_Other.TargetEnvironment = nullptr;

	return *this;
}
