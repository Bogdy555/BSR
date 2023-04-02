#ifndef Image_hpp

#define Image_hpp



#include "Main.hpp"



namespace Image
{

	struct ImageSRGBA
	{
		unsigned char* Data = nullptr;
		size_t Width = 0;
		size_t Height = 0;
	};

	struct ImageHDR
	{
		float* Data = nullptr;
		size_t Width = 0;
		size_t Height = 0;
	};

	float* HDRReadFile(const wchar_t* _Path, size_t& _Width, size_t& _Height);

	bool SaveBmp(const wchar_t* _Path, const unsigned char* _Img, const size_t _Width, const size_t _Height);

	unsigned char* LoadBmp(const wchar_t* _Path, size_t& _Width, size_t& _Height);

}



#endif
