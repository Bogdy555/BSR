#ifndef BSR_Image_hpp

#define BSR_Image_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace Image
	{

		struct SDR
		{

			size_t Width = 0;
			size_t Height = 0;
			uint8_t* Data = nullptr;

		};

		struct HDR
		{

			size_t Width = 0;
			size_t Height = 0;
			float* Data = nullptr;

		};

		uint8_t* LoadSdr(const char* _FileData, const size_t _FileSize, size_t& _Width, size_t& _Height);
		float* LoadHdr(const char* _FileData, const size_t _FileSize, size_t& _Width, size_t& _Height);

		char* SaveSdr(size_t& _FileSize, const SDR& _Image);
		char* SaveHdr(size_t& _FileSize, const HDR& _Image);

	}

}



#endif
