#ifndef BSR_Renderer_hpp

#define BSR_Renderer_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace Renderer
	{

		struct Material
		{
			const Rasterizer::Texture_RGB* Albedo = nullptr;
			const Rasterizer::Texture_R* Metalness = nullptr;
			const Rasterizer::Texture_R* Roughness = nullptr;
			const Rasterizer::Texture_R* AmbientOcclusion = nullptr;
			const Rasterizer::Texture_RGB* NormalMap = nullptr;
			const Rasterizer::Texture_RGB* Emission = nullptr;

			Math::Vec3f AlbedoMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			float MetalnessMultiplier = 1.0f;
			float RoughnessMultiplier = 1.0f;
			float AmbientOcclusionMultiplier = 1.0f;
			Math::Vec3f NormalMapMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);
			Math::Vec3f EmissionMultiplier = Math::Vec3f(1.0f, 1.0f, 1.0f);

			const Rasterizer::Texture_RGB* AlbedoBack = nullptr;
			const Rasterizer::Texture_R* MetalnessBack = nullptr;
			const Rasterizer::Texture_R* RoughnessBack = nullptr;
			const Rasterizer::Texture_R* AmbientOcclusionBack = nullptr;
			const Rasterizer::Texture_RGB* NormalMapBack = nullptr;
			const Rasterizer::Texture_RGB* EmissionBack = nullptr;

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

		enum LightTypes : uint8_t
		{
			_DirectionalLight = 0,
			_PointLight = 1,
			_SpotLight = 2
		};

		struct Light
		{
			uint8_t Type = _PointLight;
			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
			Math::Vec3f Direction = Math::Vec3f(0.0f, -1.0f, 0.0f);
			float Theta = 0.0f;
			float ThetaFade = 0.0f;
			Math::Vec3f Color = Math::Vec3f(1.0f, 1.0f, 1.0f);
			float Intensity = 1.0f;
		};

		struct Camera
		{

			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);

			float AngleFlat = 0.0f;
			float AngleVertical = 0.0f;
			float AngleTilt = 0.0f;

			bool Perspective = true;

			float FieldOfView = 90.0f * Math::fDegreesToRadians;
			float NearPlane = 0.3f;
			float FarPlane = 10.0f;

			const Math::Mat4f GetViewMatrix() const;
			const Math::Mat4f GetProjectionMatrix(const float _AspectRatio) const;
			const Math::Mat4f GetCubeMapMatrix(const float _AspectRatio) const;
			const Math::Vec3f GetForwardVector();

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

		struct VertexData
		{
			Math::Vec3f Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
			Math::Vec3f Normal = Math::Vec3f(0.0f, 0.0f, 1.0f);
			Math::Vec3f Tangent = Math::Vec3f(1.0f, 0.0f, 0.0f);
			Math::Vec2f TextureCoords = Math::Vec2f(0.0f, 0.0f);
		};

		class VertexBuffer
		{

		public:

			VertexBuffer();
			VertexBuffer(const VertexBuffer& _Other);
			VertexBuffer(VertexBuffer&& _Other) noexcept;
			~VertexBuffer();

			void PushBack(const VertexData& _Vertex);
			void Erase(const size_t _Index);
			void Clear();

			const size_t GetSize() const;

			float* GetData();
			const float* GetData() const;

			VertexData& operator[] (const size_t _Index);
			const VertexData& operator[] (const size_t _Index) const;

			void operator= (const VertexBuffer& _Other);
			void operator= (VertexBuffer&& _Other) noexcept;

		private:

			std::vector<VertexData> Verteces;

		};

		struct IndexData
		{
			size_t IndexA = 0;
			size_t IndexB = 0;
			size_t IndexC = 0;
		};

		class IndexBuffer
		{

		public:

			IndexBuffer();
			IndexBuffer(const IndexBuffer& _Other);
			IndexBuffer(IndexBuffer&& _Other) noexcept;
			~IndexBuffer();

			void PushBack(const IndexData& _Index);
			void Erase(const size_t _Index);
			void Clear();

			const size_t GetSize() const;

			size_t* GetData();
			const size_t* GetData() const;

			IndexData& operator[] (const size_t _Index);
			const IndexData& operator[] (const size_t _Index) const;

			void operator= (const IndexBuffer& _Other);
			void operator= (IndexBuffer&& _Other) noexcept;

		private:

			std::vector<IndexData> Indexes;

		};

		struct Mesh
		{

			wchar_t* Name = nullptr;
			VertexBuffer VBO;
			IndexBuffer IBO;

			static void GenerateCube(Mesh& _Mesh);
			static void GenerateQuad(Mesh& _Mesh);

		};

		class Model
		{

		public:

			Model();
			Model(const Model& _Other) = delete;
			Model(Model&& _Other) noexcept;
			~Model();

			bool Load(const wchar_t* _Path);
			const bool Save(const wchar_t* _Path) const;

			void PushBack(const Mesh& _Mesh);
			void EmplaceBack(Mesh&& _Mesh) noexcept;
			void Erase(const size_t _Index);
			void Clear();

			const size_t GetSize() const;

			Mesh& operator[] (const size_t _Index);
			const Mesh& operator[] (const size_t _Index) const;

			void operator= (const Model& _Other) = delete;
			void operator= (Model&& _Other) noexcept;

		private:

			std::vector<Mesh> Meshes;

		};

		struct FrameBuffer
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

			const bool operator== (const FrameBuffer& _Other) const;
		};

		class Instance
		{

		public:

			Instance();
			Instance(const Instance& _Other) = delete;
			Instance(Instance&& _Other) noexcept = delete;
			~Instance();

			bool StartScene(FrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera, const float _TargetExposure, const float _TargetFogStart, const float _TargetFogEnd, const Math::Vec3f& _TargetFogColor, const bool _TargetFogDepthBased, const Rasterizer::Texture_Float_RGB& _TargetEnvironment, const Rasterizer::Texture_Float_RGB& _TargetIradiance, const Rasterizer::Texture_RG& _TargetBRDFLookUp);
			bool FlushScene();

			void SubmitModel(const Mesh& _TargetMesh, const Material& _TargetMaterial, const Transform& _TargetTransform);
			void SubmitLight(const Light& _TargetLight);

			void operator= (const Instance& _Other) = delete;
			void operator= (Instance&& _Other) noexcept = delete;

		private:

			FrameBuffer TargetFrameBuffer;
			Camera TargetCamera;
			float TargetExposure;
			float TargetFogStart;
			float TargetFogEnd;
			Math::Vec3f TargetFogColor;
			bool TargetFogDepthBased;
			const Rasterizer::Texture_Float_RGB* TargetEnvironment;
			const Rasterizer::Texture_Float_RGB* TargetIradiance;
			const Rasterizer::Texture_RG* TargetBRDFLookUp;
			std::vector<const Mesh*> TargetMeshes;
			std::vector<Material> TargetMaterials;
			std::vector<Transform> TargetTransforms;
			std::vector<Light> TargetLights;

		};

	}

}



#endif
