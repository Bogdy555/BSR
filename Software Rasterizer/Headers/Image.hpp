#ifndef Image_hpp

#define Image_hpp



#include "Main.hpp"



namespace Image
{

	struct ImageSRGBA
	{
		unsigned char* Data;
		size_t Width;
		size_t Height;
	};

	struct ImageHDR
	{
		float* Data;
		size_t Width;
		size_t Height;
	};

	float* HDRReadFile(const wchar_t* _Path, size_t& _Width, size_t& _Height);

	bool SaveBmp(const wchar_t* _Path, const unsigned char* _Img, const size_t _Width, const size_t _Height);

	unsigned char* LoadBmp(const wchar_t* _Path, size_t& _Width, size_t& _Height);

}



#endif
