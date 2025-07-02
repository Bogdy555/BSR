#include "BSR_APP_ESP32.hpp"



size_t BSR_APP_ESP32::RunTime::Sync = 60;
BSR_APP_ESP32::Time::Timer BSR_APP_ESP32::RunTime::FrameTime[2];
float BSR_APP_ESP32::RunTime::SimulationSpeed = 1.0f;

TFT_eSPI BSR_APP_ESP32::RunTime::Display;
const size_t BSR_APP_ESP32::RunTime::Resize = 2;
BSR::Image::SDR BSR_APP_ESP32::RunTime::ScreenImage;
BSR::Renderer::BlinnPhongFrameBuffer BSR_APP_ESP32::RunTime::FrameBuffer;
BSR::Rasterizer::Context BSR_APP_ESP32::RunTime::Context;

BSR::Renderer::Model BSR_APP_ESP32::RunTime::TeaPot;
BSR::Renderer::Camera BSR_APP_ESP32::RunTime::Camera;
BSR::Renderer::Transform BSR_APP_ESP32::RunTime::Transform;
BSR::Rasterizer::TextureSDR BSR_APP_ESP32::RunTime::WhiteTexture;
BSR_APP_ESP32::RunTime::Materials BSR_APP_ESP32::RunTime::Material;



const BSR::Math::Vec4f BSR_APP_ESP32::RunTime::VertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers)
{
	const BSR::Renderer::VertexData& _TrueVertex = *(const BSR::Renderer::VertexData*)(_Vertex);
	const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
	Lerpers& _TrueLerpers = *(Lerpers*)(_OutLerpers);

	_TrueLerpers.Position = BSR::Math::Vec3f(_TrueUniforms.Model * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f));
	_TrueLerpers.Normal = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Normal;
	_TrueLerpers.Tangent = _TrueUniforms.ModelInversedTransposed * _TrueVertex.Tangent;
	_TrueLerpers.TextureCoords = _TrueVertex.TextureCoords;

	return _TrueUniforms.Mvp * BSR::Math::Vec4f(_TrueVertex.Position, 1.0f);
}

void BSR_APP_ESP32::RunTime::FragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType)
{
	const Lerpers& _TrueLerpers = *(const Lerpers*)(_Lerpers);
	const Uniforms& _TrueUniforms = *(const Uniforms*)(_Uniforms);
	BSR::Renderer::BlinnPhongFrameBuffer& _TrueFrameBuffer = *(BSR::Renderer::BlinnPhongFrameBuffer*)(_FrameBuffer);

	if (!BSR::Rasterizer::Context::DepthTest(_FragCoord.z, _TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width], _DepthTestingType))
	{
		return;
	}

	_TrueFrameBuffer.Depth[_XPos + _YPos * _TrueFrameBuffer.Width] = _FragCoord.z;

	BSR::Math::Vec3f _Color = BSR::Math::Vec3f(_TrueUniforms.Material.Color->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorMultiplier;
	BSR::Math::Vec3f _ColorSpecular = BSR::Math::Vec3f(_TrueUniforms.Material.ColorSpecular->Sample(_TrueLerpers.TextureCoords)) * _TrueUniforms.Material.ColorSpecularMultiplier;
	float _Shininess = _TrueUniforms.Material.Shininess->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.ShininessMultiplier;
	float _AmbientOcclusion = _TrueUniforms.Material.AmbientOcclusion->Sample(_TrueLerpers.TextureCoords).x * _TrueUniforms.Material.AmbientOcclusionMultiplier;

	BSR::Math::Vec3f _Normal = -_TrueLerpers.Normal.Normalized();
	BSR::Math::Vec3f _PositionToLight = -_TrueUniforms.CameraForwardVector;
	BSR::Math::Vec3f _PositionToCamera = (_TrueUniforms.Camera.Position - _TrueLerpers.Position).Normalized();
	BSR::Math::Vec3f _HalfWayVec = (_PositionToCamera + _PositionToLight).Normalized();

	BSR::Math::Vec3f _AmbientLight = (_Color + _ColorSpecular) * _TrueUniforms.AmbientLight * _AmbientOcclusion;
	BSR::Math::Vec3f _DiffuseLight = _Color * BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _PositionToLight), 0.0f);
	BSR::Math::Vec3f _SpecularLight = _ColorSpecular * powf(BSR::Math::Max(BSR::Math::Vec3f::Dot(_Normal, _HalfWayVec), 0.0f), _Shininess);

	BSR::Math::Vec4f _Result = BSR::Math::Vec4f(_AmbientLight + _DiffuseLight + _SpecularLight, 1.0f);

	_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width] = BSR::Rasterizer::Context::Blend(_TrueFrameBuffer.Color[_XPos + _YPos * _TrueFrameBuffer.Width], _Result, _BlendingType);
}

void BSR_APP_ESP32::RunTime::SetUp()
{
	Serial.begin(115200);

	pinMode(BSR_APP_ESP32_PowerPin, OUTPUT);
	digitalWrite(BSR_APP_ESP32_PowerPin, HIGH);

	pinMode(BSR_APP_ESP32_BackLight, OUTPUT);
	digitalWrite(BSR_APP_ESP32_BackLight, HIGH);

	SD_MMC.setPins(12, 11, 13);
	SD_MMC.begin("/BSR", true);

	Display.begin();
	Display.setRotation(1);
	Display.setSwapBytes(true);
	Display.fillScreen(0);

	ScreenImage.Width = 320;
	ScreenImage.Height = 240;
	ScreenImage.Data = new uint8_t[ScreenImage.Width * ScreenImage.Height * 2];

	FrameBuffer.Width = ScreenImage.Width / Resize;
	FrameBuffer.Height = ScreenImage.Height / Resize;
	FrameBuffer.Depth = new float[FrameBuffer.Width * FrameBuffer.Height];
	FrameBuffer.Color = new BSR::Math::Vec3f[FrameBuffer.Width * FrameBuffer.Height];

	{
		fs::File _File = SD_MMC.open("/BSR_APP_ESP32_UthaTeapot.obj");
		BSR::Vector<char> _FileData;
		while (_File.available())
		{
			_FileData.PushBack(_File.read());
		}
		_File.close();
		BSR::Renderer::LoadModel(_FileData.GetData(), _FileData.GetSize(), TeaPot);
	}

	Transform.Position = BSR::Math::Vec3f(0.0f, 0.0f, -2.0f);

	WhiteTexture.ChannelsCount = 3;
	WhiteTexture.LerpType = BSR::Rasterizer::_LerpLinear;
	WhiteTexture.WrapType = BSR::Rasterizer::_WrapRepeat;
	WhiteTexture.TextureChain.PushBack(BSR::Image::SDR());
	for (size_t _Mip = 0; _Mip < WhiteTexture.TextureChain.GetSize(); _Mip++)
	{
		WhiteTexture.TextureChain[_Mip].Width = 1;
		WhiteTexture.TextureChain[_Mip].Height = 1;
		WhiteTexture.TextureChain[_Mip].Data = new uint8_t[WhiteTexture.TextureChain[_Mip].Width * WhiteTexture.TextureChain[_Mip].Height * WhiteTexture.ChannelsCount];

		for (size_t _YPos = 0; _YPos < WhiteTexture.TextureChain[_Mip].Height; _YPos++)
		{
			for (size_t _XPos = 0; _XPos < WhiteTexture.TextureChain[_Mip].Width; _XPos++)
			{
				for (size_t _Channel = 0; _Channel < WhiteTexture.ChannelsCount; _Channel++)
				{
					WhiteTexture.TextureChain[_Mip].Data[(_XPos + _YPos * WhiteTexture.TextureChain[_Mip].Width) * WhiteTexture.ChannelsCount + _Channel] = 255;
				}
			}
		}
	}

	Material.Color = &WhiteTexture;
	Material.ColorSpecular = &WhiteTexture;
	Material.Shininess = &WhiteTexture;
	Material.AmbientOcclusion = &WhiteTexture;
	Material.ColorMultiplier = BSR::Math::Vec3f(0.1f, 0.9f, 0.8f);
	Material.ColorSpecularMultiplier = BSR::Math::Vec3f(0.5f, 0.5f, 0.5f);
	Material.ShininessMultiplier = 40.0f;
	Material.AmbientOcclusionMultiplier = 1.0f;
}

void BSR_APP_ESP32::RunTime::Update()
{
	FrameTime[_Current].Start();

	GetInputs();
	Engine();
	FrameBuild();

	if (Sync)
	{
		FrameTime[_Current].Stop();

		if (FrameTime[_Current] < 1.0f / (float)(Sync))
		{
			delay((size_t)((1.0f / (float)(Sync) - FrameTime[_Current]) * 1000.0f));
		}
	}

	FrameTime[_Current].Stop();

	FrameTime[_Previous] = FrameTime[_Current];

	Serial.println(FrameTime[_Current] * 1000.f);
}

void BSR_APP_ESP32::RunTime::GetInputs()
{

}

void BSR_APP_ESP32::RunTime::Engine()
{
	float _TimeStep = FrameTime[_Previous] * SimulationSpeed;

	Transform.AngleFlat += 0.5f * _TimeStep;
	Transform.AngleVertical += 0.2f * _TimeStep;
}

void BSR_APP_ESP32::RunTime::FrameBuild()
{
	for (size_t _YPos = 0; _YPos < FrameBuffer.Height; _YPos++)
	{
		for (size_t _XPos = 0; _XPos < FrameBuffer.Width; _XPos++)
		{
			FrameBuffer.Depth[_XPos + _YPos * FrameBuffer.Width] = 1.0f;
			FrameBuffer.Color[_XPos + _YPos * FrameBuffer.Width] = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
		}
	}

	Context.ViewPortX = 0;
	Context.ViewPortY = 0;
	Context.ViewPortWidth = FrameBuffer.Width;
	Context.ViewPortHeight = FrameBuffer.Height;
	Context.CullingType = BSR::Rasterizer::_CounterClockWiseCulling;
	Context.DepthTestingType = BSR::Rasterizer::_LowerOrEqualDepthTesting;
	Context.BlendingType = BSR::Rasterizer::_NoBlending;

	for (size_t _Index = 0; _Index < TeaPot.GetSize(); _Index++)
	{
		Uniforms _Uniforms;

		_Uniforms.Camera = Camera;
		_Uniforms.CameraForwardVector = _Uniforms.Camera.GetForwardVector();
		_Uniforms.Transform = Transform;
		_Uniforms.Model = _Uniforms.Transform.GetModelMatrix();
		_Uniforms.ModelInversedTransposed = BSR::Math::Mat3f(_Uniforms.Model).Inversed().Transposed();
		_Uniforms.View = _Uniforms.Camera.GetViewMatrix();
		_Uniforms.Projection = _Uniforms.Camera.GetProjectionMatrix((float)(FrameBuffer.Width) / (float)(FrameBuffer.Height));
		_Uniforms.Mvp = _Uniforms.Projection * _Uniforms.View * _Uniforms.Model;
		_Uniforms.Material = Material;
		_Uniforms.AmbientLight = BSR::Math::Vec3f(0.1f, 0.1f, 0.1f);

		Context.VBO = TeaPot[_Index].VBO.GetData();
		Context.VBOSize = TeaPot[_Index].VBO.GetSize();
		Context.VBOStride = sizeof(BSR::Renderer::VertexData);
		Context.IBO = (size_t*)(TeaPot[_Index].IBO.GetData());
		Context.IBOBegin = 0;
		Context.IBOEnd = TeaPot[_Index].IBO.GetSize() * 3;
		Context.Uniforms = &_Uniforms;
		Context.VertexShader = VertexShader;
		Context.LerpersCountVertToGeom = sizeof(Lerpers) / sizeof(float);
		Context.GeometryShader = nullptr;
		Context.LerpersCountGeomToFrag = sizeof(Lerpers) / sizeof(float);
		Context.FragmentShader = FragmentShader;
		Context.FrameBuffer = &FrameBuffer;
		Context.DrawCall();
	}

	for (size_t _YPos = 0; _YPos < ScreenImage.Height; _YPos++)
	{
		for (size_t _XPos = 0; _XPos < ScreenImage.Width; _XPos++)
		{
			BSR::Math::Vec3f _Color = FrameBuffer.Color[_XPos / Resize + _YPos / Resize * FrameBuffer.Width];
			((uint16_t*)(ScreenImage.Data))[_XPos + (ScreenImage.Height - _YPos - 1) * ScreenImage.Width] = ((uint16_t)(_Color.x * 31.0f) << 11) | ((uint16_t)(_Color.y * 63.0f) << 5) | (uint16_t)(_Color.z * 31.0f);
		}
	}

	Display.pushImage(0, 0, ScreenImage.Width, ScreenImage.Height, (uint16_t*)(ScreenImage.Data));
}
