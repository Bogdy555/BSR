#ifndef BSR_Renderer_hpp

#define BSR_Renderer_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace Renderer
	{

		struct VertexData
		{

			Math::Vec3f Position;
			Math::Vec3f Normal;
			Math::Vec3f Tangent;
			Math::Vec2f TextureCoords;

			VertexData(const Math::Vec3f& _Position = Math::Vec3f(0.0f, 0.0f, 0.0f), const Math::Vec3f& _Normal = Math::Vec3f(0.0f, 0.0f, 1.0f), const Math::Vec3f& _Tangent = Math::Vec3f(1.0f, 0.0f, 0.0f), const Math::Vec2f& _TextureCoords = Math::Vec2f(0.0f, 0.0f));

		};

		typedef Vector<VertexData> VertexBuffer;

		struct IndexData
		{

			size_t IndexA;
			size_t IndexB;
			size_t IndexC;

			IndexData(const size_t _IndexA = 0, const size_t _IndexB = 0, const size_t _IndexC = 0);

		};

		typedef Vector<IndexData> IndexBuffer;

		struct Mesh
		{
			char* Name = nullptr;
			VertexBuffer VBO;
			IndexBuffer IBO;

			static void GenerateCube(Mesh& _Mesh);
			static void GenerateQuad(Mesh& _Mesh);
		};

		typedef Vector<Mesh> Model;

		const bool LoadModel(const char* _FileData, const size_t _FileSize, Model& _Model);
		char* SaveModel(size_t& _FileSize, const Model& _Model);

		struct Camera
		{
			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);

			float AngleFlat = 0.0f;
			float AngleVertical = 0.0f;
			float AngleTilt = 0.0f;

			bool Perspective = true;

			float FieldOfView = 90.0f * Math::DegreesToRadians;
			float NearPlane = 0.3f;
			float FarPlane = 10.0f;

			const Math::Mat4f GetViewMatrix() const;
			const Math::Mat4f GetProjectionMatrix(const float _AspectRatio) const;
			const Math::Mat4f GetCubeMapMatrix(const float _AspectRatio) const;
			const Math::Vec3f GetForwardVector() const;
		};

		struct Transform
		{
			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);

			float AngleFlat = 0.0f;
			float AngleVertical = 0.0f;
			float AngleTilt = 0.0f;

			Math::Vec3f Scale = Math::Vec3f(1.0f, 1.0f, 1.0f);

			float ShearXByY = 0.0f;
			float ShearXByZ = 0.0f;

			float ShearYByZ = 0.0f;
			float ShearYByX = 0.0f;

			float ShearZByX = 0.0f;
			float ShearZByY = 0.0f;

			const Math::Mat4f GetModelMatrix() const;
		};

		enum LightTypes : const uint8_t
		{
			_DirectionalLight = 0,
			_PointLight = 1,
			_SpotLight = 2
		};

		struct Light
		{
			uint8_t Type = _PointLight;
			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
			Math::Vec3f Direction = Math::Vec3f(0.0f, 0.0f, -1.0f);
			float Theta = 0.0f;
			float ThetaFade = 0.0f;
			Math::Vec3f Color = Math::Vec3f(1.0f, 1.0f, 1.0f);
			float Intensity = 1.0f;
		};

		enum FogTypes : const uint8_t
		{
			_NoFog = 0,
			_DistanceBasedFog = 1,
			_DepthBasedFog = 2
		};

		struct PBRMaterial
		{
			const Rasterizer::TextureSDR* Albedo = nullptr;
			const Rasterizer::TextureSDR* Metalness = nullptr;
			const Rasterizer::TextureSDR* Roughness = nullptr;
			const Rasterizer::TextureSDR* AmbientOcclusion = nullptr;
			const Rasterizer::TextureSDR* NormalMap = nullptr;
			const Rasterizer::TextureSDR* Emission = nullptr;

			Math::Vec3f AlbedoMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			float MetalnessMultiplier = 1.0f;
			float RoughnessMultiplier = 1.0f;
			float AmbientOcclusionMultiplier = 1.0f;
			Math::Vec3f NormalMapMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			Math::Vec3f EmissionMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);

			const Rasterizer::TextureSDR* AlbedoBack = nullptr;
			const Rasterizer::TextureSDR* MetalnessBack = nullptr;
			const Rasterizer::TextureSDR* RoughnessBack = nullptr;
			const Rasterizer::TextureSDR* AmbientOcclusionBack = nullptr;
			const Rasterizer::TextureSDR* NormalMapBack = nullptr;
			const Rasterizer::TextureSDR* EmissionBack = nullptr;

			Math::Vec3f AlbedoBackMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			float MetalnessBackMultiplier = 1.0f;
			float RoughnessBackMultiplier = 1.0f;
			float AmbientOcclusionBackMultiplier = 1.0f;
			Math::Vec3f NormalMapBackMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			Math::Vec3f EmissionBackMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);

			const bool HasFrontFace() const;
			const bool HasBackFace() const;

			const uint8_t GetCullingType() const;
		};

		struct PBRFrameBuffer
		{
			size_t Width = 0;
			size_t Height = 0;

			Math::Vec3f* Albedo = nullptr;
			float* Metalness = nullptr;
			float* Roughness = nullptr;
			float* AmbientOcclusion = nullptr;
			Math::Vec3f* NormalMap = nullptr;
			Math::Vec3f* Emission = nullptr;
			Math::Vec3f* Position = nullptr;
			float* Depth = nullptr;
			uint64_t* Stencil = nullptr;
			Math::Vec3f* Result = nullptr;

			const bool Valid() const;
		};

		class PBRContext
		{

		public:

			PBRContext();
			PBRContext(const PBRContext& _Other) = delete;
			PBRContext(PBRContext&& _Other) noexcept;
			~PBRContext();

			void StartScene(PBRFrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const uint8_t _TargetFogType, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const Rasterizer::TextureHDR& _TargetEnvironment, const Rasterizer::TextureHDR& _TargetIradiance, const Rasterizer::TextureSDR& _TargetBRDFLookUp);
			void FlushScene();

			void SubmitModel(const Mesh& _TargetMesh, const PBRMaterial& _TargetMaterial, const Transform& _TargetTransform);
			void SubmitLight(const Light& _TargetLight);

			PBRContext& operator= (const PBRContext& _Other) = delete;
			PBRContext& operator= (PBRContext&& _Other) noexcept;

		private:

			PBRFrameBuffer TargetFrameBuffer;
			Camera TargetCamera;
			float TargetExposure;
			uint8_t TargetFogType;
			float TargetFogStart;
			float TargetFogEnd;
			Math::Vec3f TargetFogColor;
			const Rasterizer::TextureHDR* TargetEnvironment;
			const Rasterizer::TextureHDR* TargetIradiance;
			const Rasterizer::TextureSDR* TargetBRDFLookUp;
			Vector<const Mesh*> TargetMeshes;
			Vector<PBRMaterial> TargetMaterials;
			Vector<Transform> TargetTransforms;
			Vector<Light> TargetLights;

		};

		//struct BlinnPhongMaterial
		//{
		//	const Rasterizer::TextureSDR* Color = nullptr;
		//	const Rasterizer::TextureSDR* ShineColor = nullptr;
		//	const Rasterizer::TextureSDR* Roughness = nullptr;
		//	const Rasterizer::TextureSDR* Shine = nullptr;

		//	Math::Vec3f ColorMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
		//	Math::Vec3f ShineColorMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
		//	float RoughnessMultiplier = 1.0f;
		//	float ShineMultiplier = 1.0f;

		//	const Rasterizer::TextureSDR* ColorBack = nullptr;
		//	const Rasterizer::TextureSDR* ShineColorBack = nullptr;
		//	const Rasterizer::TextureSDR* RoughnessBack = nullptr;
		//	const Rasterizer::TextureSDR* ShineBack = nullptr;

		//	Math::Vec3f ColorBackMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
		//	Math::Vec3f ShineColorBackMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
		//	float RoughnessBackMultiplier = 1.0f;
		//	float ShineBackMultiplier = 1.0f;

		//	const bool HasFrontFace() const;
		//	const bool HasBackFace() const;

		//	const uint8_t GetCullingType() const;
		//};

		//struct BlinnPhongFrameBuffer
		//{
		//	size_t Width = 0;
		//	size_t Height = 0;

		//	float* Depth = nullptr;
		//	Math::Vec3f* Color = nullptr;

		//	const bool Valid() const;
		//};

		//class BlinnPhongContext
		//{

		//public:

		//	BlinnPhongContext();
		//	BlinnPhongContext(const BlinnPhongContext& _Other) = delete;
		//	BlinnPhongContext(BlinnPhongContext&& _Other) noexcept;
		//	~BlinnPhongContext();

		//	void StartScene(PBRFrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera);
		//	void FlushScene();

		//	void SubmitModel(const Mesh& _TargetMesh, const Transform& _TargetTransform);

		//	BlinnPhongContext& operator= (const BlinnPhongContext& _Other) = delete;
		//	BlinnPhongContext& operator= (BlinnPhongContext&& _Other) noexcept;

		//private:

		//	PBRFrameBuffer TargetFrameBuffer;
		//	Camera TargetCamera;
		//	Vector<const Mesh*> TargetMeshes;
		//	Vector<Transform> TargetTransforms;

		//};

	}

}



#endif
