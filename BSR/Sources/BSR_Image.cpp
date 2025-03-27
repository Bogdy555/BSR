#include "..\Headers\BSR.hpp"



uint8_t* BSR::Image::LoadSdr(const char* _FileData, const size_t _FileSize, size_t& _Width, size_t& _Height)
{
	_Width = 0;
	_Height = 0;

	if (!_FileData || !_FileSize)
	{
		return nullptr;
	}

	if (*(char*)(_FileData + 0) != 'B')
	{
		return nullptr;
	}

	if (*(char*)(_FileData + 1) != 'M')
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 2) != _FileSize)
	{
		return nullptr;
	}

	if (*(uint16_t*)(_FileData + 6) != 0)
	{
		return nullptr;
	}

	if (*(uint16_t*)(_FileData + 8) != 0)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 10) != 14 + 40)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 14) != 40)
	{
		return nullptr;
	}

	if ((*(uint32_t*)(_FileData + 18)) * (*(uint32_t*)(_FileData + 22)) * 4 + 14 + 40 != _FileSize)
	{
		return nullptr;
	}

	if (*(uint16_t*)(_FileData + 26) != 1)
	{
		return nullptr;
	}

	if (*(uint16_t*)(_FileData + 28) != 32)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 30) != 0)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 34) != 0 && *(uint32_t*)(_FileData + 34) != _FileSize - 14 - 40)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 46) != 0)
	{
		return nullptr;
	}

	if (*(uint32_t*)(_FileData + 50) != 0)
	{
		return nullptr;
	}

	uint8_t* _Data = new uint8_t[(*(uint32_t*)(_FileData + 18)) * (*(uint32_t*)(_FileData + 22)) * 4];

	if (!_Data)
	{
		return nullptr;
	}

	_Width = *(uint32_t*)(_FileData + 18);
	_Height = *(uint32_t*)(_FileData + 22);

	for (size_t _Y = 0; _Y < _Height; _Y++)
	{
		for (size_t _X = 0; _X < _Width; _X++)
		{
			_Data[(_X + _Y * _Width) * 4 + 0] = (_FileData + 14 + 40)[(_X + _Y * _Width) * 4 + 2];
			_Data[(_X + _Y * _Width) * 4 + 1] = (_FileData + 14 + 40)[(_X + _Y * _Width) * 4 + 1];
			_Data[(_X + _Y * _Width) * 4 + 2] = (_FileData + 14 + 40)[(_X + _Y * _Width) * 4 + 0];
			_Data[(_X + _Y * _Width) * 4 + 3] = 255;
		}
	}

	return _Data;
}

//float* BSR::Image::LoadHdr(const char* _FileData, const size_t _FileSize, size_t& _Width, size_t& _Height)
//{
//	_Width = 0;
//	_Height = 0;
//
//	if (!_FileData || !_FileSize)
//	{
//		return nullptr;
//	}
//
//	return nullptr;
//}

char* BSR::Image::SaveSdr(size_t& _FileSize, const SDR& _Image)
{
	_FileSize = 0;

	if (!_Image.Width || !_Image.Height || !_Image.Data)
	{
		return nullptr;
	}

	char* _Result = new char[14 + 40 + _Image.Width * _Image.Height * 4];

	if (!_Result)
	{
		return nullptr;
	}

	_FileSize = 14 + 40 + _Image.Width * _Image.Height * 4;

	*(char*)(_Result + 0) = 'B';
	*(char*)(_Result + 1) = 'M';
	*(uint32_t*)(_Result + 2) = 14 + 40 + (uint32_t)(_Image.Width) * (uint32_t)(_Image.Height) * 4;
	*(uint16_t*)(_Result + 6) = 0;
	*(uint16_t*)(_Result + 8) = 0;
	*(uint32_t*)(_Result + 10) = 14 + 40;

	*(uint32_t*)(_Result + 14) = 40;
	*(uint32_t*)(_Result + 18) = (uint32_t)(_Image.Width);
	*(uint32_t*)(_Result + 22) = (uint32_t)(_Image.Height);
	*(uint16_t*)(_Result + 26) = 1;
	*(uint16_t*)(_Result + 28) = 32;
	*(uint32_t*)(_Result + 30) = 0;
	*(uint32_t*)(_Result + 34) = 0;
	*(uint32_t*)(_Result + 38) = 7200;
	*(uint32_t*)(_Result + 42) = 7200;
	*(uint32_t*)(_Result + 46) = 0;
	*(uint32_t*)(_Result + 50) = 0;

	for (size_t _Y = 0; _Y < _Image.Height; _Y++)
	{
		for (size_t _X = 0; _X < _Image.Width; _X++)
		{
			(_Result + 14 + 40)[(_X + _Y * _Image.Width) * 4 + 0] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 2];
			(_Result + 14 + 40)[(_X + _Y * _Image.Width) * 4 + 1] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 1];
			(_Result + 14 + 40)[(_X + _Y * _Image.Width) * 4 + 2] = _Image.Data[(_X + _Y * _Image.Width) * 4 + 0];
			(_Result + 14 + 40)[(_X + _Y * _Image.Width) * 4 + 3] = 0;
		}
	}

	return _Result;
}

//char* BSR::Image::SaveHdr(size_t& _FileSize, const HDR& _Image)
//{
//	_FileSize = 0;
//
//	if (!_Image.Width || !_Image.Height || !_Image.Data)
//	{
//		return nullptr;
//	}
//
//	return nullptr;
//}
