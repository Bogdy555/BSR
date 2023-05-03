#ifndef BSR_String_hpp

#define BSR_String_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace String
	{

		const size_t Length(const wchar_t* _Str);
		const bool TheSame(const wchar_t* _Str1, const wchar_t* _Str2);
		const bool Contains(const wchar_t* _Str, const wchar_t _Chr);
		const wchar_t* TokenizeWhiteSpace(wchar_t* _Str);
		const wchar_t* TokenizeSlashes(wchar_t* _Str);
		const size_t Count(const wchar_t* _Str, const wchar_t _Chr);

	}

}



#endif
