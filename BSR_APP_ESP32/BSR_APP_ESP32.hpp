#ifndef BSR_APP_ESP32_hpp

#define BSR_APP_ESP32_hpp



#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SD_MMC.h>
#include <BSR.hpp>



#define BSR_APP_ESP32_PowerPin 10
#define BSR_APP_ESP32_BackLight 38



namespace BSR_APP_ESP32
{

	enum States : const size_t
	{
		_Previous = 0,
		_Current = 1
	};

	namespace Time
	{

		class Timer
		{

		public:

			Timer();
			Timer(const Timer& _Other);
			Timer(Timer&& _Other) noexcept;
			~Timer();

			void Start();
			void Stop();

			const float GetTime() const;

			operator const float () const;

			Timer& operator= (const Timer& _Other);
			Timer& operator= (Timer&& _Other) noexcept;

		private:

			size_t Begin;
			size_t End;

		};

	}

	namespace RunTime
	{

		struct Materials
		{
			const BSR::Rasterizer::TextureSDR* Color = nullptr;
			const BSR::Rasterizer::TextureSDR* ColorSpecular = nullptr;
			const BSR::Rasterizer::TextureSDR* Shininess = nullptr;
			const BSR::Rasterizer::TextureSDR* AmbientOcclusion = nullptr;

			BSR::Math::Vec3f ColorMultiplier = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
			BSR::Math::Vec3f ColorSpecularMultiplier = BSR::Math::Vec3f(1.0f, 1.0f, 1.0f);
			float ShininessMultiplier = 1.0f;
			float AmbientOcclusionMultiplier = 1.0f;
		};

		struct Uniforms
		{
			BSR::Renderer::Camera Camera;
			BSR::Math::Vec3f CameraForwardVector = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
			BSR::Renderer::Transform Transform;

			BSR::Math::Mat4f Model;
			BSR::Math::Mat3f ModelInversedTransposed;
			BSR::Math::Mat4f View;
			BSR::Math::Mat4f Projection;
			BSR::Math::Mat4f Mvp;

			Materials Material;
			BSR::Math::Vec3f AmbientLight = BSR::Math::Vec3f(0.1f, 0.1f, 0.1f);
		};

		struct Lerpers
		{
			BSR::Math::Vec3f Position = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
			BSR::Math::Vec3f Normal = BSR::Math::Vec3f(0.0f, 0.0f, 1.0f);
			BSR::Math::Vec3f Tangent = BSR::Math::Vec3f(1.0f, 0.0f, 0.0f);
			BSR::Math::Vec2f TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
		};

		const BSR::Math::Vec4f VertexShader(const void* _Vertex, const void* _Uniforms, float* _OutLerpers);
		void FragmentShader(const size_t _XPos, const size_t _YPos, const size_t _ViewPortX, const size_t _ViewPortY, const float* _Lerpers, const void* _Uniforms, void* _FrameBuffer, const BSR::Math::Vec4f& _FragCoord, const bool _FrontFacing, const uint8_t _DepthTestingType, const uint8_t _BlendingType);

		extern size_t Sync;
		extern Time::Timer FrameTime[2];
		extern float SimulationSpeed;

		extern TFT_eSPI Display;
		extern const size_t Resize;
		extern BSR::Image::SDR ScreenImage;
		extern BSR::Renderer::BlinnPhongFrameBuffer FrameBuffer;
		extern BSR::Rasterizer::Context Context;

		extern BSR::Renderer::Model TeaPot;
		extern BSR::Renderer::Camera Camera;
		extern BSR::Renderer::Transform Transform;
		extern BSR::Rasterizer::TextureSDR WhiteTexture;
		extern Materials Material;

		void SetUp();

		void Update();

		void GetInputs();

		void Engine();

		void FrameBuild();

	}

}



#endif
