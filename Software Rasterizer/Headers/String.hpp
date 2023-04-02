#ifndef String_hpp

#define String_hpp



#include "Main.hpp"



namespace String
{

	const size_t Length(const wchar_t* _Str);
	const bool TheSame(const wchar_t* _Str1, const wchar_t* _Str2);
	const bool Contains(const wchar_t* _Str, const wchar_t _Chr);
	const wchar_t* TokenizeWhiteSpace(wchar_t* _Str);

}



#endif
