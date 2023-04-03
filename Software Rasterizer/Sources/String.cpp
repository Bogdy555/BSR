#include "..\Headers\Main.hpp"



const size_t String::Length(const wchar_t* _Str)
{
	if (!_Str)
	{
		return 0;
	}

	size_t _Index = 0;

	while (_Str[_Index] != '\0')
	{
		_Index++;
	}

	return _Index;
}

const bool String::TheSame(const wchar_t* _Str1, const wchar_t* _Str2)
{
	if (_Str1 == _Str2)
	{
		return true;
	}

	if (!_Str1 || !_Str2)
	{
		return false;
	}

	size_t _Len1 = Length(_Str1);
	size_t _Len2 = Length(_Str2);

	if (_Len1 != _Len2)
	{
		return false;
	}

	for (size_t _Index = 0; _Index < _Len1; _Index++)
	{
		if (_Str1[_Index] != _Str2[_Index])
		{
			return false;
		}
	}

	return true;
}

const bool String::Contains(const wchar_t* _Str, const wchar_t _Chr)
{
	if (!_Str)
	{
		return false;
	}

	size_t _Len = Length(_Str);

	for (size_t _Index = 0; _Index < _Len; _Index++)
	{
		if (_Str[_Index] == _Chr)
		{
			return true;
		}
	}

	return false;
}

const wchar_t* String::TokenizeWhiteSpace(wchar_t* _Str)
{
	static thread_local wchar_t* _CurrentTokenStr = nullptr;
	static thread_local size_t _CurrentTokenStrLen = 0;
	static thread_local size_t _CurrentIndex = 0;

	if (_Str)
	{
		_CurrentTokenStr = _Str;
		_CurrentTokenStrLen = Length(_CurrentTokenStr);
		_CurrentIndex = 0;

		for (size_t _Index = 0; _Index < _CurrentTokenStrLen; _Index++)
		{
			if (Contains(L" \t\n", _CurrentTokenStr[_Index]))
			{
				_CurrentTokenStr[_Index] = '\0';
			}
		}

		while (_CurrentTokenStr[_CurrentIndex] == '\0')
		{
			_CurrentIndex++;

			if (_CurrentIndex >= _CurrentTokenStrLen)
			{
				_CurrentTokenStr = nullptr;
				_CurrentTokenStrLen = 0;
				_CurrentIndex = 0;

				return nullptr;
			}
		}

		return _CurrentTokenStr + _CurrentIndex;
	}

	if (!_CurrentTokenStr)
	{
		return nullptr;
	}

	while (_CurrentTokenStr[_CurrentIndex] != '\0')
	{
		_CurrentIndex++;

		if (_CurrentIndex >= _CurrentTokenStrLen)
		{
			_CurrentTokenStr = nullptr;
			_CurrentTokenStrLen = 0;
			_CurrentIndex = 0;

			return nullptr;
		}
	}

	while (_CurrentTokenStr[_CurrentIndex] == '\0')
	{
		_CurrentIndex++;

		if (_CurrentIndex >= _CurrentTokenStrLen)
		{
			_CurrentTokenStr = nullptr;
			_CurrentTokenStrLen = 0;
			_CurrentIndex = 0;

			return nullptr;
		}
	}

	return _CurrentTokenStr + _CurrentIndex;
}

const wchar_t* String::TokenizeSlashes(wchar_t* _Str)
{
	static thread_local wchar_t* _CurrentTokenStr = nullptr;
	static thread_local size_t _CurrentTokenStrLen = 0;
	static thread_local size_t _CurrentIndex = 0;

	if (_Str)
	{
		_CurrentTokenStr = _Str;
		_CurrentTokenStrLen = Length(_CurrentTokenStr);
		_CurrentIndex = 0;

		for (size_t _Index = 0; _Index < _CurrentTokenStrLen; _Index++)
		{
			if (_CurrentTokenStr[_Index] == L'/')
			{
				_CurrentTokenStr[_Index] = '\0';
			}
		}

		while (_CurrentTokenStr[_CurrentIndex] == '\0')
		{
			_CurrentIndex++;

			if (_CurrentIndex >= _CurrentTokenStrLen)
			{
				_CurrentTokenStr = nullptr;
				_CurrentTokenStrLen = 0;
				_CurrentIndex = 0;

				return nullptr;
			}
		}

		return _CurrentTokenStr + _CurrentIndex;
	}

	if (!_CurrentTokenStr)
	{
		return nullptr;
	}

	while (_CurrentTokenStr[_CurrentIndex] != '\0')
	{
		_CurrentIndex++;

		if (_CurrentIndex >= _CurrentTokenStrLen)
		{
			_CurrentTokenStr = nullptr;
			_CurrentTokenStrLen = 0;
			_CurrentIndex = 0;

			return nullptr;
		}
	}

	while (_CurrentTokenStr[_CurrentIndex] == '\0')
	{
		_CurrentIndex++;

		if (_CurrentIndex >= _CurrentTokenStrLen)
		{
			_CurrentTokenStr = nullptr;
			_CurrentTokenStrLen = 0;
			_CurrentIndex = 0;

			return nullptr;
		}
	}

	return _CurrentTokenStr + _CurrentIndex;
}

const size_t String::Count(const wchar_t* _Str, const wchar_t _Chr)
{
	if (!_Str)
	{
		return 0;
	}

	size_t _Len = Length(_Str);

	size_t _Count = 0;

	for (size_t _Index = 0; _Index < _Len; _Index++)
	{
		if (_Str[_Index] == _Chr)
		{
			_Count++;
		}
	}

	return _Count;
}
