#include "..\Headers\Main.hpp"



Math::Mat2f::Mat2f() : Matrix()
{
	Matrix[0][0] = 1.0f; Matrix[0][1] = 0.0f;
	Matrix[1][0] = 0.0f; Matrix[1][1] = 1.0f;
}

Math::Mat2f::Mat2f(const Mat2f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1];
}

Math::Mat2f::~Mat2f()
{

}

float* Math::Mat2f::Data()
{
	return (float*)(Matrix);
}

const float* Math::Mat2f::Data() const
{
	return (const float*)(Matrix);
}

const float Math::Mat2f::Determinant() const
{
	return Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0];
}

const float Math::Mat2f::Trace() const
{
	return Matrix[0][0] + Matrix[1][1];
}

const Math::Mat2f& Math::Mat2f::Transpose()
{
	Swap(Matrix[0][1], Matrix[1][0]);

	return *this;
}

const Math::Mat2f Math::Mat2f::Transposed() const
{
	return Mat2f(*this).Transpose();
}

const Math::Mat2f& Math::Mat2f::Inverse()
{
	Mat2f _Matrix;

	_Matrix[0][0] = Matrix[1][1]; _Matrix[0][1] = -Matrix[0][1];
	_Matrix[1][0] = -Matrix[1][0]; _Matrix[1][1] = Matrix[0][0];

	_Matrix /= Determinant();

	*this = _Matrix;

	return *this;
}

const Math::Mat2f Math::Mat2f::Inversed() const
{
	return Mat2f(*this).Inverse();
}

const Math::Mat2f& Math::Mat2f::operator+ () const
{
	return *this;
}

const Math::Mat2f Math::Mat2f::operator- () const
{
	Mat2f _Matrix;

	_Matrix[0][0] = -Matrix[0][0]; _Matrix[0][1] = -Matrix[0][1];
	_Matrix[1][0] = -Matrix[1][0]; _Matrix[1][1] = -Matrix[1][1];

	return _Matrix;
}

const Math::Vec2f Math::Mat2f::operator* (const Vec2f& _Vec) const
{
	return Vec2f
	(
		Matrix[0][0] * _Vec.x + Matrix[0][1] * _Vec.y,
		Matrix[1][0] * _Vec.x + Matrix[1][1] * _Vec.y
	);
}

const Math::Mat2f Math::Mat2f::operator+ (const Mat2f& _Other) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] += _Other[0][0]; _Matrix[0][1] += _Other[0][1];
	_Matrix[1][0] += _Other[1][0]; _Matrix[1][1] += _Other[1][1];

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator+= (const Mat2f& _Other)
{
	Matrix[0][0] += _Other[0][0]; Matrix[0][1] += _Other[0][1];
	Matrix[1][0] += _Other[1][0]; Matrix[1][1] += _Other[1][1];

	return *this;
}

const Math::Mat2f Math::Mat2f::operator+ (const float _Scale) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] += _Scale; _Matrix[0][1] += _Scale;
	_Matrix[1][0] += _Scale; _Matrix[1][1] += _Scale;

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator+= (const float _Scale)
{
	Matrix[0][0] += _Scale; Matrix[0][1] += _Scale;
	Matrix[1][0] += _Scale; Matrix[1][1] += _Scale;

	return *this;
}

const Math::Mat2f Math::Mat2f::operator- (const Mat2f& _Other) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] -= _Other[0][0]; _Matrix[0][1] -= _Other[0][1];
	_Matrix[1][0] -= _Other[1][0]; _Matrix[1][1] -= _Other[1][1];

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator-= (const Mat2f& _Other)
{
	Matrix[0][0] -= _Other[0][0]; Matrix[0][1] -= _Other[0][1];
	Matrix[1][0] -= _Other[1][0]; Matrix[1][1] -= _Other[1][1];

	return *this;
}

const Math::Mat2f Math::Mat2f::operator- (const float _Scale) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] -= _Scale; _Matrix[0][1] -= _Scale;
	_Matrix[1][0] -= _Scale; _Matrix[1][1] -= _Scale;

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator-= (const float _Scale)
{
	Matrix[0][0] -= _Scale; Matrix[0][1] -= _Scale;
	Matrix[1][0] -= _Scale; Matrix[1][1] -= _Scale;

	return *this;
}

const Math::Mat2f Math::Mat2f::operator* (const Mat2f& _Other) const
{
	Mat2f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1];
	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1];

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator*= (const Mat2f& _Other)
{
	Mat2f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1];
	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1];

	*this = _Matrix;

	return *this;
}

const Math::Mat2f Math::Mat2f::operator* (const float _Scale) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] *= _Scale; _Matrix[0][1] *= _Scale;
	_Matrix[1][0] *= _Scale; _Matrix[1][1] *= _Scale;

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator*= (const float _Scale)
{
	Matrix[0][0] *= _Scale; Matrix[0][1] *= _Scale;
	Matrix[1][0] *= _Scale; Matrix[1][1] *= _Scale;

	return *this;
}

const Math::Mat2f Math::Mat2f::operator/ (const float _Scale) const
{
	Mat2f _Matrix(*this);

	_Matrix[0][0] /= _Scale; _Matrix[0][1] /= _Scale;
	_Matrix[1][0] /= _Scale; _Matrix[1][1] /= _Scale;

	return _Matrix;
}

const Math::Mat2f& Math::Mat2f::operator/= (const float _Scale)
{
	Matrix[0][0] /= _Scale; Matrix[0][1] /= _Scale;
	Matrix[1][0] /= _Scale; Matrix[1][1] /= _Scale;

	return *this;
}

const bool Math::Mat2f::operator== (const Mat2f& _Other) const
{
	return
		Matrix[0][0] == _Other[0][0] && Matrix[0][1] == _Other[0][1] &&
		Matrix[1][0] == _Other[1][0] && Matrix[1][1] == _Other[1][1];
}

const bool Math::Mat2f::operator!= (const Mat2f& _Other) const
{
	return
		Matrix[0][0] != _Other[0][0] || Matrix[0][1] != _Other[0][1] ||
		Matrix[1][0] != _Other[1][0] || Matrix[1][1] != _Other[1][1];
}

float* Math::Mat2f::operator[] (const size_t _Index)
{
	return Matrix[_Index].Data();
}

const float* Math::Mat2f::operator[] (const size_t _Index) const
{
	return Matrix[_Index].Data();
}

const Math::Mat2f& Math::Mat2f::operator= (const Mat2f& _Other)
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1];

	return *this;
}

const Math::Mat2f Math::Mat2f::GetScale(const float _x, const float _y)
{
	Mat2f _Matrix;

	_Matrix[0][0] = _x;
	_Matrix[1][1] = _y;

	return _Matrix;
}

const Math::Mat2f Math::Mat2f::GetFill(const float _Value)
{
	Mat2f _Matrix;

	_Matrix[0][0] = _Value; _Matrix[0][1] = _Value;
	_Matrix[1][0] = _Value; _Matrix[1][1] = _Value;

	return _Matrix;
}

const Math::Mat2f Math::Mat2f::GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis)
{
	Mat2f _Matrix;

	_Matrix[_ShearedAxis][_ShearByAxis] = _Value;

	return _Matrix;
}

const Math::Mat2f Math::Mat2f::GetRotation(const float _Angle)
{
	Mat2f _Matrix;

	float _Sin = sinf(_Angle);
	float _Cos = cosf(_Angle);

	_Matrix[0][0] = _Cos; _Matrix[0][1] = -_Sin;
	_Matrix[1][0] = _Sin; _Matrix[1][1] = _Cos;

	return _Matrix;
}



Math::Mat3f::Mat3f() : Matrix()
{
	Matrix[0][0] = 1.0f; Matrix[0][1] = 0.0f; Matrix[0][2] = 0.0f;
	Matrix[1][0] = 0.0f; Matrix[1][1] = 1.0f; Matrix[1][2] = 0.0f;
	Matrix[2][0] = 0.0f; Matrix[2][1] = 0.0f; Matrix[2][2] = 1.0f;
}

Math::Mat3f::Mat3f(const Mat3f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = _Other[0][2];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = _Other[1][2];
	Matrix[2][0] = _Other[2][0]; Matrix[2][1] = _Other[2][1]; Matrix[2][2] = _Other[2][2];
}

Math::Mat3f::Mat3f(const Mat2f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = 0.0f;
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = 0.0f;
	Matrix[2][0] = 0.0f; Matrix[2][1] = 0.0f; Matrix[2][2] = 1.0f;
}

Math::Mat3f::~Mat3f()
{

}

const Math::Mat2f Math::Mat3f::ToMat2f() const
{
	Mat2f _Matrix;

	_Matrix[0][0] = Matrix[0][0]; _Matrix[0][1] = Matrix[0][1];
	_Matrix[1][0] = Matrix[1][0]; _Matrix[1][1] = Matrix[1][1];

	return _Matrix;
}

float* Math::Mat3f::Data()
{
	return (float*)(Matrix);
}

const float* Math::Mat3f::Data() const
{
	return (const float*)(Matrix);
}

const float Math::Mat3f::Determinant() const
{
	return
		Matrix[0][0] * Matrix[1][1] * Matrix[2][2] +
		Matrix[0][2] * Matrix[1][0] * Matrix[2][1] +
		Matrix[0][1] * Matrix[1][2] * Matrix[2][0] -
		Matrix[0][2] * Matrix[1][1] * Matrix[2][0] -
		Matrix[0][0] * Matrix[1][2] * Matrix[2][1] -
		Matrix[0][1] * Matrix[1][0] * Matrix[2][2];
}

const float Math::Mat3f::Trace() const
{
	return Matrix[0][0] + Matrix[1][1] + Matrix[2][2];
}

const Math::Mat3f& Math::Mat3f::Transpose()
{
	Swap(Matrix[0][1], Matrix[1][0]);
	Swap(Matrix[0][2], Matrix[2][0]);
	Swap(Matrix[1][2], Matrix[2][1]);

	return *this;
}

const Math::Mat3f Math::Mat3f::Transposed() const
{
	return Mat3f(*this).Transpose();
}

const Math::Mat3f& Math::Mat3f::Inverse()
{
	Mat3f _Matrix;

	Mat2f _DetMat;

	_DetMat[0][0] = Matrix[1][1]; _DetMat[0][1] = Matrix[2][1];
	_DetMat[1][0] = Matrix[1][2]; _DetMat[1][1] = Matrix[2][2];

	_Matrix[0][0] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][1]; _DetMat[0][1] = Matrix[2][1];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[2][2];

	_Matrix[0][1] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][1]; _DetMat[0][1] = Matrix[1][1];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2];

	_Matrix[0][2] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[1][0]; _DetMat[0][1] = Matrix[2][0];
	_DetMat[1][0] = Matrix[1][2]; _DetMat[1][1] = Matrix[2][2];

	_Matrix[1][0] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[2][0];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[2][2];

	_Matrix[1][1] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2];

	_Matrix[1][2] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[1][0]; _DetMat[0][1] = Matrix[2][0];
	_DetMat[1][0] = Matrix[1][1]; _DetMat[1][1] = Matrix[2][1];

	_Matrix[2][0] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[2][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[2][1];

	_Matrix[2][1] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[1][1];

	_Matrix[2][2] = _DetMat.Determinant();

	_Matrix /= Determinant();

	*this = _Matrix;

	return *this;
}

const Math::Mat3f Math::Mat3f::Inversed() const
{
	return Mat3f(*this).Inverse();
}

const Math::Mat3f& Math::Mat3f::operator+ () const
{
	return *this;
}

const Math::Mat3f Math::Mat3f::operator- () const
{
	Mat3f _Matrix;

	_Matrix[0][0] = -Matrix[0][0]; _Matrix[0][1] = -Matrix[0][1]; _Matrix[0][2] = -Matrix[0][2];
	_Matrix[1][0] = -Matrix[1][0]; _Matrix[1][1] = -Matrix[1][1]; _Matrix[1][2] = -Matrix[1][2];
	_Matrix[2][0] = -Matrix[2][0]; _Matrix[2][1] = -Matrix[2][1]; _Matrix[2][2] = -Matrix[2][2];

	return _Matrix;
}

const Math::Vec3f Math::Mat3f::operator* (const Vec3f& _Vec) const
{
	return Vec3f
	(
		Matrix[0][0] * _Vec.x + Matrix[0][1] * _Vec.y + Matrix[0][2] * _Vec.z,
		Matrix[1][0] * _Vec.x + Matrix[1][1] * _Vec.y + Matrix[1][2] * _Vec.z,
		Matrix[2][0] * _Vec.x + Matrix[2][1] * _Vec.y + Matrix[2][2] * _Vec.z
	);
}

const Math::Mat3f Math::Mat3f::operator+ (const Mat3f& _Other) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] += _Other[0][0]; _Matrix[0][1] += _Other[0][1]; _Matrix[0][2] += _Other[0][2];
	_Matrix[1][0] += _Other[1][0]; _Matrix[1][1] += _Other[1][1]; _Matrix[1][2] += _Other[1][2];
	_Matrix[2][0] += _Other[2][0]; _Matrix[2][1] += _Other[2][1]; _Matrix[2][2] += _Other[2][2];

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator+= (const Mat3f& _Other)
{
	Matrix[0][0] += _Other[0][0]; Matrix[0][1] += _Other[0][1]; Matrix[0][2] += _Other[0][2];
	Matrix[1][0] += _Other[1][0]; Matrix[1][1] += _Other[1][1]; Matrix[1][2] += _Other[1][2];
	Matrix[2][0] += _Other[2][0]; Matrix[2][1] += _Other[2][1]; Matrix[2][2] += _Other[2][2];

	return *this;
}

const Math::Mat3f Math::Mat3f::operator+ (const float _Scale) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] += _Scale; _Matrix[0][1] += _Scale; _Matrix[0][2] += _Scale;
	_Matrix[1][0] += _Scale; _Matrix[1][1] += _Scale; _Matrix[1][2] += _Scale;
	_Matrix[2][0] += _Scale; _Matrix[2][1] += _Scale; _Matrix[2][2] += _Scale;

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator+= (const float _Scale)
{
	Matrix[0][0] += _Scale; Matrix[0][1] += _Scale; Matrix[0][2] += _Scale;
	Matrix[1][0] += _Scale; Matrix[1][1] += _Scale; Matrix[1][2] += _Scale;
	Matrix[2][0] += _Scale; Matrix[2][1] += _Scale; Matrix[2][2] += _Scale;

	return *this;
}

const Math::Mat3f Math::Mat3f::operator- (const Mat3f& _Other) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] -= _Other[0][0]; _Matrix[0][1] -= _Other[0][1]; _Matrix[0][2] -= _Other[0][2];
	_Matrix[1][0] -= _Other[1][0]; _Matrix[1][1] -= _Other[1][1]; _Matrix[1][2] -= _Other[1][2];
	_Matrix[2][0] -= _Other[2][0]; _Matrix[2][1] -= _Other[2][1]; _Matrix[2][2] -= _Other[2][2];

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator-= (const Mat3f& _Other)
{
	Matrix[0][0] -= _Other[0][0]; Matrix[0][1] -= _Other[0][1]; Matrix[0][2] -= _Other[0][2];
	Matrix[1][0] -= _Other[1][0]; Matrix[1][1] -= _Other[1][1]; Matrix[1][2] -= _Other[1][2];
	Matrix[2][0] -= _Other[2][0]; Matrix[2][1] -= _Other[2][1]; Matrix[2][2] -= _Other[2][2];

	return *this;
}

const Math::Mat3f Math::Mat3f::operator- (const float _Scale) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] -= _Scale; _Matrix[0][1] -= _Scale; _Matrix[0][2] -= _Scale;
	_Matrix[1][0] -= _Scale; _Matrix[1][1] -= _Scale; _Matrix[1][2] -= _Scale;
	_Matrix[2][0] -= _Scale; _Matrix[2][1] -= _Scale; _Matrix[2][2] -= _Scale;

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator-= (const float _Scale)
{
	Matrix[0][0] -= _Scale; Matrix[0][1] -= _Scale; Matrix[0][2] -= _Scale;
	Matrix[1][0] -= _Scale; Matrix[1][1] -= _Scale; Matrix[1][2] -= _Scale;
	Matrix[2][0] -= _Scale; Matrix[2][1] -= _Scale; Matrix[2][2] -= _Scale;

	return *this;
}

const Math::Mat3f Math::Mat3f::operator* (const Mat3f& _Other) const
{
	Mat3f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0] + Matrix[0][2] * _Other[2][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1] + Matrix[0][2] * _Other[2][1];
	_Matrix[0][2] = Matrix[0][0] * _Other[0][2] + Matrix[0][1] * _Other[1][2] + Matrix[0][2] * _Other[2][2];

	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0] + Matrix[1][2] * _Other[2][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1] + Matrix[1][2] * _Other[2][1];
	_Matrix[1][2] = Matrix[1][0] * _Other[0][2] + Matrix[1][1] * _Other[1][2] + Matrix[1][2] * _Other[2][2];

	_Matrix[2][0] = Matrix[2][0] * _Other[0][0] + Matrix[2][1] * _Other[1][0] + Matrix[2][2] * _Other[2][0];
	_Matrix[2][1] = Matrix[2][0] * _Other[0][1] + Matrix[2][1] * _Other[1][1] + Matrix[2][2] * _Other[2][1];
	_Matrix[2][2] = Matrix[2][0] * _Other[0][2] + Matrix[2][1] * _Other[1][2] + Matrix[2][2] * _Other[2][2];

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator*= (const Mat3f& _Other)
{
	Mat3f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0] + Matrix[0][2] * _Other[2][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1] + Matrix[0][2] * _Other[2][1];
	_Matrix[0][2] = Matrix[0][0] * _Other[0][2] + Matrix[0][1] * _Other[1][2] + Matrix[0][2] * _Other[2][2];

	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0] + Matrix[1][2] * _Other[2][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1] + Matrix[1][2] * _Other[2][1];
	_Matrix[1][2] = Matrix[1][0] * _Other[0][2] + Matrix[1][1] * _Other[1][2] + Matrix[1][2] * _Other[2][2];

	_Matrix[2][0] = Matrix[2][0] * _Other[0][0] + Matrix[2][1] * _Other[1][0] + Matrix[2][2] * _Other[2][0];
	_Matrix[2][1] = Matrix[2][0] * _Other[0][1] + Matrix[2][1] * _Other[1][1] + Matrix[2][2] * _Other[2][1];
	_Matrix[2][2] = Matrix[2][0] * _Other[0][2] + Matrix[2][1] * _Other[1][2] + Matrix[2][2] * _Other[2][2];

	*this = _Matrix;

	return *this;
}

const Math::Mat3f Math::Mat3f::operator* (const float _Scale) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] *= _Scale; _Matrix[0][1] *= _Scale; _Matrix[0][2] *= _Scale;
	_Matrix[1][0] *= _Scale; _Matrix[1][1] *= _Scale; _Matrix[1][2] *= _Scale;
	_Matrix[2][0] *= _Scale; _Matrix[2][1] *= _Scale; _Matrix[2][2] *= _Scale;

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator*= (const float _Scale)
{
	Matrix[0][0] *= _Scale; Matrix[0][1] *= _Scale; Matrix[0][2] *= _Scale;
	Matrix[1][0] *= _Scale; Matrix[1][1] *= _Scale; Matrix[1][2] *= _Scale;
	Matrix[2][0] *= _Scale; Matrix[2][1] *= _Scale; Matrix[2][2] *= _Scale;

	return *this;
}

const Math::Mat3f Math::Mat3f::operator/ (const float _Scale) const
{
	Mat3f _Matrix(*this);

	_Matrix[0][0] /= _Scale; _Matrix[0][1] /= _Scale; _Matrix[0][2] /= _Scale;
	_Matrix[1][0] /= _Scale; _Matrix[1][1] /= _Scale; _Matrix[1][2] /= _Scale;
	_Matrix[2][0] /= _Scale; _Matrix[2][1] /= _Scale; _Matrix[2][2] /= _Scale;

	return _Matrix;
}

const Math::Mat3f& Math::Mat3f::operator/= (const float _Scale)
{
	Matrix[0][0] /= _Scale; Matrix[0][1] /= _Scale; Matrix[0][2] /= _Scale;
	Matrix[1][0] /= _Scale; Matrix[1][1] /= _Scale; Matrix[1][2] /= _Scale;
	Matrix[2][0] /= _Scale; Matrix[2][1] /= _Scale; Matrix[2][2] /= _Scale;

	return *this;
}

const bool Math::Mat3f::operator== (const Mat3f& _Other) const
{
	return
		Matrix[0][0] == _Other[0][0] && Matrix[0][1] == _Other[0][1] && Matrix[0][2] == _Other[0][2] &&
		Matrix[1][0] == _Other[1][0] && Matrix[1][1] == _Other[1][1] && Matrix[1][2] == _Other[1][2] &&
		Matrix[2][0] == _Other[2][0] && Matrix[2][1] == _Other[2][1] && Matrix[2][2] == _Other[2][2];
}

const bool Math::Mat3f::operator!= (const Mat3f& _Other) const
{
	return
		Matrix[0][0] != _Other[0][0] || Matrix[0][1] != _Other[0][1] || Matrix[0][2] != _Other[0][2] ||
		Matrix[1][0] != _Other[1][0] || Matrix[1][1] != _Other[1][1] || Matrix[1][2] != _Other[1][2] ||
		Matrix[2][0] != _Other[2][0] || Matrix[2][1] != _Other[2][1] || Matrix[2][2] != _Other[2][2];
}

float* Math::Mat3f::operator[] (const size_t _Index)
{
	return Matrix[_Index].Data();
}

const float* Math::Mat3f::operator[] (const size_t _Index) const
{
	return Matrix[_Index].Data();
}

const Math::Mat3f& Math::Mat3f::operator= (const Mat3f& _Other)
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = _Other[0][2];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = _Other[1][2];
	Matrix[2][0] = _Other[2][0]; Matrix[2][1] = _Other[2][1]; Matrix[2][2] = _Other[2][2];

	return *this;
}

const Math::Mat3f Math::Mat3f::GetScale(const float _x, const float _y, const float _z)
{
	Mat3f _Matrix;

	_Matrix[0][0] = _x;
	_Matrix[1][1] = _y;
	_Matrix[2][2] = _z;

	return _Matrix;
}

const Math::Mat3f Math::Mat3f::GetFill(const float _Value)
{
	Mat3f _Matrix;

	_Matrix[0][0] = _Value; _Matrix[0][1] = _Value; _Matrix[0][2] = _Value;
	_Matrix[1][0] = _Value; _Matrix[1][1] = _Value; _Matrix[1][2] = _Value;
	_Matrix[2][0] = _Value; _Matrix[2][1] = _Value; _Matrix[2][2] = _Value;

	return _Matrix;
}

const Math::Mat3f Math::Mat3f::GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis)
{
	Mat3f _Matrix;

	_Matrix[_ShearedAxis][_ShearByAxis] = _Value;

	return _Matrix;
}

const Math::Mat3f Math::Mat3f::GetRotation(const float _Angle, const Vec3f& _RotationAxis)
{
	Mat3f _Matrix;

	float _Sin = sinf(_Angle);
	float _Cos = cosf(_Angle);

	_Matrix[0][0] = _Cos + _RotationAxis.x * _RotationAxis.x * (1.0f - _Cos);
	_Matrix[0][1] = _RotationAxis.x * _RotationAxis.y * (1.0f - _Cos) - _RotationAxis.z * _Sin;
	_Matrix[0][2] = _RotationAxis.x * _RotationAxis.z * (1.0f - _Cos) + _RotationAxis.y * _Sin;

	_Matrix[1][0] = _RotationAxis.y * _RotationAxis.x * (1.0f - _Cos) + _RotationAxis.z * _Sin;
	_Matrix[1][1] = _Cos + _RotationAxis.y * _RotationAxis.y * (1.0f - _Cos);
	_Matrix[1][2] = _RotationAxis.y * _RotationAxis.z * (1.0f - _Cos) - _RotationAxis.x * _Sin;

	_Matrix[2][0] = _RotationAxis.z * _RotationAxis.x * (1.0f - _Cos) - _RotationAxis.y * _Sin;
	_Matrix[2][1] = _RotationAxis.z * _RotationAxis.y * (1.0f - _Cos) + _RotationAxis.x * _Sin;
	_Matrix[2][2] = _Cos + _RotationAxis.z * _RotationAxis.z * (1.0f - _Cos);

	return _Matrix;
}

const Math::Mat3f Math::Mat3f::GetTranslation(const Vec2f& _Coords)
{
	Mat3f _Matrix;

	_Matrix[0][2] = _Coords.x;
	_Matrix[1][2] = _Coords.y;

	return _Matrix;
}

const Math::Mat3f Math::Mat3f::GetOrtho(const float _Left, const float _Right, const float _Bottom, const float _Top)
{
	Mat3f _Matrix;

	_Matrix[0][0] = 2.0f / (_Right - _Left);
	_Matrix[1][1] = 2.0f / (_Top - _Bottom);

	_Matrix[0][2] = (_Right + _Left) / (_Left - _Right);
	_Matrix[1][2] = (_Top + _Bottom) / (_Bottom - _Top);

	return _Matrix;
}



Math::Mat4f::Mat4f() : Matrix()
{
	Matrix[0][0] = 1.0f; Matrix[0][1] = 0.0f; Matrix[0][2] = 0.0f; Matrix[0][3] = 0.0f;
	Matrix[1][0] = 0.0f; Matrix[1][1] = 1.0f; Matrix[1][2] = 0.0f; Matrix[1][3] = 0.0f;
	Matrix[2][0] = 0.0f; Matrix[2][1] = 0.0f; Matrix[2][2] = 1.0f; Matrix[2][3] = 0.0f;
	Matrix[3][0] = 0.0f; Matrix[3][1] = 0.0f; Matrix[3][2] = 0.0f; Matrix[3][3] = 1.0f;
}

Math::Mat4f::Mat4f(const Mat4f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = _Other[0][2]; Matrix[0][3] = _Other[0][3];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = _Other[1][2]; Matrix[1][3] = _Other[1][3];
	Matrix[2][0] = _Other[2][0]; Matrix[2][1] = _Other[2][1]; Matrix[2][2] = _Other[2][2]; Matrix[2][3] = _Other[2][3];
	Matrix[3][0] = _Other[3][0]; Matrix[3][1] = _Other[3][1]; Matrix[3][2] = _Other[3][2]; Matrix[3][3] = _Other[3][3];
}

Math::Mat4f::Mat4f(const Mat2f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = 0.0f; Matrix[0][3] = 0.0f;
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = 0.0f; Matrix[1][3] = 0.0f;
	Matrix[2][0] = 0.0f; Matrix[2][1] = 0.0f; Matrix[2][2] = 1.0f; Matrix[2][3] = 0.0f;
	Matrix[3][0] = 0.0f; Matrix[3][1] = 0.0f; Matrix[3][2] = 0.0f; Matrix[3][3] = 1.0f;
}

Math::Mat4f::Mat4f(const Mat3f& _Other) : Matrix()
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = _Other[0][2]; Matrix[0][3] = 0.0f;
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = _Other[1][2]; Matrix[1][3] = 0.0f;
	Matrix[2][0] = _Other[2][0]; Matrix[2][1] = _Other[2][1]; Matrix[2][2] = _Other[2][2]; Matrix[2][3] = 0.0f;
	Matrix[3][0] = 0.0f; Matrix[3][1] = 0.0f; Matrix[3][2] = 0.0f; Matrix[3][3] = 1.0f;
}

Math::Mat4f::~Mat4f()
{

}

const Math::Mat2f Math::Mat4f::ToMat2f() const
{
	Mat2f _Matrix;

	_Matrix[0][0] = Matrix[0][0]; _Matrix[0][1] = Matrix[0][1];
	_Matrix[1][0] = Matrix[1][0]; _Matrix[1][1] = Matrix[1][1];

	return _Matrix;
}

const Math::Mat3f Math::Mat4f::ToMat3f() const
{
	Mat3f _Matrix;

	_Matrix[0][0] = Matrix[0][0]; _Matrix[0][1] = Matrix[0][1]; _Matrix[0][2] = Matrix[0][2];
	_Matrix[1][0] = Matrix[1][0]; _Matrix[1][1] = Matrix[1][1]; _Matrix[1][2] = Matrix[1][2];
	_Matrix[2][0] = Matrix[2][0]; _Matrix[2][1] = Matrix[2][1]; _Matrix[2][2] = Matrix[2][2];

	return _Matrix;
}

float* Math::Mat4f::Data()
{
	return (float*)(Matrix);
}

const float* Math::Mat4f::Data() const
{
	return (const float*)(Matrix);
}

const float Math::Mat4f::Determinant() const
{
	Mat3f _Mat[4];

	_Mat[0][0][0] = Matrix[1][1]; _Mat[0][0][1] = Matrix[1][2]; _Mat[0][0][2] = Matrix[1][3];
	_Mat[0][1][0] = Matrix[2][1]; _Mat[0][1][1] = Matrix[2][2]; _Mat[0][1][2] = Matrix[2][3];
	_Mat[0][2][0] = Matrix[3][1]; _Mat[0][2][1] = Matrix[3][2]; _Mat[0][2][2] = Matrix[3][3];

	_Mat[1][0][0] = Matrix[0][1]; _Mat[1][0][1] = Matrix[0][2]; _Mat[1][0][2] = Matrix[0][3];
	_Mat[1][1][0] = Matrix[2][1]; _Mat[1][1][1] = Matrix[2][2]; _Mat[1][1][2] = Matrix[2][3];
	_Mat[1][2][0] = Matrix[3][1]; _Mat[1][2][1] = Matrix[3][2]; _Mat[1][2][2] = Matrix[3][3];

	_Mat[2][0][0] = Matrix[0][1]; _Mat[2][0][1] = Matrix[0][2]; _Mat[2][0][2] = Matrix[0][3];
	_Mat[2][1][0] = Matrix[1][1]; _Mat[2][1][1] = Matrix[1][2]; _Mat[2][1][2] = Matrix[1][3];
	_Mat[2][2][0] = Matrix[3][1]; _Mat[2][2][1] = Matrix[3][2]; _Mat[2][2][2] = Matrix[3][3];

	_Mat[3][0][0] = Matrix[0][1]; _Mat[3][0][1] = Matrix[0][2]; _Mat[3][0][2] = Matrix[0][3];
	_Mat[3][1][0] = Matrix[1][1]; _Mat[3][1][1] = Matrix[1][2]; _Mat[3][1][2] = Matrix[1][3];
	_Mat[3][2][0] = Matrix[2][1]; _Mat[3][2][1] = Matrix[2][2]; _Mat[3][2][2] = Matrix[2][3];

	return
		Matrix[0][0] * _Mat[0].Determinant() -
		Matrix[1][0] * _Mat[1].Determinant() +
		Matrix[2][0] * _Mat[2].Determinant() -
		Matrix[3][0] * _Mat[3].Determinant();
}

const float Math::Mat4f::Trace() const
{
	return Matrix[0][0] + Matrix[1][1] + Matrix[2][2] + Matrix[3][3];
}

const Math::Mat4f& Math::Mat4f::Transpose()
{
	Swap(Matrix[0][1], Matrix[1][0]);
	Swap(Matrix[0][2], Matrix[2][0]);
	Swap(Matrix[0][3], Matrix[3][0]);
	Swap(Matrix[1][2], Matrix[2][1]);
	Swap(Matrix[1][3], Matrix[3][1]);
	Swap(Matrix[2][3], Matrix[3][2]);

	return *this;
}

const Math::Mat4f Math::Mat4f::Transposed() const
{
	return Mat4f(*this).Transpose();
}

const Math::Mat4f& Math::Mat4f::Inverse()
{
	Mat4f _Matrix;

	Mat3f _DetMat;

	_DetMat[0][0] = Matrix[1][1]; _DetMat[0][1] = Matrix[2][1]; _DetMat[0][2] = Matrix[3][1];
	_DetMat[1][0] = Matrix[1][2]; _DetMat[1][1] = Matrix[2][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[1][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[0][0] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][1]; _DetMat[0][1] = Matrix[2][1]; _DetMat[0][2] = Matrix[3][1];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[2][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[0][1] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][1]; _DetMat[0][1] = Matrix[1][1]; _DetMat[0][2] = Matrix[3][1];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[0][2] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][1]; _DetMat[0][1] = Matrix[1][1]; _DetMat[0][2] = Matrix[2][1];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2]; _DetMat[1][2] = Matrix[2][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[2][3];

	_Matrix[0][3] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[1][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[1][2]; _DetMat[1][1] = Matrix[2][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[1][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[1][0] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[2][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[1][1] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2]; _DetMat[1][2] = Matrix[3][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[1][2] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[2][0];
	_DetMat[1][0] = Matrix[0][2]; _DetMat[1][1] = Matrix[1][2]; _DetMat[1][2] = Matrix[2][2];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[2][3];

	_Matrix[1][3] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[1][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[1][1]; _DetMat[1][1] = Matrix[2][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[1][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[2][0] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[2][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[2][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[2][1] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[1][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[3][3];

	_Matrix[2][2] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[2][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[1][1]; _DetMat[1][2] = Matrix[2][1];
	_DetMat[2][0] = Matrix[0][3]; _DetMat[2][1] = Matrix[1][3]; _DetMat[2][2] = Matrix[2][3];

	_Matrix[2][3] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[1][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[1][1]; _DetMat[1][1] = Matrix[2][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[1][2]; _DetMat[2][1] = Matrix[2][2]; _DetMat[2][2] = Matrix[3][2];

	_Matrix[3][0] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[2][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[2][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[0][2]; _DetMat[2][1] = Matrix[2][2]; _DetMat[2][2] = Matrix[3][2];

	_Matrix[3][1] = _DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[3][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[1][1]; _DetMat[1][2] = Matrix[3][1];
	_DetMat[2][0] = Matrix[0][2]; _DetMat[2][1] = Matrix[1][2]; _DetMat[2][2] = Matrix[3][2];

	_Matrix[3][2] = -_DetMat.Determinant();

	_DetMat[0][0] = Matrix[0][0]; _DetMat[0][1] = Matrix[1][0]; _DetMat[0][2] = Matrix[2][0];
	_DetMat[1][0] = Matrix[0][1]; _DetMat[1][1] = Matrix[1][1]; _DetMat[1][2] = Matrix[2][1];
	_DetMat[2][0] = Matrix[0][2]; _DetMat[2][1] = Matrix[1][2]; _DetMat[2][2] = Matrix[2][2];

	_Matrix[3][3] = _DetMat.Determinant();

	_Matrix /= Determinant();

	*this = _Matrix;

	return *this;
}

const Math::Mat4f Math::Mat4f::Inversed() const
{
	return Mat4f(*this).Inverse();
}

const Math::Mat4f& Math::Mat4f::operator+ () const
{
	return *this;
}

const Math::Mat4f Math::Mat4f::operator- () const
{
	Mat4f _Matrix;

	_Matrix[0][0] = -Matrix[0][0]; _Matrix[0][1] = -Matrix[0][1]; _Matrix[0][2] = -Matrix[0][2]; _Matrix[0][3] = -Matrix[0][3];
	_Matrix[1][0] = -Matrix[1][0]; _Matrix[1][1] = -Matrix[1][1]; _Matrix[1][2] = -Matrix[1][2]; _Matrix[1][3] = -Matrix[1][3];
	_Matrix[2][0] = -Matrix[2][0]; _Matrix[2][1] = -Matrix[2][1]; _Matrix[2][2] = -Matrix[2][2]; _Matrix[2][3] = -Matrix[2][3];
	_Matrix[3][0] = -Matrix[3][0]; _Matrix[3][1] = -Matrix[3][1]; _Matrix[3][2] = -Matrix[3][2]; _Matrix[3][3] = -Matrix[3][3];

	return _Matrix;
}

const Math::Vec4f Math::Mat4f::operator* (const Vec4f& _Vec) const
{
	return Vec4f
	(
		Matrix[0][0] * _Vec.x + Matrix[0][1] * _Vec.y + Matrix[0][2] * _Vec.z + +Matrix[0][3] * _Vec.w,
		Matrix[1][0] * _Vec.x + Matrix[1][1] * _Vec.y + Matrix[1][2] * _Vec.z + +Matrix[1][3] * _Vec.w,
		Matrix[2][0] * _Vec.x + Matrix[2][1] * _Vec.y + Matrix[2][2] * _Vec.z + +Matrix[2][3] * _Vec.w,
		Matrix[3][0] * _Vec.x + Matrix[3][1] * _Vec.y + Matrix[3][2] * _Vec.z + +Matrix[3][3] * _Vec.w
	);
}

const Math::Mat4f Math::Mat4f::operator+ (const Mat4f& _Other) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] += _Other[0][0]; _Matrix[0][1] += _Other[0][1]; _Matrix[0][2] += _Other[0][2]; _Matrix[0][3] += _Other[0][3];
	_Matrix[1][0] += _Other[1][0]; _Matrix[1][1] += _Other[1][1]; _Matrix[1][2] += _Other[1][2]; _Matrix[1][3] += _Other[1][3];
	_Matrix[2][0] += _Other[2][0]; _Matrix[2][1] += _Other[2][1]; _Matrix[2][2] += _Other[2][2]; _Matrix[2][3] += _Other[2][3];
	_Matrix[3][0] += _Other[3][0]; _Matrix[3][1] += _Other[3][1]; _Matrix[3][2] += _Other[3][2]; _Matrix[3][3] += _Other[3][3];

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator+= (const Mat4f& _Other)
{
	Matrix[0][0] += _Other[0][0]; Matrix[0][1] += _Other[0][1]; Matrix[0][2] += _Other[0][2]; Matrix[0][3] += _Other[0][3];
	Matrix[1][0] += _Other[1][0]; Matrix[1][1] += _Other[1][1]; Matrix[1][2] += _Other[1][2]; Matrix[1][3] += _Other[1][3];
	Matrix[2][0] += _Other[2][0]; Matrix[2][1] += _Other[2][1]; Matrix[2][2] += _Other[2][2]; Matrix[2][3] += _Other[2][3];
	Matrix[3][0] += _Other[3][0]; Matrix[3][1] += _Other[3][1]; Matrix[3][2] += _Other[3][2]; Matrix[3][3] += _Other[3][3];

	return *this;
}

const Math::Mat4f Math::Mat4f::operator+ (const float _Scale) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] += _Scale; _Matrix[0][1] += _Scale; _Matrix[0][2] += _Scale; _Matrix[0][3] += _Scale;
	_Matrix[1][0] += _Scale; _Matrix[1][1] += _Scale; _Matrix[1][2] += _Scale; _Matrix[1][3] += _Scale;
	_Matrix[2][0] += _Scale; _Matrix[2][1] += _Scale; _Matrix[2][2] += _Scale; _Matrix[2][3] += _Scale;
	_Matrix[3][0] += _Scale; _Matrix[3][1] += _Scale; _Matrix[3][2] += _Scale; _Matrix[3][3] += _Scale;

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator+= (const float _Scale)
{
	Matrix[0][0] += _Scale; Matrix[0][1] += _Scale; Matrix[0][2] += _Scale; Matrix[0][3] += _Scale;
	Matrix[1][0] += _Scale; Matrix[1][1] += _Scale; Matrix[1][2] += _Scale; Matrix[1][3] += _Scale;
	Matrix[2][0] += _Scale; Matrix[2][1] += _Scale; Matrix[2][2] += _Scale; Matrix[2][3] += _Scale;
	Matrix[3][0] += _Scale; Matrix[3][1] += _Scale; Matrix[3][2] += _Scale; Matrix[3][3] += _Scale;

	return *this;
}

const Math::Mat4f Math::Mat4f::operator- (const Mat4f& _Other) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] -= _Other[0][0]; _Matrix[0][1] -= _Other[0][1]; _Matrix[0][2] -= _Other[0][2]; _Matrix[0][3] -= _Other[0][3];
	_Matrix[1][0] -= _Other[1][0]; _Matrix[1][1] -= _Other[1][1]; _Matrix[1][2] -= _Other[1][2]; _Matrix[1][3] -= _Other[1][3];
	_Matrix[2][0] -= _Other[2][0]; _Matrix[2][1] -= _Other[2][1]; _Matrix[2][2] -= _Other[2][2]; _Matrix[2][3] -= _Other[2][3];
	_Matrix[3][0] -= _Other[3][0]; _Matrix[3][1] -= _Other[3][1]; _Matrix[3][2] -= _Other[3][2]; _Matrix[3][3] -= _Other[3][3];

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator-= (const Mat4f& _Other)
{
	Matrix[0][0] -= _Other[0][0]; Matrix[0][1] -= _Other[0][1]; Matrix[0][2] -= _Other[0][2]; Matrix[0][3] -= _Other[0][3];
	Matrix[1][0] -= _Other[1][0]; Matrix[1][1] -= _Other[1][1]; Matrix[1][2] -= _Other[1][2]; Matrix[1][3] -= _Other[1][3];
	Matrix[2][0] -= _Other[2][0]; Matrix[2][1] -= _Other[2][1]; Matrix[2][2] -= _Other[2][2]; Matrix[2][3] -= _Other[2][3];
	Matrix[3][0] -= _Other[3][0]; Matrix[3][1] -= _Other[3][1]; Matrix[3][2] -= _Other[3][2]; Matrix[3][3] -= _Other[3][3];

	return *this;
}

const Math::Mat4f Math::Mat4f::operator- (const float _Scale) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] -= _Scale; _Matrix[0][1] -= _Scale; _Matrix[0][2] -= _Scale; _Matrix[0][3] -= _Scale;
	_Matrix[1][0] -= _Scale; _Matrix[1][1] -= _Scale; _Matrix[1][2] -= _Scale; _Matrix[1][3] -= _Scale;
	_Matrix[2][0] -= _Scale; _Matrix[2][1] -= _Scale; _Matrix[2][2] -= _Scale; _Matrix[2][3] -= _Scale;
	_Matrix[3][0] -= _Scale; _Matrix[3][1] -= _Scale; _Matrix[3][2] -= _Scale; _Matrix[3][3] -= _Scale;

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator-= (const float _Scale)
{
	Matrix[0][0] -= _Scale; Matrix[0][1] -= _Scale; Matrix[0][2] -= _Scale; Matrix[0][3] -= _Scale;
	Matrix[1][0] -= _Scale; Matrix[1][1] -= _Scale; Matrix[1][2] -= _Scale; Matrix[1][3] -= _Scale;
	Matrix[2][0] -= _Scale; Matrix[2][1] -= _Scale; Matrix[2][2] -= _Scale; Matrix[2][3] -= _Scale;
	Matrix[3][0] -= _Scale; Matrix[3][1] -= _Scale; Matrix[3][2] -= _Scale; Matrix[3][3] -= _Scale;

	return *this;
}

const Math::Mat4f Math::Mat4f::operator* (const Mat4f& _Other) const
{
	Mat4f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0] + Matrix[0][2] * _Other[2][0] + Matrix[0][3] * _Other[3][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1] + Matrix[0][2] * _Other[2][1] + Matrix[0][3] * _Other[3][1];
	_Matrix[0][2] = Matrix[0][0] * _Other[0][2] + Matrix[0][1] * _Other[1][2] + Matrix[0][2] * _Other[2][2] + Matrix[0][3] * _Other[3][2];
	_Matrix[0][3] = Matrix[0][0] * _Other[0][3] + Matrix[0][1] * _Other[1][3] + Matrix[0][2] * _Other[2][3] + Matrix[0][3] * _Other[3][3];

	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0] + Matrix[1][2] * _Other[2][0] + Matrix[1][3] * _Other[3][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1] + Matrix[1][2] * _Other[2][1] + Matrix[1][3] * _Other[3][1];
	_Matrix[1][2] = Matrix[1][0] * _Other[0][2] + Matrix[1][1] * _Other[1][2] + Matrix[1][2] * _Other[2][2] + Matrix[1][3] * _Other[3][2];
	_Matrix[1][3] = Matrix[1][0] * _Other[0][3] + Matrix[1][1] * _Other[1][3] + Matrix[1][2] * _Other[2][3] + Matrix[1][3] * _Other[3][3];

	_Matrix[2][0] = Matrix[2][0] * _Other[0][0] + Matrix[2][1] * _Other[1][0] + Matrix[2][2] * _Other[2][0] + Matrix[2][3] * _Other[3][0];
	_Matrix[2][1] = Matrix[2][0] * _Other[0][1] + Matrix[2][1] * _Other[1][1] + Matrix[2][2] * _Other[2][1] + Matrix[2][3] * _Other[3][1];
	_Matrix[2][2] = Matrix[2][0] * _Other[0][2] + Matrix[2][1] * _Other[1][2] + Matrix[2][2] * _Other[2][2] + Matrix[2][3] * _Other[3][2];
	_Matrix[2][3] = Matrix[2][0] * _Other[0][3] + Matrix[2][1] * _Other[1][3] + Matrix[2][2] * _Other[2][3] + Matrix[2][3] * _Other[3][3];

	_Matrix[3][0] = Matrix[3][0] * _Other[0][0] + Matrix[3][1] * _Other[1][0] + Matrix[3][2] * _Other[2][0] + Matrix[3][3] * _Other[3][0];
	_Matrix[3][1] = Matrix[3][0] * _Other[0][1] + Matrix[3][1] * _Other[1][1] + Matrix[3][2] * _Other[2][1] + Matrix[3][3] * _Other[3][1];
	_Matrix[3][2] = Matrix[3][0] * _Other[0][2] + Matrix[3][1] * _Other[1][2] + Matrix[3][2] * _Other[2][2] + Matrix[3][3] * _Other[3][2];
	_Matrix[3][3] = Matrix[3][0] * _Other[0][3] + Matrix[3][1] * _Other[1][3] + Matrix[3][2] * _Other[2][3] + Matrix[3][3] * _Other[3][3];

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator*= (const Mat4f& _Other)
{
	Mat4f _Matrix;

	_Matrix[0][0] = Matrix[0][0] * _Other[0][0] + Matrix[0][1] * _Other[1][0] + Matrix[0][2] * _Other[2][0] + Matrix[0][3] * _Other[3][0];
	_Matrix[0][1] = Matrix[0][0] * _Other[0][1] + Matrix[0][1] * _Other[1][1] + Matrix[0][2] * _Other[2][1] + Matrix[0][3] * _Other[3][1];
	_Matrix[0][2] = Matrix[0][0] * _Other[0][2] + Matrix[0][1] * _Other[1][2] + Matrix[0][2] * _Other[2][2] + Matrix[0][3] * _Other[3][2];
	_Matrix[0][3] = Matrix[0][0] * _Other[0][3] + Matrix[0][1] * _Other[1][3] + Matrix[0][2] * _Other[2][3] + Matrix[0][3] * _Other[3][3];

	_Matrix[1][0] = Matrix[1][0] * _Other[0][0] + Matrix[1][1] * _Other[1][0] + Matrix[1][2] * _Other[2][0] + Matrix[1][3] * _Other[3][0];
	_Matrix[1][1] = Matrix[1][0] * _Other[0][1] + Matrix[1][1] * _Other[1][1] + Matrix[1][2] * _Other[2][1] + Matrix[1][3] * _Other[3][1];
	_Matrix[1][2] = Matrix[1][0] * _Other[0][2] + Matrix[1][1] * _Other[1][2] + Matrix[1][2] * _Other[2][2] + Matrix[1][3] * _Other[3][2];
	_Matrix[1][3] = Matrix[1][0] * _Other[0][3] + Matrix[1][1] * _Other[1][3] + Matrix[1][2] * _Other[2][3] + Matrix[1][3] * _Other[3][3];

	_Matrix[2][0] = Matrix[2][0] * _Other[0][0] + Matrix[2][1] * _Other[1][0] + Matrix[2][2] * _Other[2][0] + Matrix[2][3] * _Other[3][0];
	_Matrix[2][1] = Matrix[2][0] * _Other[0][1] + Matrix[2][1] * _Other[1][1] + Matrix[2][2] * _Other[2][1] + Matrix[2][3] * _Other[3][1];
	_Matrix[2][2] = Matrix[2][0] * _Other[0][2] + Matrix[2][1] * _Other[1][2] + Matrix[2][2] * _Other[2][2] + Matrix[2][3] * _Other[3][2];
	_Matrix[2][3] = Matrix[2][0] * _Other[0][3] + Matrix[2][1] * _Other[1][3] + Matrix[2][2] * _Other[2][3] + Matrix[2][3] * _Other[3][3];

	_Matrix[3][0] = Matrix[3][0] * _Other[0][0] + Matrix[3][1] * _Other[1][0] + Matrix[3][2] * _Other[2][0] + Matrix[3][3] * _Other[3][0];
	_Matrix[3][1] = Matrix[3][0] * _Other[0][1] + Matrix[3][1] * _Other[1][1] + Matrix[3][2] * _Other[2][1] + Matrix[3][3] * _Other[3][1];
	_Matrix[3][2] = Matrix[3][0] * _Other[0][2] + Matrix[3][1] * _Other[1][2] + Matrix[3][2] * _Other[2][2] + Matrix[3][3] * _Other[3][2];
	_Matrix[3][3] = Matrix[3][0] * _Other[0][3] + Matrix[3][1] * _Other[1][3] + Matrix[3][2] * _Other[2][3] + Matrix[3][3] * _Other[3][3];

	*this = _Matrix;

	return *this;
}

const Math::Mat4f Math::Mat4f::operator* (const float _Scale) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] *= _Scale; _Matrix[0][1] *= _Scale; _Matrix[0][2] *= _Scale; _Matrix[0][3] *= _Scale;
	_Matrix[1][0] *= _Scale; _Matrix[1][1] *= _Scale; _Matrix[1][2] *= _Scale; _Matrix[1][3] *= _Scale;
	_Matrix[2][0] *= _Scale; _Matrix[2][1] *= _Scale; _Matrix[2][2] *= _Scale; _Matrix[2][3] *= _Scale;
	_Matrix[3][0] *= _Scale; _Matrix[3][1] *= _Scale; _Matrix[3][2] *= _Scale; _Matrix[3][3] *= _Scale;

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator*= (const float _Scale)
{
	Matrix[0][0] *= _Scale; Matrix[0][1] *= _Scale; Matrix[0][2] *= _Scale; Matrix[0][3] *= _Scale;
	Matrix[1][0] *= _Scale; Matrix[1][1] *= _Scale; Matrix[1][2] *= _Scale; Matrix[1][3] *= _Scale;
	Matrix[2][0] *= _Scale; Matrix[2][1] *= _Scale; Matrix[2][2] *= _Scale; Matrix[2][3] *= _Scale;
	Matrix[3][0] *= _Scale; Matrix[3][1] *= _Scale; Matrix[3][2] *= _Scale; Matrix[3][3] *= _Scale;

	return *this;
}

const Math::Mat4f Math::Mat4f::operator/ (const float _Scale) const
{
	Mat4f _Matrix(*this);

	_Matrix[0][0] /= _Scale; _Matrix[0][1] /= _Scale; _Matrix[0][2] /= _Scale; _Matrix[0][3] /= _Scale;
	_Matrix[1][0] /= _Scale; _Matrix[1][1] /= _Scale; _Matrix[1][2] /= _Scale; _Matrix[1][3] /= _Scale;
	_Matrix[2][0] /= _Scale; _Matrix[2][1] /= _Scale; _Matrix[2][2] /= _Scale; _Matrix[2][3] /= _Scale;
	_Matrix[3][0] /= _Scale; _Matrix[3][1] /= _Scale; _Matrix[3][2] /= _Scale; _Matrix[3][3] /= _Scale;

	return _Matrix;
}

const Math::Mat4f& Math::Mat4f::operator/= (const float _Scale)
{
	Matrix[0][0] /= _Scale; Matrix[0][1] /= _Scale; Matrix[0][2] /= _Scale; Matrix[0][3] /= _Scale;
	Matrix[1][0] /= _Scale; Matrix[1][1] /= _Scale; Matrix[1][2] /= _Scale; Matrix[1][3] /= _Scale;
	Matrix[2][0] /= _Scale; Matrix[2][1] /= _Scale; Matrix[2][2] /= _Scale; Matrix[2][3] /= _Scale;
	Matrix[3][0] /= _Scale; Matrix[3][1] /= _Scale; Matrix[3][2] /= _Scale; Matrix[3][3] /= _Scale;

	return *this;
}

const bool Math::Mat4f::operator== (const Mat4f& _Other) const
{
	return
		Matrix[0][0] == _Other[0][0] && Matrix[0][1] == _Other[0][1] && Matrix[0][2] == _Other[0][2] && Matrix[0][3] == _Other[0][3] &&
		Matrix[1][0] == _Other[1][0] && Matrix[1][1] == _Other[1][1] && Matrix[1][2] == _Other[1][2] && Matrix[1][3] == _Other[1][3] &&
		Matrix[2][0] == _Other[2][0] && Matrix[2][1] == _Other[2][1] && Matrix[2][2] == _Other[2][2] && Matrix[2][3] == _Other[2][3] &&
		Matrix[3][0] == _Other[3][0] && Matrix[3][1] == _Other[3][1] && Matrix[3][2] == _Other[3][2] && Matrix[3][3] == _Other[3][3];
}

const bool Math::Mat4f::operator!= (const Mat4f& _Other) const
{
	return
		Matrix[0][0] != _Other[0][0] || Matrix[0][1] != _Other[0][1] || Matrix[0][2] != _Other[0][2] || Matrix[0][3] != _Other[0][3] ||
		Matrix[1][0] != _Other[1][0] || Matrix[1][1] != _Other[1][1] || Matrix[1][2] != _Other[1][2] || Matrix[1][3] != _Other[1][3] ||
		Matrix[2][0] != _Other[2][0] || Matrix[2][1] != _Other[2][1] || Matrix[2][2] != _Other[2][2] || Matrix[2][3] != _Other[2][3] ||
		Matrix[3][0] != _Other[3][0] || Matrix[3][1] != _Other[3][1] || Matrix[3][2] != _Other[3][2] || Matrix[3][3] != _Other[3][3];
}

float* Math::Mat4f::operator[] (const size_t _Index)
{
	return Matrix[_Index].Data();
}

const float* Math::Mat4f::operator[] (const size_t _Index) const
{
	return Matrix[_Index].Data();
}

const Math::Mat4f& Math::Mat4f::operator= (const Mat4f& _Other)
{
	Matrix[0][0] = _Other[0][0]; Matrix[0][1] = _Other[0][1]; Matrix[0][2] = _Other[0][2]; Matrix[0][3] = _Other[0][3];
	Matrix[1][0] = _Other[1][0]; Matrix[1][1] = _Other[1][1]; Matrix[1][2] = _Other[1][2]; Matrix[1][3] = _Other[1][3];
	Matrix[2][0] = _Other[2][0]; Matrix[2][1] = _Other[2][1]; Matrix[2][2] = _Other[2][2]; Matrix[2][3] = _Other[2][3];
	Matrix[3][0] = _Other[3][0]; Matrix[3][1] = _Other[3][1]; Matrix[3][2] = _Other[3][2]; Matrix[3][3] = _Other[3][3];

	return *this;
}

const Math::Mat4f Math::Mat4f::GetScale(const float _x, const float _y, const float _z, const float _w)
{
	Mat4f _Matrix;

	_Matrix[0][0] = _x;
	_Matrix[1][1] = _y;
	_Matrix[2][2] = _z;
	_Matrix[3][3] = _w;

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetFill(const float _Value)
{
	Mat4f _Matrix;

	_Matrix[0][0] = _Value; _Matrix[0][1] = _Value; _Matrix[0][2] = _Value; _Matrix[0][3] = _Value;
	_Matrix[1][0] = _Value; _Matrix[1][1] = _Value; _Matrix[1][2] = _Value; _Matrix[1][3] = _Value;
	_Matrix[2][0] = _Value; _Matrix[2][1] = _Value; _Matrix[2][2] = _Value; _Matrix[2][3] = _Value;
	_Matrix[3][0] = _Value; _Matrix[3][1] = _Value; _Matrix[3][2] = _Value; _Matrix[3][3] = _Value;

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetShear(const float _Value, const size_t _ShearedAxis, const size_t _ShearByAxis)
{
	Mat4f _Matrix;

	_Matrix[_ShearedAxis][_ShearByAxis] = _Value;

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetRotation(const float _Angle, const Vec4f& _RotationAxis)
{
	Mat4f _Matrix;

	float _Sin = sinf(_Angle);
	float _Cos = cosf(_Angle);

	_Matrix[0][0] = _Cos + _RotationAxis.x * _RotationAxis.x * (1.0f - _Cos);
	_Matrix[0][1] = _RotationAxis.x * _RotationAxis.y * (1.0f - _Cos) - _RotationAxis.z * _Sin;
	_Matrix[0][2] = _RotationAxis.x * _RotationAxis.z * (1.0f - _Cos) + _RotationAxis.y * _Sin;

	_Matrix[1][0] = _RotationAxis.y * _RotationAxis.x * (1.0f - _Cos) + _RotationAxis.z * _Sin;
	_Matrix[1][1] = _Cos + _RotationAxis.y * _RotationAxis.y * (1.0f - _Cos);
	_Matrix[1][2] = _RotationAxis.y * _RotationAxis.z * (1.0f - _Cos) - _RotationAxis.x * _Sin;

	_Matrix[2][0] = _RotationAxis.z * _RotationAxis.x * (1.0f - _Cos) - _RotationAxis.y * _Sin;
	_Matrix[2][1] = _RotationAxis.z * _RotationAxis.y * (1.0f - _Cos) + _RotationAxis.x * _Sin;
	_Matrix[2][2] = _Cos + _RotationAxis.z * _RotationAxis.z * (1.0f - _Cos);

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetTranslation(const Vec3f& _Coords)
{
	Mat4f _Matrix;

	_Matrix[0][3] = _Coords.x;
	_Matrix[1][3] = _Coords.y;
	_Matrix[2][3] = _Coords.z;

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetOrtho(const float _Left, const float _Right, const float _Bottom, const float _Top, const float _Front, const float _Back)
{
	Mat4f _Matrix;

	_Matrix[0][0] = 2.0f / (_Right - _Left);
	_Matrix[1][1] = 2.0f / (_Top - _Bottom);
	_Matrix[2][2] = 2.0f / (_Back - _Front);

	_Matrix[0][3] = (_Right + _Left) / (_Left - _Right);
	_Matrix[1][3] = (_Top + _Bottom) / (_Bottom - _Top);
	_Matrix[2][3] = (_Back + _Front) / (_Front - _Back);

	return _Matrix;
}

const Math::Mat4f Math::Mat4f::GetPerspective(const float _Fov, const float _AspectRatio, const float _ZNear, const float _ZFar)
{
	Mat4f _Matrix;

	float _Tan = tanf(_Fov / 2);

	_Matrix[0][0] = 1.0f / (_AspectRatio * _Tan);
	_Matrix[1][1] = 1.0f / _Tan;
	_Matrix[2][2] = -(_ZFar + _ZNear) / (_ZFar - _ZNear);
	_Matrix[2][3] = -(2.0f * _ZFar * _ZNear) / (_ZFar - _ZNear);
	_Matrix[3][2] = -1.0f;
	_Matrix[3][3] = 0.0f;

	return _Matrix;
}
