#pragma once

namespace pxl
{
	struct Vec3;
	struct Mat4x4
	{
	public:
		float m11;
		float m12;
		float m13;
		float m14;

		float m21;
		float m22;
		float m23;
		float m24;

		float m31;
		float m32;
		float m33;
		float m34;

		float m41;
		float m42;
		float m43;
		float m44;

		Mat4x4();
		Mat4x4(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

		static const Mat4x4 identity;
		static Mat4x4 createOrtho(float width, float height, float z_near_plane, float z_far_plane);
		static Mat4x4 createOrthoOffcenter(float left, float right, float bottom, float top, float z_near_plane, float z_far_plane);
		static Mat4x4 createPerspective(float field_of_view, float ratio, float z_near_plane, float z_far_plane);
		static Mat4x4 createTranslation(float x, float y, float z);
		static Mat4x4 createRotationZ(float radians);
		static Mat4x4 createScale(float x, float y, float z);
		static Mat4x4 createLookAt(const Vec3& position, const Vec3& target, const Vec3& up);
		Mat4x4 operator* (const Mat4x4& rhs);
	};
}
