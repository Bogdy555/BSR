#include "../Headers/BSR.hpp"



const float BSR::Math::Pi = 3.14159265f;
const float BSR::Math::DegreesToRadians = 3.14159265f / 180.0f;
const float BSR::Math::RadiansToDegrees = 180.0f / 3.14159265f;



void BSR::Math::Swap(float& _A, float& _B)
{
	float _Aux = _A;
	_A = _B;
	_B = _Aux;
}

const float BSR::Math::Clamp(const float _Value, const float _Min, const float _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const float BSR::Math::Mix(const float _A, const float _B, const float _Percentage)
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
