#include "..\Headers\Main.hpp"



const float Math::fPi = 3.14159265f;
const float Math::fDegreesToRadians = 3.14159265f / 180.0f;
const float Math::fRadiansToDegrees = 180.0f / 3.14159265f;

const double Math::dPi = 3.14159265;
const double Math::dDegreesToRadians = 3.14159265 / 180.0;
const double Math::dRadiansToDegrees = 180.0 / 3.14159265;



void Math::Swap(float& _A, float& _B)
{
	float _Aux = _A;
	_A = _B;
	_B = _Aux;
}

void Math::Swap(double& _A, double& _B)
{
	double _Aux = _A;
	_A = _B;
	_B = _Aux;
}

const float Math::Clamp(const float _Value, const float _Min, const float _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const double Math::Clamp(const double _Value, const double _Min, const double _Max)
{
	return _Value * (_Min <= _Value && _Value <= _Max) + _Min * (_Value < _Min) + _Max * (_Value > _Max);
}

const float Math::Mix(const float _A, const float _B, const float _Percentage)
{
	return _A + (_B - _A) * _Percentage;
}

const double Math::Mix(const double _A, const double _B, const double _Percentage)
{
	return _A + (_B - _A) * _Percentage;
}

const float Math::Period(const float _Value, const float _Min, const float _Max)
{
	float _Rez = _Value;

	while (_Rez >= _Max)
	{
		_Rez -= _Max - _Min;
	}

	while (_Rez < _Min)
	{
		_Rez += _Max - _Min;
	}

	return _Rez;
}

const double Math::Period(const double _Value, const double _Min, const double _Max)
{
	double _Rez = _Value;

	while (_Rez >= _Max)
	{
		_Rez -= _Max - _Min;
	}

	while (_Rez < _Min)
	{
		_Rez += _Max - _Min;
	}

	return _Rez;
}
