#include "..\Headers\Main.hpp"



const Math::Mat4f Rasterizer::Camera::GetViewMatrix() const
{
	return
		Math::Mat4f::GetRotation(AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetRotation(AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) *
		Math::Mat4f::GetTranslation(-Position);
}

const Math::Mat4f Rasterizer::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	if (Perspective)
	{
		return Math::Mat4f::GetPerspective(FieldOfView, _AspectRatio, NearPlane, FarPlane);
	}

	return Math::Mat4f::GetOrtho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f, -FarPlane, -NearPlane);
}



const Math::Mat4f Rasterizer::Transform::GetModelMatrix() const
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
