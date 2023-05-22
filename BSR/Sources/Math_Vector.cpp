#include "../Headers/BSR.hpp"



BSR::Math::Vec2f::Vec2f() : x(0.0f), y(0.0f)
{

}

BSR::Math::Vec2f::Vec2f(const float _x, const float _y) : x(_x), y(_y)
{

}

BSR::Math::Vec2f::Vec2f(const Vec2f& _Other) : x(_Other.x), y(_Other.y)
{

}

BSR::Math::Vec2f::Vec2f(Vec2f&& _Other) noexcept : x(_Other.x), y(_Other.y)
{

}

BSR::Math::Vec2f::~Vec2f()
{

}

float* BSR::Math::Vec2f::Data()
{
	return &x;
}

const float* BSR::Math::Vec2f::Data() const
{
	return &x;
}

const float BSR::Math::Vec2f::Magnitude() const
{
	return sqrtf(x * x + y * y);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::Normalize()
{
	*this /= Magnitude();

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Normalized() const
{
	return *this / Magnitude();
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator+ () const
{
	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator- () const
{
	return Vec2f(-x, -y);
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator+ (const Vec2f& _Other) const
{
	return Vec2f(x + _Other.x, y + _Other.y);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator+= (const Vec2f& _Other)
{
	x += _Other.x;
	y += _Other.y;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator+ (const float _Scale) const
{
	return Vec2f(x + _Scale, y + _Scale);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator+= (const float _Scale)
{
	x += _Scale;
	y += _Scale;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator- (const Vec2f& _Other) const
{
	return Vec2f(x - _Other.x, y - _Other.y);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator-= (const Vec2f& _Other)
{
	x -= _Other.x;
	y -= _Other.y;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator- (const float _Scale) const
{
	return Vec2f(x - _Scale, y - _Scale);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator-= (const float _Scale)
{
	x -= _Scale;
	y -= _Scale;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator* (const Vec2f& _Other) const
{
	return Vec2f(x * _Other.x, y * _Other.y);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator*= (const Vec2f& _Other)
{
	x *= _Other.x;
	y *= _Other.y;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator* (const float _Scale) const
{
	return Vec2f(x * _Scale, y * _Scale);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator*= (const float _Scale)
{
	x *= _Scale;
	y *= _Scale;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator/ (const Vec2f& _Other) const
{
	return Vec2f(x / _Other.x, y / _Other.y);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator/= (const Vec2f& _Other)
{
	x /= _Other.x;
	y /= _Other.y;

	return *this;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::operator/ (const float _Scale) const
{
	return Vec2f(x / _Scale, y / _Scale);
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator/= (const float _Scale)
{
	x /= _Scale;
	y /= _Scale;

	return *this;
}

const bool BSR::Math::Vec2f::operator== (const Vec2f& _Other) const
{
	return x == _Other.x && y == _Other.y;
}

const bool BSR::Math::Vec2f::operator!= (const Vec2f& _Other) const
{
	return x != _Other.x || y != _Other.y;
}

float& BSR::Math::Vec2f::operator[] (const size_t _Index)
{
	return (&x)[_Index];
}

const float& BSR::Math::Vec2f::operator[] (const size_t _Index) const
{
	return (&x)[_Index];
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator= (const Vec2f& _Other)
{
	x = _Other.x;
	y = _Other.y;

	return *this;
}

const BSR::Math::Vec2f& BSR::Math::Vec2f::operator= (Vec2f&& _Other) noexcept
{
	x = _Other.x;
	y = _Other.y;

	return *this;
}

const float BSR::Math::Vec2f::GetAngle(const Vec2f& _A, const Vec2f& _B)
{
	return acosf(Dot(_A.Normalized(), _B.Normalized()));
}

const float BSR::Math::Vec2f::Distance(const Vec2f & _A, const Vec2f & _B)
{
	return (_B - _A).Magnitude();
}

const float BSR::Math::Vec2f::Dot(const Vec2f& _A, const Vec2f& _B)
{
	return _A.x * _B.x + _A.y * _B.y;
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Clamp(const Vec2f& _Value, const Vec2f& _Min, const Vec2f& _Max)
{
	return Vec2f(BSR::Math::Clamp(_Value.x, _Min.x, _Max.x), BSR::Math::Clamp(_Value.y, _Min.y, _Max.y));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Mix(const Vec2f& _A, const Vec2f& _B, const float _Percentage)
{
	return Vec2f(BSR::Math::Mix(_A.x, _B.x, _Percentage), BSR::Math::Mix(_A.y, _B.y, _Percentage));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Pow(const Vec2f& _Base, const Vec2f& _Pow)
{
	return Vec2f(powf(_Base.x, _Pow.x), powf(_Base.y, _Pow.y));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Exp(const Vec2f& _Pow)
{
	return Vec2f(expf(_Pow.x), expf(_Pow.y));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Min(const Vec2f& _A, const Vec2f& _B)
{
	return Vec2f(BSR::Math::Min(_A.x, _B.x), BSR::Math::Min(_A.y, _B.y));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Max(const Vec2f& _A, const Vec2f& _B)
{
	return Vec2f(BSR::Math::Max(_A.x, _B.x), BSR::Math::Max(_A.y, _B.y));
}

const BSR::Math::Vec2f BSR::Math::Vec2f::Reflect(const Vec2f& _Vec, const Vec2f& _Normal)
{
	return _Vec - _Normal * 2.0f * Dot(_Vec, _Normal) / _Normal.Magnitude() / _Normal.Magnitude();
}



BSR::Math::Vec3f::Vec3f() : x(0.0f), y(0.0f), z(0.0f)
{

}

BSR::Math::Vec3f::Vec3f(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)
{

}

BSR::Math::Vec3f::Vec3f(const Vec3f& _Other) : x(_Other.x), y(_Other.y), z(_Other.z)
{

}

BSR::Math::Vec3f::Vec3f(Vec3f&& _Other) noexcept : x(_Other.x), y(_Other.y), z(_Other.z)
{

}

BSR::Math::Vec3f::Vec3f(const Vec2f& _Other, const float _z) : x(_Other.x), y(_Other.y), z(_z)
{

}

BSR::Math::Vec3f::~Vec3f()
{

}

BSR::Math::Vec3f::operator const BSR::Math::Vec2f() const
{
	return Vec2f(x, y);
}

float* BSR::Math::Vec3f::Data()
{
	return &x;
}

const float* BSR::Math::Vec3f::Data() const
{
	return &x;
}

const float BSR::Math::Vec3f::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::Normalize()
{
	*this /= Magnitude();

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Normalized() const
{
	return *this / Magnitude();
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator+ () const
{
	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator- () const
{
	return Vec3f(-x, -y, -z);
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator+ (const Vec3f& _Other) const
{
	return Vec3f(x + _Other.x, y + _Other.y, z + _Other.z);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator+= (const Vec3f& _Other)
{
	x += _Other.x;
	y += _Other.y;
	z += _Other.z;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator+ (const float _Scale) const
{
	return Vec3f(x + _Scale, y + _Scale, z + _Scale);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator+= (const float _Scale)
{
	x += _Scale;
	y += _Scale;
	z += _Scale;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator- (const Vec3f& _Other) const
{
	return Vec3f(x - _Other.x, y - _Other.y, z - _Other.z);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator-= (const Vec3f& _Other)
{
	x -= _Other.x;
	y -= _Other.y;
	z -= _Other.z;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator- (const float _Scale) const
{
	return Vec3f(x - _Scale, y - _Scale, z - _Scale);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator-= (const float _Scale)
{
	x -= _Scale;
	y -= _Scale;
	z -= _Scale;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator* (const Vec3f& _Other) const
{
	return Vec3f(x * _Other.x, y * _Other.y, z * _Other.z);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator*= (const Vec3f& _Other)
{
	x *= _Other.x;
	y *= _Other.y;
	z *= _Other.z;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator* (const float _Scale) const
{
	return Vec3f(x * _Scale, y * _Scale, z * _Scale);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator*= (const float _Scale)
{
	x *= _Scale;
	y *= _Scale;
	z *= _Scale;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator/ (const Vec3f& _Other) const
{
	return Vec3f(x / _Other.x, y / _Other.y, z / _Other.z);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator/= (const Vec3f& _Other)
{
	x /= _Other.x;
	y /= _Other.y;
	z /= _Other.z;

	return *this;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::operator/ (const float _Scale) const
{
	return Vec3f(x / _Scale, y / _Scale, z / _Scale);
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator/= (const float _Scale)
{
	x /= _Scale;
	y /= _Scale;
	z /= _Scale;

	return *this;
}

const bool BSR::Math::Vec3f::operator== (const Vec3f& _Other) const
{
	return x == _Other.x && y == _Other.y && z == _Other.z;
}

const bool BSR::Math::Vec3f::operator!= (const Vec3f& _Other) const
{
	return x != _Other.x || y != _Other.y || z != _Other.z;
}

float& BSR::Math::Vec3f::operator[] (const size_t _Index)
{
	return (&x)[_Index];
}

const float& BSR::Math::Vec3f::operator[] (const size_t _Index) const
{
	return (&x)[_Index];
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator= (const Vec3f& _Other)
{
	x = _Other.x;
	y = _Other.y;
	z = _Other.z;

	return *this;
}

const BSR::Math::Vec3f& BSR::Math::Vec3f::operator= (Vec3f&& _Other) noexcept
{
	x = _Other.x;
	y = _Other.y;
	z = _Other.z;

	return *this;
}

const float BSR::Math::Vec3f::GetAngle(const Vec3f& _A, const Vec3f& _B)
{
	return acosf(Dot(_A.Normalized(), _B.Normalized()));
}

const float BSR::Math::Vec3f::Distance(const Vec3f& _A, const Vec3f& _B)
{
	return (_B - _A).Magnitude();
}

const float BSR::Math::Vec3f::Dot(const Vec3f& _A, const Vec3f& _B)
{
	return _A.x * _B.x + _A.y * _B.y + _A.z * _B.z;
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Cross(const Vec3f& _A, const Vec3f& _B)
{
	return Vec3f(_A.y * _B.z - _A.z * _B.y, _A.z * _B.x - _A.x * _B.z, _A.x * _B.y - _A.y * _B.x);
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Clamp(const Vec3f & _Value, const Vec3f & _Min, const Vec3f & _Max)
{
	return Vec3f(BSR::Math::Clamp(_Value.x, _Min.x, _Max.x), BSR::Math::Clamp(_Value.y, _Min.y, _Max.y), BSR::Math::Clamp(_Value.z, _Min.z, _Max.z));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Mix(const Vec3f & _A, const Vec3f & _B, const float _Percentage)
{
	return Vec3f(BSR::Math::Mix(_A.x, _B.x, _Percentage), BSR::Math::Mix(_A.y, _B.y, _Percentage), BSR::Math::Mix(_A.z, _B.z, _Percentage));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Pow(const Vec3f& _Base, const Vec3f& _Pow)
{
	return Vec3f(powf(_Base.x, _Pow.x), powf(_Base.y, _Pow.y), powf(_Base.z, _Pow.z));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Exp(const Vec3f& _Pow)
{
	return Vec3f(expf(_Pow.x), expf(_Pow.y), expf(_Pow.z));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Min(const Vec3f& _A, const Vec3f& _B)
{
	return Vec3f(BSR::Math::Min(_A.x, _B.x), BSR::Math::Min(_A.y, _B.y), BSR::Math::Min(_A.z, _B.z));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Max(const Vec3f& _A, const Vec3f& _B)
{
	return Vec3f(BSR::Math::Max(_A.x, _B.x), BSR::Math::Max(_A.y, _B.y), BSR::Math::Max(_A.z, _B.z));
}

const BSR::Math::Vec3f BSR::Math::Vec3f::Reflect(const Vec3f& _Vec, const Vec3f& _Normal)
{
	return _Vec - _Normal * 2.0f * Dot(_Vec, _Normal) / _Normal.Magnitude() / _Normal.Magnitude();
}



BSR::Math::Vec4f::Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{

}

BSR::Math::Vec4f::Vec4f(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w)
{

}

BSR::Math::Vec4f::Vec4f(const Vec4f& _Other) : x(_Other.x), y(_Other.y), z(_Other.z), w(_Other.w)
{

}

BSR::Math::Vec4f::Vec4f(Vec4f&& _Other) noexcept : x(_Other.x), y(_Other.y), z(_Other.z), w(_Other.w)
{

}

BSR::Math::Vec4f::Vec4f(const Vec2f& _Other, const float _z, const float _w) : x(_Other.x), y(_Other.y), z(_z), w(_w)
{

}

BSR::Math::Vec4f::Vec4f(const Vec3f& _Other, const float _w) : x(_Other.x), y(_Other.y), z(_Other.z), w(_w)
{

}

BSR::Math::Vec4f::~Vec4f()
{

}

BSR::Math::Vec4f::operator const BSR::Math::Vec2f() const
{
	return Vec2f(x, y);
}

BSR::Math::Vec4f::operator const BSR::Math::Vec3f() const
{
	return Vec3f(x, y, z);
}

float* BSR::Math::Vec4f::Data()
{
	return &x;
}

const float* BSR::Math::Vec4f::Data() const
{
	return &x;
}

const float BSR::Math::Vec4f::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::Normalize()
{
	*this /= Magnitude();

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Normalized() const
{
	return *this / Magnitude();
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator+ () const
{
	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator- () const
{
	return Vec4f(-x, -y, -z, -w);
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator+ (const Vec4f& _Other) const
{
	return Vec4f(x + _Other.x, y + _Other.y, z + _Other.z, w + _Other.w);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator+= (const Vec4f& _Other)
{
	x += _Other.x;
	y += _Other.y;
	z += _Other.z;
	w += _Other.w;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator+ (const float _Scale) const
{
	return Vec4f(x + _Scale, y + _Scale, z + _Scale, w + _Scale);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator+= (const float _Scale)
{
	x += _Scale;
	y += _Scale;
	z += _Scale;
	w += _Scale;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator- (const Vec4f& _Other) const
{
	return Vec4f(x - _Other.x, y - _Other.y, z - _Other.z, w - _Other.w);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator-= (const Vec4f& _Other)
{
	x -= _Other.x;
	y -= _Other.y;
	z -= _Other.z;
	w -= _Other.w;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator- (const float _Scale) const
{
	return Vec4f(x - _Scale, y - _Scale, z - _Scale, w - _Scale);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator-= (const float _Scale)
{
	x -= _Scale;
	y -= _Scale;
	z -= _Scale;
	w -= _Scale;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator* (const Vec4f& _Other) const
{
	return Vec4f(x * _Other.x, y * _Other.y, z * _Other.z, w * _Other.w);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator*= (const Vec4f& _Other)
{
	x *= _Other.x;
	y *= _Other.y;
	z *= _Other.z;
	w *= _Other.w;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator* (const float _Scale) const
{
	return Vec4f(x * _Scale, y * _Scale, z * _Scale, w * _Scale);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator*= (const float _Scale)
{
	x *= _Scale;
	y *= _Scale;
	z *= _Scale;
	w *= _Scale;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator/ (const Vec4f& _Other) const
{
	return Vec4f(x / _Other.x, y / _Other.y, z / _Other.z, w / _Other.w);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator/= (const Vec4f& _Other)
{
	x /= _Other.x;
	y /= _Other.y;
	z /= _Other.z;
	w /= _Other.w;

	return *this;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::operator/ (const float _Scale) const
{
	return Vec4f(x / _Scale, y / _Scale, z / _Scale, w / _Scale);
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator/= (const float _Scale)
{
	x /= _Scale;
	y /= _Scale;
	z /= _Scale;
	w /= _Scale;

	return *this;
}

const bool BSR::Math::Vec4f::operator== (const Vec4f& _Other) const
{
	return x == _Other.x && y == _Other.y && z == _Other.z && w == _Other.w;
}

const bool BSR::Math::Vec4f::operator!= (const Vec4f& _Other) const
{
	return x != _Other.x || y != _Other.y || z != _Other.z || w != _Other.w;
}

float& BSR::Math::Vec4f::operator[] (const size_t _Index)
{
	return (&x)[_Index];
}

const float& BSR::Math::Vec4f::operator[] (const size_t _Index) const
{
	return (&x)[_Index];
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator= (const Vec4f& _Other)
{
	x = _Other.x;
	y = _Other.y;
	z = _Other.z;
	w = _Other.w;

	return *this;
}

const BSR::Math::Vec4f& BSR::Math::Vec4f::operator= (Vec4f&& _Other) noexcept
{
	x = _Other.x;
	y = _Other.y;
	z = _Other.z;
	w = _Other.w;

	return *this;
}

const float BSR::Math::Vec4f::GetAngle(const Vec4f& _A, const Vec4f& _B)
{
	return acosf(Vec3f::Dot(((Vec3f)(_A)).Normalized(), ((Vec3f)(_B)).Normalized()));
}

const float BSR::Math::Vec4f::Distance(const Vec4f & _A, const Vec4f & _B)
{
	return (_B - _A).Magnitude();
}

const float BSR::Math::Vec4f::Dot(const Vec4f& _A, const Vec4f& _B)
{
	return _A.x * _B.x + _A.y * _B.y + _A.z * _B.z + _A.w * _B.w;
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Cross(const Vec4f& _A, const Vec4f& _B)
{
	return Vec4f(_A.y * _B.z - _A.z * _B.y, _A.z * _B.x - _A.x * _B.z, _A.x * _B.y - _A.y * _B.x, 1.0f);
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Clamp(const Vec4f& _Value, const Vec4f& _Min, const Vec4f& _Max)
{
	return Vec4f(BSR::Math::Clamp(_Value.x, _Min.x, _Max.x), BSR::Math::Clamp(_Value.y, _Min.y, _Max.y), BSR::Math::Clamp(_Value.z, _Min.z, _Max.z), BSR::Math::Clamp(_Value.w, _Min.w, _Max.w));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Mix(const Vec4f& _A, const Vec4f& _B, const float _Percentage)
{
	return Vec4f(BSR::Math::Mix(_A.x, _B.x, _Percentage), BSR::Math::Mix(_A.y, _B.y, _Percentage), BSR::Math::Mix(_A.z, _B.z, _Percentage), BSR::Math::Mix(_A.w, _B.w, _Percentage));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Pow(const Vec4f& _Base, const Vec4f& _Pow)
{
	return Vec4f(powf(_Base.x, _Pow.x), powf(_Base.y, _Pow.y), powf(_Base.z, _Pow.z), powf(_Base.w, _Pow.w));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Exp(const Vec4f& _Pow)
{
	return Vec4f(expf(_Pow.x), expf(_Pow.y), expf(_Pow.z), expf(_Pow.w));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Min(const Vec4f& _A, const Vec4f& _B)
{
	return Vec4f(BSR::Math::Min(_A.x, _B.x), BSR::Math::Min(_A.y, _B.y), BSR::Math::Min(_A.z, _B.z), BSR::Math::Min(_A.w, _B.w));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Max(const Vec4f& _A, const Vec4f& _B)
{
	return Vec4f(BSR::Math::Max(_A.x, _B.x), BSR::Math::Max(_A.y, _B.y), BSR::Math::Max(_A.z, _B.z), BSR::Math::Max(_A.w, _B.w));
}

const BSR::Math::Vec4f BSR::Math::Vec4f::Reflect(const Vec4f& _Vec, const Vec4f& _Normal)
{
	return _Vec - _Normal * 2.0f * Dot(_Vec, _Normal) / _Normal.Magnitude() / _Normal.Magnitude();
}
