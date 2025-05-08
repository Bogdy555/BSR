#include "../Headers/BSR.hpp"



const float BSR::Math::Pi = 3.14159265f;
const float BSR::Math::DegreesToRadians = 3.14159265f / 180.0f;
const float BSR::Math::RadiansToDegrees = 180.0f / 3.14159265f;



void BSR::Math::Swap(float& _V1, float& _V2)
{
	float _Aux = _V1;
	_V1 = _V2;
	_V2 = _Aux;
}

const float BSR::Math::Clamp(const float _Value, const float _Min, const float _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const float BSR::Math::Mix(const float _V1, const float _V2, const float _Percentage)
{
	return _V1 + (_V2 - _V1) * _Percentage;
}

const float BSR::Math::Min(const float _V1, const float _V2)
{
	return _V1 * (_V1 <= _V2) + _V2 * (_V1 > _V2);
}

const float BSR::Math::Max(const float _V1, const float _V2)
{
	return _V1 * (_V1 >= _V2) + _V2 * (_V1 < _V2);
}
