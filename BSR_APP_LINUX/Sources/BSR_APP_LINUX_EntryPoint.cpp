#include "../Headers/BSR_APP_LINUX.hpp"



char* LoadFile(size_t& _FileSize, const char* _FilePath)
{
	_FileSize = 0;

	std::ifstream _FileIn;

	_FileIn.open(_FilePath, std::ios::binary);

	if (!_FileIn.is_open())
	{
		return nullptr;
	}

	_FileIn.seekg(0, std::ios::end);
	_FileSize = (size_t)(_FileIn.tellg());
	_FileIn.seekg(0, std::ios::beg);

	char* _Result = new char[_FileSize];

	if (!_Result)
	{
		_FileSize = 0;
		return nullptr;
	}

	_FileIn.read(_Result, _FileSize);

	_FileIn.close();

	return _Result;
}



int main()
{
	char* _FileData = nullptr;
	size_t _FileSize = 0;

	BSR::Rasterizer::TextureSDR _Albedo;
	_Albedo.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Materials/Iron/Albedo.bmp");
	_Albedo.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _Albedo.TextureChain[0].Width, _Albedo.TextureChain[0].Height);
	delete[] _FileData;
	_Albedo.ChannelsCount = 4;
	BSR::Rasterizer::TextureSDR _Metalness;
	_Metalness.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Materials/Iron/Metalness.bmp");
	_Metalness.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _Metalness.TextureChain[0].Width, _Metalness.TextureChain[0].Height);
	delete[] _FileData;
	_Metalness.ChannelsCount = 4;
	BSR::Rasterizer::TextureSDR _Roughness;
	_Roughness.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Materials/Iron/Roughness.bmp");
	_Roughness.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _Roughness.TextureChain[0].Width, _Roughness.TextureChain[0].Height);
	delete[] _FileData;
	_Roughness.ChannelsCount = 4;
	BSR::Rasterizer::TextureSDR _AmbientOcclusion;
	_AmbientOcclusion.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Intern/Textures/White.bmp");
	_AmbientOcclusion.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _AmbientOcclusion.TextureChain[0].Width, _AmbientOcclusion.TextureChain[0].Height);
	delete[] _FileData;
	_AmbientOcclusion.ChannelsCount = 4;
	BSR::Rasterizer::TextureSDR _NormalMap;
	_NormalMap.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Materials/Iron/Normal.bmp");
	_NormalMap.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _NormalMap.TextureChain[0].Width, _NormalMap.TextureChain[0].Height);
	delete[] _FileData;
	_NormalMap.ChannelsCount = 4;
	BSR::Rasterizer::TextureSDR _Emission;
	_Emission.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Intern/Textures/Black.bmp");
	_Emission.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _Emission.TextureChain[0].Width, _Emission.TextureChain[0].Height);
	delete[] _FileData;
	_Emission.ChannelsCount = 4;
	BSR::Rasterizer::TextureHDR _Environment;
	_Environment.TextureChain.EmplaceBack(BSR::Image::HDR());
	_Environment.TextureChain[0].Width = 1;
	_Environment.TextureChain[0].Height = 1;
	_Environment.TextureChain[0].Data = new float[3];
	_Environment.TextureChain[0].Data[0] = 1.0f;
	_Environment.TextureChain[0].Data[1] = 0.85f;
	_Environment.TextureChain[0].Data[2] = 0.6f;
	_Environment.ChannelsCount = 3;
	BSR::Rasterizer::TextureHDR _Irradiance;
	_Irradiance.TextureChain.EmplaceBack(BSR::Image::HDR());
	_Irradiance.TextureChain[0].Width = 1;
	_Irradiance.TextureChain[0].Height = 1;
	_Irradiance.TextureChain[0].Data = new float[3];
	_Irradiance.TextureChain[0].Data[0] = 1.0f;
	_Irradiance.TextureChain[0].Data[1] = 0.85f;
	_Irradiance.TextureChain[0].Data[2] = 0.6f;
	_Irradiance.ChannelsCount = 3;
	BSR::Rasterizer::TextureSDR _BRDFLookUp;
	_BRDFLookUp.TextureChain.EmplaceBack(BSR::Image::SDR());
	_FileData = LoadFile(_FileSize, "./Intern/Textures/BRDF.bmp");
	_BRDFLookUp.TextureChain[0].Data = BSR::Image::LoadSdr(_FileData, _FileSize, _BRDFLookUp.TextureChain[0].Width, _BRDFLookUp.TextureChain[0].Height);
	delete[] _FileData;
	_BRDFLookUp.ChannelsCount = 4;

	BSR::Renderer::Model _Model;

	_Model.EmplaceBack(BSR::Renderer::Mesh());
	_Model[0].Name = new char[strlen("Cube") + 1];
	strcpy(_Model[0].Name, "Cube");
	BSR::Renderer::Mesh::GenerateCube(_Model[0]);

	BSR::Renderer::Camera _Camera;

	_Camera.Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
	_Camera.AngleFlat = 0.0f;
	_Camera.AngleVertical = 0.0f;
	_Camera.AngleTilt = 0.0f;
	_Camera.Perspective = true;
	_Camera.FieldOfView = 90.0f * BSR::Math::DegreesToRadians;
	_Camera.NearPlane = 0.3f;
	_Camera.FarPlane = 10.0f;

	BSR::Renderer::Transform _Transform;

	_Transform.Position = BSR::Math::Vec3f(0.0f, 0.0f, -5.0f);
	_Transform.AngleFlat = 45.0f * BSR::Math::DegreesToRadians;
	_Transform.AngleVertical = 45.0f * BSR::Math::DegreesToRadians;
	_Transform.AngleTilt = 0.0f;
	_Transform.Scale = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f) * 1.0f;
	_Transform.ShearXByY = 0.0f;
	_Transform.ShearXByZ = 0.0f;
	_Transform.ShearYByZ = 0.0f;
	_Transform.ShearYByX = 0.0f;
	_Transform.ShearZByX = 0.0f;
	_Transform.ShearZByY = 0.0f;

	BSR::Renderer::PBRMaterial _Material;

	_Material.Albedo = &_Albedo;
	_Material.Metalness = &_Metalness;
	_Material.Roughness = &_Roughness;
	_Material.AmbientOcclusion = &_AmbientOcclusion;
	_Material.NormalMap = &_NormalMap;
	_Material.Emission = &_Emission;
	_Material.AlbedoMultiplier = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	_Material.MetalnessMultiplier = 1.0f;
	_Material.RoughnessMultiplier = 1.0f;
	_Material.AmbientOcclusionMultiplier = 1.0f;
	_Material.NormalMapMultiplier = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	_Material.EmissionMultiplier = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
	_Material.AlbedoBack = _Material.Albedo;
	_Material.MetalnessBack = _Material.Metalness;
	_Material.RoughnessBack = _Material.Roughness;
	_Material.AmbientOcclusionBack = _Material.AmbientOcclusion;
	_Material.NormalMapBack = _Material.NormalMap;
	_Material.EmissionBack = _Material.Emission;
	_Material.AlbedoBackMultiplier = _Material.AlbedoMultiplier;
	_Material.MetalnessBackMultiplier = _Material.MetalnessMultiplier;
	_Material.RoughnessBackMultiplier = _Material.RoughnessMultiplier;
	_Material.AmbientOcclusionBackMultiplier = _Material.AmbientOcclusionMultiplier;
	_Material.NormalMapBackMultiplier = _Material.NormalMapMultiplier;
	_Material.EmissionBackMultiplier = _Material.EmissionMultiplier;

	BSR::Renderer::PBRFrameBuffer _FrameBuffer;

	_FrameBuffer.Width = 1920;
	_FrameBuffer.Height = 1080;
	_FrameBuffer.Albedo = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Metalness = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Roughness = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.AmbientOcclusion = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.NormalMap = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Emission = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Position = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Depth = new float[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Stencil = new uint64_t[_FrameBuffer.Width * _FrameBuffer.Height];
	_FrameBuffer.Result = new BSR::Math::Vec3f[_FrameBuffer.Width * _FrameBuffer.Height];

	BSR::Renderer::PBRContext _RendererContext;

	_RendererContext.StartScene(_FrameBuffer, _Camera, 1.0f, BSR::Renderer::_NoFog, 0.0f, 0.0f, BSR::Math::Vec3f(0.0f, 0.0f, 0.0f), _Environment, _Irradiance, _BRDFLookUp);

	_RendererContext.SubmitMesh(_Model[0], _Material, _Transform);

	_RendererContext.FlushScene();

	BSR::Image::SDR _Result;

	_Result.Width = _FrameBuffer.Width;
	_Result.Height = _FrameBuffer.Height;
	_Result.Data = new uint8_t[_Result.Width * _Result.Height * 4];

	for (size_t _YPos = 0; _YPos < _Result.Height; _YPos++)
	{
		for (size_t _XPos = 0; _XPos < _Result.Width; _XPos++)
		{
			_Result.Data[(_XPos + _YPos * _Result.Width) * 4 + 0] = (uint8_t)(_FrameBuffer.Result[_XPos + _YPos * _FrameBuffer.Width].x * 255.0f);
			_Result.Data[(_XPos + _YPos * _Result.Width) * 4 + 1] = (uint8_t)(_FrameBuffer.Result[_XPos + _YPos * _FrameBuffer.Width].y * 255.0f);
			_Result.Data[(_XPos + _YPos * _Result.Width) * 4 + 2] = (uint8_t)(_FrameBuffer.Result[_XPos + _YPos * _FrameBuffer.Width].z * 255.0f);
			_Result.Data[(_XPos + _YPos * _Result.Width) * 4 + 3] = 255;
		}
	}

	delete[] _FrameBuffer.Albedo;
	delete[] _FrameBuffer.Metalness;
	delete[] _FrameBuffer.Roughness;
	delete[] _FrameBuffer.AmbientOcclusion;
	delete[] _FrameBuffer.NormalMap;
	delete[] _FrameBuffer.Emission;
	delete[] _FrameBuffer.Position;
	delete[] _FrameBuffer.Depth;
	delete[] _FrameBuffer.Stencil;
	delete[] _FrameBuffer.Result;

	delete[] _Model[0].Name;

	delete[] _Albedo.TextureChain[0].Data;
	delete[] _Metalness.TextureChain[0].Data;
	delete[] _Roughness.TextureChain[0].Data;
	delete[] _AmbientOcclusion.TextureChain[0].Data;
	delete[] _NormalMap.TextureChain[0].Data;
	delete[] _Emission.TextureChain[0].Data;
	delete[] _Environment.TextureChain[0].Data;
	delete[] _Irradiance.TextureChain[0].Data;
	delete[] _BRDFLookUp.TextureChain[0].Data;

	_FileData = BSR::Image::SaveSdr(_FileSize, _Result);

	delete[] _Result.Data;

	std::ofstream _FileOut;

	_FileOut.open("./Test.bmp", std::ios::binary);

	_FileOut.write(_FileData, _FileSize);

	_FileOut.close();

	delete[] _FileData;

	return 0;
}
