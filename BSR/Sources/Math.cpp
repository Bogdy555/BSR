#include "../Headers/BSR.hpp"



const float BSR::Math::fPi = 3.14159265f;
const float BSR::Math::fDegreesToRadians = 3.14159265f / 180.0f;
const float BSR::Math::fRadiansToDegrees = 180.0f / 3.14159265f;

const double BSR::Math::dPi = 3.14159265;
const double BSR::Math::dDegreesToRadians = 3.14159265 / 180.0;
const double BSR::Math::dRadiansToDegrees = 180.0 / 3.14159265;



void BSR::Math::Swap(float& _A, float& _B)
{
	float _Aux = _A;
	_A = _B;
	_B = _Aux;
}

void BSR::Math::Swap(double& _A, double& _B)
{
	double _Aux = _A;
	_A = _B;
	_B = _Aux;
}

const float BSR::Math::Clamp(const float _Value, const float _Min, const float _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const double BSR::Math::Clamp(const double _Value, const double _Min, const double _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const float BSR::Math::Mix(const float _A, const float _B, const float _Percentage)
{
	return _A + (_B - _A) * _Percentage;
}

const double BSR::Math::Mix(const double _A, const double _B, const double _Percentage)
{
	return _A + (_B - _A) * _Percentage;
}

const float BSR::Math::Min(const float _A, const float _B)
{
	return _A * (_A <= _B) + _B * (_A > _B);
}

const float BSR::Math::Max(const float _A, const float _B)
{
	return _A * (_A >= _B) + _B * (_A < _B);
}
