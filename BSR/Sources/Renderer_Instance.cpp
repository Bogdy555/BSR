#include "..\Headers\BSR.hpp"



BSR::Renderer::Instance::Instance() : TargetFrameBuffer(), TargetCamera(), TargetMeshes(), TargetMaterials(), TargetTransforms(), TargetLights()
{

}

BSR::Renderer::Instance::~Instance()
{
	BSR_ASSERT(TargetFrameBuffer == FrameBuffer());
}

bool BSR::Renderer::Instance::StartScene(FrameBuffer& _TargetFrameBuffer, const Camera& _TargetCamera)
{
	TargetMeshes.clear();
	TargetMaterials.clear();
	TargetTransforms.clear();
	TargetLights.clear();

	if (!_TargetFrameBuffer.Valid())
	{
		TargetCamera = Camera();
		TargetFrameBuffer = FrameBuffer();
		return false;
	}

	TargetCamera = _TargetCamera;
	TargetFrameBuffer = _TargetFrameBuffer;

	return true;
}

bool BSR::Renderer::Instance::FlushScene()
{


	TargetCamera = Camera();
	TargetFrameBuffer = FrameBuffer();
	TargetMeshes.clear();
	TargetMaterials.clear();
	TargetTransforms.clear();
	TargetLights.clear();

	return true;
}

void BSR::Renderer::Instance::SubmitModel(const Mesh& _TargetMesh, const Material& _TargetMaterial, const Transform& _TargetTransform)
{
	TargetMeshes.push_back(&_TargetMesh);
	TargetMaterials.push_back(_TargetMaterial);
	TargetTransforms.push_back(_TargetTransform);
}

void BSR::Renderer::Instance::SubmitLight(const Light& _TargetLight)
{
	TargetLights.push_back(_TargetLight);
}
