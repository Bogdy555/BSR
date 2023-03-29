#ifndef Math_hpp

#define Math_hpp



#include "Main.hpp"



namespace Math
{

	struct Vec2f
	{

		float x, y;

		Vec2f(const float _x = 0.0f, const float _y = 0.0f);
		Vec2f(const Vec2f& _Other);
		~Vec2f();

		float* Data();
		const float* Data() const;

		const float Magnitude() const;

		const Vec2f& Normalize();
		const Vec2f Normalized() const;

		const Vec2f& operator+ () const;
		const Vec2f operator- () const;

		const Vec2f operator+ (const Vec2f& _Other) const;
		const Vec2f& operator+= (const Vec2f& _Other);
		const Vec2f operator+ (const float _Scale) const;
		const Vec2f& operator+= (const float _Scale);

		const Vec2f operator- (const Vec2f& _Other) const;
		const Vec2f& operator-= (const Vec2f& _Other);
		const Vec2f operator- (const float _Scale) const;
		const Vec2f& operator-= (const float _Scale);

		const Vec2f operator* (const Vec2f& _Other) const;
		const Vec2f& operator*= (const Vec2f& _Other);
		const Vec2f operator* (const float _Scale) const;
		const Vec2f& operator*= (const float _Scale);

		const Vec2f operator/ (const Vec2f& _Other) const;
		const Vec2f& operator/= (const Vec2f& _Other);
		const Vec2f operator/ (const float _Scale) const;
		const Vec2f& operator/= (const float _Scale);

		const bool operator== (const Vec2f& _Other) const;
		const bool operator!= (const Vec2f& _Other) const;

		float& operator[] (const size_t _Index);
		const float& operator[] (const size_t _Index) const;

		const Vec2f& operator= (const Vec2f& _Other);

		static const float GetAngle(const Vec2f& _A, const Vec2f& _B);
		static const float Distance(const Vec2f& _A, const Vec2f& _B);
		static const float Dot(const Vec2f& _A, const Vec2f& _B);
		static const Vec2f Clamp(const Vec2f& _Value, const Vec2f& _Min, const Vec2f& _Max);
		static const Vec2f Mix(const Vec2f& _A, const Vec2f& _B, const float _Percentage);

	};

	struct Vec3f
	{

		float x, y, z;

		Vec3f(const float _x = 0.0f, const float _y = 0.0f, const float _z = 0.0f);
		Vec3f(const Vec3f& _Other);
		explicit Vec3f(const Vec2f& _Other, const float _z = 0.0f);
		~Vec3f();

		const Vec2f ToVec2f() const;

		float* Data();
		const float* Data() const;

		const float Magnitude() const;

		const Vec3f& Normalize();
		const Vec3f Normalized() const;

		const Vec3f& operator+ () const;
		const Vec3f operator- () const;

		const Vec3f operator+ (const Vec3f& _Other) const;
		const Vec3f& operator+= (const Vec3f& _Other);
		const Vec3f operator+ (const float _Scale) const;
		const Vec3f& operator+= (const float _Scale);

		const Vec3f operator- (const Vec3f& _Other) const;
		const Vec3f& operator-= (const Vec3f& _Other);
		const Vec3f operator- (const float _Scale) const;
		const Vec3f& operator-= (const float _Scale);

		const Vec3f operator* (const Vec3f& _Other) const;
		const Vec3f& operator*= (const Vec3f& _Other);
		const Vec3f operator* (const float _Scale) const;
		const Vec3f& operator*= (const float _Scale);

		const Vec3f operator/ (const Vec3f& _Other) const;
		const Vec3f& operator/= (const Vec3f& _Other);
		const Vec3f operator/ (const float _Scale) const;
		const Vec3f& operator/= (const float _Scale);

		const bool operator== (const Vec3f& _Other) const;
		const bool operator!= (const Vec3f& _Other) const;

		float& operator[] (const size_t _Index);
		const float& operator[] (const size_t _Index) const;

		const Vec3f& operator= (const Vec3f& _Other);

		static const float GetAngle(const Vec3f& _A, const Vec3f& _B);
		static const float Distance(const Vec3f& _A, const Vec3f& _B);
		static const float Dot(const Vec3f& _A, const Vec3f& _B);
		static const Vec3f Cross(const Vec3f& _A, const Vec3f& _B);
		static const Vec3f Clamp(const Vec3f& _Value, const Vec3f& _Min, const Vec3f& _Max);
		static const Vec3f Mix(const Vec3f& _A, const Vec3f& _B, const float _Percentage);

	};

	struct Vec4f
	{

		float x, y, z, w;

		Vec4f(const float _x = 0.0f, const float _y = 0.0f, const float _z = 0.0f, const float _w = 0.0f);
		Vec4f(const Vec4f& _Other);
		explicit Vec4f(const Vec2f& _Other, const float _z = 0.0f, const float _w = 0.0f);
		explicit Vec4f(const Vec3f& _Other, const float _w = 0.0f);
		~Vec4f();

		const Vec2f ToVec2f() const;
		const Vec3f ToVec3f() const;

		float* Data();
		const float* Data() const;

		const float Magnitude() const;

		const Vec4f& Normalize();
		const Vec4f Normalized() const;

		const Vec4f& operator+ () const;
		const Vec4f operator- () const;

		const Vec4f operator+ (const Vec4f& _Other) const;
		const Vec4f& operator+= (const Vec4f& _Other);
		const Vec4f operator+ (const float _Scale) const;
		const Vec4f& operator+= (const float _Scale);

		const Vec4f operator- (const Vec4f& _Other) const;
		const Vec4f& operator-= (const Vec4f& _Other);
		const Vec4f operator- (const float _Scale) const;
		const Vec4f& operator-= (const float _Scale);

		const Vec4f operator* (const Vec4f& _Other) const;
		const Vec4f& operator*= (const Vec4f& _Other);
		const Vec4f operator* (const float _Scale) const;
		const Vec4f& operator*= (const float _Scale);

		const Vec4f operator/ (const Vec4f& _Other) const;
		const Vec4f& operator/= (const Vec4f& _Other);
		const Vec4f operator/ (const float _Scale) const;
		const Vec4f& operator/= (const float _Scale);

		const bool operator== (const Vec4f& _Other) const;
		const bool operator!= (const Vec4f& _Other) const;

		float& operator[] (const size_t _Index);
		const float& operator[] (const size_t _Index) const;

		const Vec4f& operator= (const Vec4f& _Other);

		static const float GetAngle(const Vec4f& _A, const Vec4f& _B);
		static const float Distance(const Vec4f& _A, const Vec4f& _B);
		static const float Dot(const Vec4f& _A, const Vec4f& _B);
		static const Vec4f Cross(const Vec4f& _A, const Vec4f& _B);
		static const Vec4f Clamp(const Vec4f& _Value, const Vec4f& _Min, const Vec4f& _Max);
		static const Vec4f Mix(const Vec4f& _A, const Vec4f& _B, const float _Percentage);

	};

	class Mat2f
	{

	public:

		Mat2f();
		Mat2f(const Mat2f& _Other);
		~Mat2f();

		float* Data();
		const float* Data() const;

		const float Determinant() const;
		const float Trace() const;

		const Mat2f& Transpose();
		const Mat2f Transposed() const;

		const Mat2f& Inverse();
		const Mat2f Inversed() const;

		const Mat2f& operator+ () const;
		const Mat2f operator- () const;

		const Vec2f operator* (const Vec2f& _Vec) const;

		const Mat2f operator+ (const Mat2f& _Other) const;
		const Mat2f& operator+= (const Mat2f& _Other);
		const Mat2f operator+ (const float _Scale) const;
		const Mat2f& operator+= (const float _Scale);

		const Mat2f operator- (const Mat2f& _Other) const;
		const Mat2f& operator-= (const Mat2f& _Other);
		const Mat2f operator- (const float _Scale) const;
		const Mat2f& operator-= (const float _Scale);

		const Mat2f operator* (const Mat2f& _Other) const;
		const Mat2f& operator*= (const Mat2f& _Other);
		const Mat2f operator* (const float _Scale) const;
		const Mat2f& operator*= (const float _Scale);

		const Mat2f operator/ (const float _Scale) const;
		const Mat2f& operator/= (const float _Scale);

		const bool operator== (const Mat2f& _Other) const;
		const bool operator!= (const Mat2f& _Other) const;

		float* operator[] (const size_t _Index);
		const float* operator[] (const size_t _Index) const;

		const Mat2f& operator= (const Mat2f& _Other);

		static const Mat2f GetScale(const float _x, const float _y);
		static const Mat2f GetFill(const float _Value);
		static const Mat2f GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis);
		static const Mat2f GetRotation(const float _Angle);

	private:

		Vec2f Matrix[2];

	};

	class Mat3f
	{

	public:

		Mat3f();
		Mat3f(const Mat3f& _Other);
		explicit Mat3f(const Mat2f& _Other);
		~Mat3f();

		const Mat2f ToMat2f() const;

		float* Data();
		const float* Data() const;

		const float Determinant() const;
		const float Trace() const;

		const Mat3f& Transpose();
		const Mat3f Transposed() const;

		const Mat3f& Inverse();
		const Mat3f Inversed() const;

		const Mat3f& operator+ () const;
		const Mat3f operator- () const;

		const Vec3f operator* (const Vec3f& _Vec) const;

		const Mat3f operator+ (const Mat3f& _Other) const;
		const Mat3f& operator+= (const Mat3f& _Other);
		const Mat3f operator+ (const float _Scale) const;
		const Mat3f& operator+= (const float _Scale);

		const Mat3f operator- (const Mat3f& _Other) const;
		const Mat3f& operator-= (const Mat3f& _Other);
		const Mat3f operator- (const float _Scale) const;
		const Mat3f& operator-= (const float _Scale);

		const Mat3f operator* (const Mat3f& _Other) const;
		const Mat3f& operator*= (const Mat3f& _Other);
		const Mat3f operator* (const float _Scale) const;
		const Mat3f& operator*= (const float _Scale);

		const Mat3f operator/ (const float _Scale) const;
		const Mat3f& operator/= (const float _Scale);

		const bool operator== (const Mat3f& _Other) const;
		const bool operator!= (const Mat3f& _Other) const;

		float* operator[] (const size_t _Index);
		const float* operator[] (const size_t _Index) const;

		const Mat3f& operator= (const Mat3f& _Other);

		static const Mat3f GetScale(const float _x, const float _y, const float _z);
		static const Mat3f GetFill(const float _Value);
		static const Mat3f GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis);
		static const Mat3f GetRotation(const float _Angle, const Vec3f& _RotationAxis);
		static const Mat3f GetTranslation(const Vec2f& _Coords);
		static const Mat3f GetOrtho(const float _Left, const float _Right, const float _Bottom, const float _Top);

	private:

		Vec3f Matrix[3];

	};

	class Mat4f
	{

	public:

		Mat4f();
		Mat4f(const Mat4f& _Other);
		explicit Mat4f(const Mat2f& _Other);
		explicit Mat4f(const Mat3f& _Other);
		~Mat4f();

		const Mat2f ToMat2f() const;
		const Mat3f ToMat3f() const;

		float* Data();
		const float* Data() const;

		const float Determinant() const;
		const float Trace() const;

		const Mat4f& Transpose();
		const Mat4f Transposed() const;

		const Mat4f& Inverse();
		const Mat4f Inversed() const;

		const Mat4f& operator+ () const;
		const Mat4f operator- () const;

		const Vec4f operator* (const Vec4f& _Vec) const;

		const Mat4f operator+ (const Mat4f& _Other) const;
		const Mat4f& operator+= (const Mat4f& _Other);
		const Mat4f operator+ (const float _Scale) const;
		const Mat4f& operator+= (const float _Scale);

		const Mat4f operator- (const Mat4f& _Other) const;
		const Mat4f& operator-= (const Mat4f& _Other);
		const Mat4f operator- (const float _Scale) const;
		const Mat4f& operator-= (const float _Scale);

		const Mat4f operator* (const Mat4f& _Other) const;
		const Mat4f& operator*= (const Mat4f& _Other);
		const Mat4f operator* (const float _Scale) const;
		const Mat4f& operator*= (const float _Scale);

		const Mat4f operator/ (const float _Scale) const;
		const Mat4f& operator/= (const float _Scale);

		const bool operator== (const Mat4f& _Other) const;
		const bool operator!= (const Mat4f& _Other) const;

		float* operator[] (const size_t _Index);
		const float* operator[] (const size_t _Index) const;

		const Mat4f& operator= (const Mat4f& _Other);

		static const Mat4f GetScale(const float _x, const float _y, const float _z, const float _w);
		static const Mat4f GetFill(const float _Value);
		static const Mat4f GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis);
		static const Mat4f GetRotation(const float _Angle, const Vec4f& _RotationAxis);
		static const Mat4f GetTranslation(const Vec3f& _Coords);
		static const Mat4f GetOrtho(const float _Left, const float _Right, const float _Bottom, const float _Top, const float _Front, const float _Back);
		static const Mat4f GetPerspective(const float _Fov, const float _AspectRatio, const float _ZNear, const float _ZFar);

	private:

		Vec4f Matrix[4];

	};

	extern const float fPi;
	extern const float fDegreesToRadians;
	extern const float fRadiansToDegrees;

	extern const double dPi;
	extern const double dDegreesToRadians;
	extern const double dRadiansToDegrees;

	void Swap(float& _A, float& _B);
	void Swap(double& _A, double& _B);

	const float Clamp(const float _Value, const float _Min, const float _Max);
	const double Clamp(const double _Value, const double _Min, const double _Max);

	const float Mix(const float _A, const float _B, const float _Percentage);
	const double Mix(const double _A, const double _B, const double _Percentage);

	const float Period(const float _Value, const float _Min, const float _Max);
	const double Period(const double _Value, const double _Min, const double _Max);

}



#endif
