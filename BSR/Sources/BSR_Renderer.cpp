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

	BSR::Math::Vec3f _NewNormal = _Normal.Normalized();
	BSR::Math::Vec3f _NewTangent = _Tangent.Normalized();
	_NewTangent = (_NewTangent - _NewNormal * BSR::Math::Vec3f::Dot(_NewNormal, _NewTangent)).Normalized();
	BSR::Math::Vec3f _NewBitangent = BSR::Math::Vec3f::Cross(_NewNormal, _NewTangent);

	BSR::Math::Mat3f _TBN;

	_TBN[0][0] = _NewTangent.x; _TBN[0][1] = _NewBitangent.x; _TBN[0][2] = _NewNormal.x;
	_TBN[1][0] = _NewTangent.y; _TBN[1][1] = _NewBitangent.y; _TBN[1][2] = _NewNormal.y;
	_TBN[2][0] = _NewTangent.z; _TBN[2][1] = _NewBitangent.z; _TBN[2][2] = _NewNormal.z;

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

static void PBRDeferredFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRDeferredLerpers& _TrueLerpers = *(const PBRDeferredLerpers*)(_Lerpers);
	const PBRDeferredUniforms& _TrueUniforms = *(const PBRDeferredUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	if (_FrontFacing)
	{
		_TrueFrameBuffer.Albedo[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.Albedo->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.AlbedoMultiplier;
		_TrueFrameBuffer.Metalness[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Metalness->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.MetalnessMultiplier;
		_TrueFrameBuffer.Roughness[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.Roughness->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.RoughnessMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusion->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionMultiplier;
		_TrueFrameBuffer.NormalMap[_XPos + _YPos * _TrueFrameBuffer.Width] = SampleNormal(*_TrueUniforms.Material.NormalMap, _TrueUniforms.Material.NormalMapMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.Emission->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionMultiplier;
	}
	else
	{
		_TrueFrameBuffer.Albedo[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.AlbedoBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.AlbedoBackMultiplier;
		_TrueFrameBuffer.Metalness[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.MetalnessBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.MetalnessBackMultiplier;
		_TrueFrameBuffer.Roughness[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.RoughnessBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.RoughnessBackMultiplier;
		_TrueFrameBuffer.AmbientOcclusion[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.Material.AmbientOcclusionBack->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionBackMultiplier;
		_TrueFrameBuffer.NormalMap[_XPos + _YPos * _TrueFrameBuffer.Width] = -SampleNormal(*_TrueUniforms.Material.NormalMapBack, _TrueUniforms.Material.NormalMapBackMultiplier, _TrueLerpers.Normal, _TrueLerpers.Tangent, _TrueLerpers.TextureCoords);
		_TrueFrameBuffer.Emission[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Material.EmissionBack->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.EmissionBackMultiplier;
	}

	_TrueFrameBuffer.Position[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueLerpers.Position;
	_TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] = _FragCoord.z;
	_TrueFrameBuffer.Stencil[_XPos + _YPos * _TrueFrameBuffer.Width] = _TrueUniforms.MeshId;
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

static void PBRCubeMapFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRCubeMapLerpers& _TrueLerpers = *(const PBRCubeMapLerpers*)(_Lerpers);
	const PBRCubeMapUniforms& _TrueUniforms = *(const PBRCubeMapUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (_TrueUniforms.Camera.Perspective)
	{
		_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueLerpers.Position.Normalized())));
	}
	else
	{
		_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueUniforms.CameraForwardVector)));
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

static void PBRCompositFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRCompositUniforms& _TrueUniforms = *(const PBRCompositUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	if (_TrueFrameBuffer.Stencil[_XPos + _YPos * _TrueFrameBuffer.Width] == 0)
	{
		return;
	}

	BSR::Math::Vec3f _PositionToCamera;

	if (_TrueUniforms.Camera.Perspective)
	{
		_PositionToCamera = (_TrueUniforms.Camera.Position - _TrueFrameBuffer.Position[_XPos + _YPos * _TrueFrameBuffer.Width]).Normalized();
	}
	else
	{
		_PositionToCamera = -_TrueUniforms.CameraForwardVector;
	}

	BSR::Math::Vec3f _Reflectivity = BSR::Math::Vec3f::Mix(BSR::Math::Vec3f(0.04f, 0.04f, 0.04f), _TrueFrameBuffer.Albedo[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_XPos + _YPos * _TrueFrameBuffer.Width]);

	BSR::Math::Vec3f _Result = _TrueFrameBuffer.Emission[_XPos + _YPos * _TrueFrameBuffer.Width];

	for (size_t _Index = 0; _Index < _TrueUniforms.Lights->GetSize(); _Index++)
	{
		_Result += PBRLightCalculation((*_TrueUniforms.Lights)[_Index], _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_XPos + _YPos * _TrueFrameBuffer.Width]);
	}

	_Result += PBRImageBasedLightCalculation(*_TrueUniforms.Environment, *_TrueUniforms.Irradiance, *_TrueUniforms.BRDFLookUp, _PositionToCamera, _Reflectivity, _TrueFrameBuffer.Albedo[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Metalness[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Roughness[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.AmbientOcclusion[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.NormalMap[_XPos + _YPos * _TrueFrameBuffer.Width], _TrueFrameBuffer.Position[_XPos + _YPos * _TrueFrameBuffer.Width]);

	switch (_TrueUniforms.FogType)
	{
	case BSR::Renderer::_NoFog:
	{
		break;
	}
	case BSR::Renderer::_DistanceBasedFog:
	{
		float _Distance = (_TrueFrameBuffer.Position[_XPos + _YPos * _TrueFrameBuffer.Width] - _TrueUniforms.Camera.Position).Magnitude();

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
			_Distance = 2.0f * _TrueUniforms.Camera.FarPlane * _TrueUniforms.Camera.NearPlane / (_TrueUniforms.Camera.FarPlane + _TrueUniforms.Camera.NearPlane - (_TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] * 2.0f - 1.0f) * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane));
		}
		else
		{
			_Distance = _TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane) + _TrueUniforms.Camera.NearPlane;
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

	_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] = _Result;
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

static void PBRGammaAndHDRFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const PBRGammaAndHDRUniforms& _TrueUniforms = *(const PBRGammaAndHDRUniforms*)(_Uniforms);
	BSR::Renderer::PBRFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::PBRFrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - BSR::Math::Vec3f::Exp(-_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] * _TrueUniforms.Exposure);
	_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f::Pow(_TrueFrameBuffer.Result[_XPos + _YPos * _TrueFrameBuffer.Width], BSR::Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
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

static void BlinnPhongCubeMapFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongCubeMapLerpers& _TrueLerpers = *(const BlinnPhongCubeMapLerpers*)(_Lerpers);
	const BlinnPhongCubeMapUniforms& _TrueUniforms = *(const BlinnPhongCubeMapUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	if (_TrueUniforms.Camera.Perspective)
	{
		_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueLerpers.Position.Normalized())));
	}
	else
	{
		_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(_TrueUniforms.Environment->Sample(SampleEquirectangularMap(_TrueUniforms.CameraForwardVector)));
	}
}

static const BSR::Math::Vec3f BlinnPhongDirectionalLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Color, const BSR::Math::Vec3f& _ColorSpecular, const float _Shininess, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = -_Light.Direction;
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity;

	BSR::Math::Vec3f _Diffuse = _Color * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
	BSR::Math::Vec3f _Specular = _ColorSpecular * powf(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f), _Shininess);

	return (_Diffuse + _Specular) * _Radiance;
}

static const BSR::Math::Vec3f BlinnPhongPointLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Color, const BSR::Math::Vec3f& _ColorSpecular, const float _Shininess, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Light.Position - _Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	BSR::Math::Vec3f _Diffuse = _Color * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
	BSR::Math::Vec3f _Specular = _ColorSpecular * powf(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f), _Shininess);

	return (_Diffuse + _Specular) * _Radiance;
}

static const BSR::Math::Vec3f BlinnPhongSpotLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Color, const BSR::Math::Vec3f& _ColorSpecular, const float _Shininess, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	BSR::Math::Vec3f _PositionToLight = (_Light.Position - _Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();
	float _Distance = (_Light.Position - _Position).Magnitude();
	float _Attenuation = 1.0f / (_Distance * _Distance);
	BSR::Math::Vec3f _Radiance = _Light.Color * _Light.Intensity * _Attenuation;

	BSR::Math::Vec3f _Diffuse = _Color * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
	BSR::Math::Vec3f _Specular = _ColorSpecular * powf(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f), _Shininess);

	BSR::Math::Vec3f _Result = (_Diffuse + _Specular) * _Radiance;

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

static const BSR::Math::Vec3f BlinnPhongAmbientLightCalculation(const BSR::Math::Vec3f _AmbientLight, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Color, const BSR::Math::Vec3f _ColorSpecular, const float _Shininess, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	return (_Color + _ColorSpecular) * _AmbientLight * _AmbientOcclusion;
}

static const BSR::Math::Vec3f BlinnPhongLightCalculation(const BSR::Renderer::Light& _Light, const BSR::Math::Vec3f& _PositionToCamera, const BSR::Math::Vec3f& _Color, const BSR::Math::Vec3f& _ColorSpecular, const float _Shininess, const float _AmbientOcclusion, const BSR::Math::Vec3f& _Normal, const BSR::Math::Vec3f& _Position)
{
	switch (_Light.Type)
	{
	case BSR::Renderer::_DirectionalLight:
	{
		return BlinnPhongDirectionalLightCalculation(_Light, _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_PointLight:
	{
		return BlinnPhongPointLightCalculation(_Light, _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);
	}
	case BSR::Renderer::_SpotLight:
	{
		return BlinnPhongSpotLightCalculation(_Light, _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);
	}
	default:
	{
		break;
	}
	}

	return BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
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

	BSR::Math::Vec3f AmbientLight = BSR::Math::Vec3f(0.1f, 0.1f, 0.1f);
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

static void BlinnPhongFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongLerpers& _TrueLerpers = *(const BlinnPhongLerpers*)(_Lerpers);
	const BlinnPhongUniforms& _TrueUniforms = *(const BlinnPhongUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	_TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] = _FragCoord.z;

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
		_Result += BlinnPhongLightCalculation((*_TrueUniforms.Lights)[_Index], _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);
	}

	_Result += BlinnPhongAmbientLightCalculation(_TrueUniforms.AmbientLight, _PositionToCamera, _Color, _ColorSpecular, _Shininess, _AmbientOcclusion, _Normal, _Position);

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
			_Distance = 2.0f * _TrueUniforms.Camera.FarPlane * _TrueUniforms.Camera.NearPlane / (_TrueUniforms.Camera.FarPlane + _TrueUniforms.Camera.NearPlane - (_TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] * 2.0f - 1.0f) * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane));
		}
		else
		{
			_Distance = _TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] * (_TrueUniforms.Camera.FarPlane - _TrueUniforms.Camera.NearPlane) + _TrueUniforms.Camera.NearPlane;
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

	_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = _Result;
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

static void BlinnPhongGammaAndHDRFragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const BlinnPhongGammaAndHDRUniforms& _TrueUniforms = *(const BlinnPhongGammaAndHDRUniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) - BSR::Math::Vec3f::Exp(-_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] * _TrueUniforms.Exposure);
	_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Math::Vec3f::Pow(_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width], BSR::Math::Vec3f(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
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



const bool BSR::Renderer::LoadModel(const char* _FileData, const size_t _FileSize, Model& _Model)
{
	struct MeshFileData
	{
		char* Name = nullptr;
		size_t FacesStart = (size_t)(-1);
		size_t FacesEnd = (size_t)(-1);
	};

	struct FaceVertex
	{
		size_t Position = (size_t)(-1);
		size_t Normal = (size_t)(-1);
		size_t TextureCoords = (size_t)(-1);
	};

	for (size_t _Index = 0; _Index < _Model.GetSize(); _Index++)
	{
		delete[] _Model[_Index].Name;
	}

	_Model.Clear();

	if (!_FileData || !_FileSize)
	{
		return false;
	}

	BSR::Vector<MeshFileData> _Meshes;

	BSR::Vector<BSR::Math::Vec3f> _Positions;
	BSR::Vector<BSR::Math::Vec3f> _Normals;
	BSR::Vector<BSR::Math::Vec2f> _TextureCoords;

	BSR::Vector<BSR::Vector<FaceVertex>> _Faces;

	BSR::Vector<BSR::Vector<char>> _Lines;

	{
		_Lines.EmplaceBack(BSR::Vector<char>());

		size_t _Index = 0;

		while (_Index < _FileSize)
		{
			if (_FileData[_Index] == '\n')
			{
				_Lines[_Lines.GetSize() - 1].PushBack('\0');
				_Lines.EmplaceBack(BSR::Vector<char>());
				_Index++;
				continue;
			}

			_Lines[_Lines.GetSize() - 1].PushBack(_FileData[_Index]);
			_Index++;
		}

		_Lines.Erase(_Lines.GetSize() - 1);
	}

	for (size_t _LineIndex = 0; _LineIndex < _Lines.GetSize(); _LineIndex++)
	{
		BSR::Vector<char*> _Tokens;

		for (size_t _Index = 0; _Index < _Lines[_LineIndex].GetSize() - 1; _Index++)
		{
			if (_Lines[_LineIndex][_Index] == ' ' || _Lines[_LineIndex][_Index] == '\t')
			{
				_Lines[_LineIndex][_Index] = '\0';
			}
		}

		if (_Lines[_LineIndex][0] != '\0')
		{
			_Tokens.PushBack(&_Lines[_LineIndex][0]);
		}

		for (size_t _Index = 1; _Index < _Lines[_LineIndex].GetSize() - 1; _Index++)
		{
			if (_Lines[_LineIndex][_Index] != '\0' && _Lines[_LineIndex][_Index - 1] == '\0')
			{
				_Tokens.PushBack(&_Lines[_LineIndex][_Index]);
			}
		}

		if (!_Tokens.GetSize())
		{
			continue;
		}

		if (_Tokens[0][0] == '#')
		{
			continue;
		}

		if (strcmp(_Tokens[0], "o") == 0)
		{
			if (_Tokens.GetSize() != 2)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _Meshes.GetSize(); _Index++)
			{
				if (strcmp(_Tokens[1], _Meshes[_Index].Name) == 0)
				{
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
			}

			MeshFileData _MeshFileData;

			size_t _NameLen = strlen(_Tokens[1]);

			_MeshFileData.Name = new char[_NameLen + 1];

			if (!_MeshFileData.Name)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _NameLen + 1; _Index++)
			{
				_MeshFileData.Name[_Index] = _Tokens[1][_Index];
			}

			_MeshFileData.FacesStart = _Faces.GetSize();

			if (_Meshes.GetSize())
			{
				_Meshes[_Meshes.GetSize() - 1].FacesEnd = _Faces.GetSize();
			}

			_Meshes.PushBack(_MeshFileData);

			continue;
		}

		if (strcmp(_Tokens[0], "v") == 0)
		{
			if (_Tokens.GetSize() != 4)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Positions.PushBack(BSR::Math::Vec3f((float)(atof(_Tokens[1])), (float)(atof(_Tokens[2])), (float)(atof(_Tokens[3]))));

			continue;
		}

		if (strcmp(_Tokens[0], "vn") == 0)
		{
			if (_Tokens.GetSize() != 4)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Normals.PushBack((BSR::Math::Vec3f((float)(atof(_Tokens[1])), (float)(atof(_Tokens[2])), (float)(atof(_Tokens[3])))));

			continue;
		}

		if (strcmp(_Tokens[0], "vt") == 0)
		{
			if (_Tokens.GetSize() != 3)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_TextureCoords.PushBack((BSR::Math::Vec2f((float)(atof(_Tokens[1])), (float)(atof(_Tokens[2])))));

			continue;
		}

		if (strcmp(_Tokens[0], "f") == 0)
		{
			if (_Tokens.GetSize() < 4)
			{
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Faces.EmplaceBack(BSR::Vector<FaceVertex>());

			BSR::Vector<FaceVertex>& _CurrentFace = _Faces[_Faces.GetSize() - 1];

			for (size_t _Index = 1; _Index < _Tokens.GetSize(); _Index++)
			{
				size_t _TokenLen = strlen(_Tokens[_Index]);

				if (_Tokens[_Index][0] == '/' || _Tokens[_Index][_TokenLen - 1] == '/')
				{
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}

				FaceVertex _FaceVertex;

				size_t _CountSlash = 0;

				for (size_t _IndexCount = 0; _IndexCount < strlen(_Tokens[_Index]); _IndexCount++)
				{
					if (_Tokens[_Index][_IndexCount] == '/')
					{
						_CountSlash++;
					}
				}

				switch (_CountSlash)
				{
				case 0:
				{
					_FaceVertex.Position = (size_t)(strtoul(_Tokens[_Index], nullptr, 10) - 1);

					if (_FaceVertex.Position >= _Positions.GetSize())
					{
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 1:
				{
					BSR::Vector<char> _FaceVertexText;

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText.PushBack(_Tokens[_Index][_IndexCopy]);
					}

					BSR::Vector<const char*> _FaceVertexTokens;

					for (size_t _IndexToken = 0; _IndexToken < _FaceVertexText.GetSize() - 1; _IndexToken++)
					{
						if (_FaceVertexText[_IndexToken] == '/')
						{
							_FaceVertexText[_IndexToken] = '\0';
						}
					}

					if (_FaceVertexText[0] != '\0')
					{
						_FaceVertexTokens.PushBack(&_FaceVertexText[0]);
					}

					for (size_t _IndexToken = 1; _IndexToken < _FaceVertexText.GetSize() - 1; _IndexToken++)
					{
						if (_FaceVertexText[_IndexToken] != '\0' && _FaceVertexText[_IndexToken - 1] == '\0')
						{
							_FaceVertexTokens.PushBack(&_FaceVertexText[_IndexToken]);
						}
					}

					_FaceVertex.Position = (size_t)(strtoul(_FaceVertexTokens[0], nullptr, 10) - 1);

					if (_FaceVertex.Position >= _Positions.GetSize())
					{
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					_FaceVertex.TextureCoords = (size_t)(strtoul(_FaceVertexTokens[1], nullptr, 10) - 1);

					if (_FaceVertex.TextureCoords >= _TextureCoords.GetSize())
					{
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 2:
				{
					BSR::Vector<char> _FaceVertexText;

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText.PushBack(_Tokens[_Index][_IndexCopy]);
					}

					BSR::Vector<const char*> _FaceVertexTokens;

					for (size_t _IndexToken = 0; _IndexToken < _FaceVertexText.GetSize() - 1; _IndexToken++)
					{
						if (_FaceVertexText[_IndexToken] == '/')
						{
							_FaceVertexText[_IndexToken] = '\0';
						}
					}

					if (_FaceVertexText[0] != '\0')
					{
						_FaceVertexTokens.PushBack(&_FaceVertexText[0]);
					}

					for (size_t _IndexToken = 1; _IndexToken < _FaceVertexText.GetSize() - 1; _IndexToken++)
					{
						if (_FaceVertexText[_IndexToken] != '\0' && _FaceVertexText[_IndexToken - 1] == '\0')
						{
							_FaceVertexTokens.PushBack(&_FaceVertexText[_IndexToken]);
						}
					}

					if (_FaceVertexTokens.GetSize() == 2)
					{
						_FaceVertex.Position = (size_t)(strtoul(_FaceVertexTokens[0], nullptr, 10) - 1);

						if (_FaceVertex.Position >= _Positions.GetSize())
						{
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						_FaceVertex.Normal = (size_t)(strtoul(_FaceVertexTokens[1], nullptr, 10) - 1);

						if (_FaceVertex.Normal >= _Normals.GetSize())
						{
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}
					else
					{
						_FaceVertex.Position = (size_t)(strtoul(_FaceVertexTokens[0], nullptr, 10) - 1);

						if (_FaceVertex.Position >= _Positions.GetSize())
						{
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						_FaceVertex.TextureCoords = (size_t)(strtoul(_FaceVertexTokens[1], nullptr, 10) - 1);

						if (_FaceVertex.TextureCoords >= _TextureCoords.GetSize())
						{
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						_FaceVertex.Normal = (size_t)(strtoul(_FaceVertexTokens[2], nullptr, 10) - 1);

						if (_FaceVertex.Normal >= _Normals.GetSize())
						{
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}

					break;
				}
				default:
				{
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.GetSize(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
				}

				_CurrentFace.PushBack(_FaceVertex);
			}

			continue;
		}
	}

	if (!_Meshes.GetSize())
	{
		const char* _DefaultName = "default_name";

		MeshFileData _DefaultMesh;

		_DefaultMesh.Name = new char[strlen(_DefaultName) + 1];

		if (!_DefaultMesh.Name)
		{
			return false;
		}

		for (size_t _Index = 0; _Index < strlen(_DefaultName); _Index++)
		{
			_DefaultMesh.Name[_Index] = _DefaultName[_Index];
		}
		_DefaultMesh.FacesStart = 0;

		_Meshes.PushBack(_DefaultMesh);
	}

	_Meshes[_Meshes.GetSize() - 1].FacesEnd = _Faces.GetSize();

	for (size_t _IndexMesh = 0; _IndexMesh < _Meshes.GetSize(); _IndexMesh++)
	{
		MeshFileData& _CurrentMesh = _Meshes[_IndexMesh];

		Mesh _MeshTemp;

		_MeshTemp.Name = _CurrentMesh.Name;

		for (size_t _IndexFace = _CurrentMesh.FacesStart; _IndexFace < _CurrentMesh.FacesEnd; _IndexFace++)
		{
			BSR::Vector<FaceVertex>& _CurrentFace = _Faces[_IndexFace];

			for (size_t _IndexFaceVertex = 1; _IndexFaceVertex < _CurrentFace.GetSize() - 1; _IndexFaceVertex++)
			{
				VertexData _VertA;
				VertexData _VertB;
				VertexData _VertC;

				_VertA.Position = _Positions[_CurrentFace[0].Position];

				_VertB.Position = _Positions[_CurrentFace[_IndexFaceVertex].Position];

				_VertC.Position = _Positions[_CurrentFace[_IndexFaceVertex + 1].Position];

				if (_CurrentFace[0].Normal != (size_t)(-1))
				{
					_VertA.Normal = _Normals[_CurrentFace[0].Normal];
				}
				else
				{
					_VertA.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[0].TextureCoords != (size_t)(-1))
				{
					_VertA.TextureCoords = _TextureCoords[_CurrentFace[0].TextureCoords];
				}
				else
				{
					_VertA.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex].Normal != (size_t)(-1))
				{
					_VertB.Normal = _Normals[_CurrentFace[_IndexFaceVertex].Normal];
				}
				else
				{
					_VertB.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1))
				{
					_VertB.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex].TextureCoords];
				}
				else
				{
					_VertB.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex + 1].Normal != (size_t)(-1))
				{
					_VertC.Normal = _Normals[_CurrentFace[_IndexFaceVertex + 1].Normal];
				}
				else
				{
					_VertC.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					_VertC.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex + 1].TextureCoords];
				}
				else
				{
					_VertC.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[0].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					if (_VertA.TextureCoords != _VertB.TextureCoords && _VertB.TextureCoords != _VertC.TextureCoords && _VertC.TextureCoords != _VertA.TextureCoords)
					{
						BSR::Math::Vec3f _Edge1 = _VertB.Position - _VertA.Position;
						BSR::Math::Vec3f _Edge2 = _VertC.Position - _VertA.Position;

						BSR::Math::Vec2f _Delta1 = _VertB.TextureCoords - _VertA.TextureCoords;
						BSR::Math::Vec2f _Delta2 = _VertC.TextureCoords - _VertA.TextureCoords;

						float _Factor = 1.0f / (_Delta1.x * _Delta2.y - _Delta2.x * _Delta1.y);

						_VertA.Tangent.x = _Factor * (_Delta2.y * _Edge1.x - _Delta1.y * _Edge2.x);
						_VertA.Tangent.y = _Factor * (_Delta2.y * _Edge1.y - _Delta1.y * _Edge2.y);
						_VertA.Tangent.z = _Factor * (_Delta2.y * _Edge1.z - _Delta1.y * _Edge2.z);

						_VertA.Tangent /= _VertA.Tangent.Magnitude();

						_VertB.Tangent.x = _VertA.Tangent.x;
						_VertB.Tangent.y = _VertA.Tangent.y;
						_VertB.Tangent.z = _VertA.Tangent.z;

						_VertC.Tangent.x = _VertA.Tangent.x;
						_VertC.Tangent.y = _VertA.Tangent.y;
						_VertC.Tangent.z = _VertA.Tangent.z;
					}
					else
					{
						_VertA.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertB.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertC.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					_VertA.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertB.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertC.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
				}

				IndexData _IndexData;

				_IndexData.IndexA = _MeshTemp.VBO.GetSize();
				_IndexData.IndexB = _MeshTemp.VBO.GetSize() + 1;
				_IndexData.IndexC = _MeshTemp.VBO.GetSize() + 2;

				_MeshTemp.VBO.PushBack(_VertA);
				_MeshTemp.VBO.PushBack(_VertB);
				_MeshTemp.VBO.PushBack(_VertC);

				_MeshTemp.IBO.PushBack(_IndexData);
			}
		}

		Mesh _MeshTrue;

		_MeshTrue.Name = _CurrentMesh.Name;

		for (size_t _IndexTemp = 0; _IndexTemp < _MeshTemp.VBO.GetSize(); _IndexTemp++)
		{
			bool _Found = false;

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_IndexTemp].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_IndexTemp].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_IndexTemp].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_IndexTemp].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_Found = true;
					break;
				}
			}

			if (_Found)
			{
				continue;
			}

			_MeshTrue.VBO.PushBack(_MeshTemp.VBO[_IndexTemp]);
		}

		for (size_t _IndexTemp = 0; _IndexTemp < _MeshTemp.IBO.GetSize(); _IndexTemp++)
		{
			IndexData _IndexDataTrue;

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexA = _IndexTrue;
					break;
				}
			}

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexB = _IndexTrue;
					break;
				}
			}

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexC = _IndexTrue;
					break;
				}
			}

			_MeshTrue.IBO.PushBack(_IndexDataTrue);
		}

		_Model.EmplaceBack((BSR::Renderer::Mesh&&)(_MeshTrue));
	}

	return true;
}



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
	TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
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

	for (size_t _YPos = 0; _YPos < TargetFrameBuffer.Height; _YPos++)
	{
		for (size_t _XPos = 0; _XPos < TargetFrameBuffer.Width; _XPos++)
		{
			TargetFrameBuffer.Albedo[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Metalness[_XPos + _YPos * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.Roughness[_XPos + _YPos * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.AmbientOcclusion[_XPos + _YPos * TargetFrameBuffer.Width] = 0.0f;
			TargetFrameBuffer.NormalMap[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Emission[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Position[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
			TargetFrameBuffer.Depth[_XPos + _YPos * TargetFrameBuffer.Width] = 1.0f;
			TargetFrameBuffer.Stencil[_XPos + _YPos * TargetFrameBuffer.Width] = 0;
			TargetFrameBuffer.Result[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
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
	TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
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



BSR::Renderer::BlinnPhongContext::BlinnPhongContext() : TargetFrameBuffer(), TargetCamera(), TargetExposure(1.0f), TargetFogType(_NoFog), TargetFogStart(0.0f), TargetFogEnd(0.0f), TargetFogColor(Math::Vec3f(0.8f, 0.8f, 0.8f)), TargetEnvironment(nullptr), TargetAmbientLight(Math::Vec3f(0.1f, 0.1f, 0.1f)), TargetMeshes(), TargetMaterials(), TargetTransforms(), TargetLights()
{

}

BSR::Renderer::BlinnPhongContext::BlinnPhongContext(BlinnPhongContext&& _Other) noexcept : TargetFrameBuffer(_Other.TargetFrameBuffer), TargetCamera(_Other.TargetCamera), TargetExposure(_Other.TargetExposure), TargetFogType(_Other.TargetFogType), TargetFogStart(_Other.TargetFogStart), TargetFogEnd(_Other.TargetFogEnd), TargetFogColor((Math::Vec3f&&)(_Other.TargetFogColor)), TargetEnvironment(_Other.TargetEnvironment), TargetAmbientLight(_Other.TargetAmbientLight), TargetMeshes((Vector<const Mesh*>&&)(_Other.TargetMeshes)), TargetMaterials((Vector<BlinnPhongMaterial>&&)(_Other.TargetMaterials)), TargetTransforms((Vector<Transform>&&)(_Other.TargetTransforms)), TargetLights((Vector<Light>&&)(_Other.TargetLights))
{
	_Other.TargetFrameBuffer = BlinnPhongFrameBuffer();
	_Other.TargetCamera = Camera();
	_Other.TargetExposure = 1.0f;
	_Other.TargetFogType = _NoFog;
	_Other.TargetFogStart = 0.0f;
	_Other.TargetFogEnd = 0.0f;
	_Other.TargetEnvironment = nullptr;
	_Other.TargetAmbientLight = Math::Vec3f(0.1f, 0.1f, 0.1f);
}

BSR::Renderer::BlinnPhongContext::~BlinnPhongContext()
{

}

void BSR::Renderer::BlinnPhongContext::StartScene(BlinnPhongFrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const uint8_t _TargetFogType, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const Rasterizer::TextureHDR* _TargetEnvironment, const Math::Vec3f& _TargetAmbientLight)
{
	TargetFrameBuffer = BlinnPhongFrameBuffer();
	TargetCamera = Camera();
	TargetExposure = 1.0f;
	TargetFogType = _NoFog;
	TargetFogStart = 0.0f;
	TargetFogEnd = 0.0f;
	TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
	TargetEnvironment = nullptr;
	TargetAmbientLight = Math::Vec3f(0.1f, 0.1f, 0.1f);
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
	TargetAmbientLight = _TargetAmbientLight;
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

	for (size_t _YPos = 0; _YPos < TargetFrameBuffer.Height; _YPos++)
	{
		for (size_t _XPos = 0; _XPos < TargetFrameBuffer.Width; _XPos++)
		{
			TargetFrameBuffer.Depth[_XPos + _YPos * TargetFrameBuffer.Width] = 1.0f;
			TargetFrameBuffer.Color[_XPos + _YPos * TargetFrameBuffer.Width] = Math::Vec3f(0.0f, 0.0f, 0.0f);
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
		_Uniforms.AmbientLight = TargetAmbientLight;

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
	TargetFogColor = Math::Vec3f(0.8f, 0.8f, 0.8f);
	TargetEnvironment = nullptr;
	TargetAmbientLight = Math::Vec3f(0.1f, 0.1f, 0.1f);
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
	TargetAmbientLight = _Other.TargetAmbientLight;
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
	_Other.TargetAmbientLight = Math::Vec3f(0.1f, 0.1f, 0.1f);

	return *this;
}
