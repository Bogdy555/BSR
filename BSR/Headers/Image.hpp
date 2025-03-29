#ifndef BSR_Image_hpp

#define BSR_Image_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace Image
	{

		struct Image
		{
			uint8_t* Data = nullptr;
			size_t Width = 0;
			size_t Height = 0;
		};

		struct ImageFloat
		{
			float* Data = nullptr;
			size_t Width = 0;
			size_t Height = 0;
		};

		float* LoadHdr(const wchar_t* _Path, size_t& _Width, size_t& _Height);

		bool SaveBmp(const wchar_t* _Path, const uint8_t* _Img, const size_t _Width, const size_t _Height);

		uint8_t* LoadBmp(const wchar_t* _Path, size_t& _Width, size_t& _Height);

	}

}



#endif
