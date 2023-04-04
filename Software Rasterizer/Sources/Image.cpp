#include "..\Headers\Main.hpp"



static bool ReachedFileEnd(FILE* _File)
{
	char _Chr = fgetc(_File);

	if (feof(_File))
	{
		return true;
	}

	fseek(_File, -1, SEEK_CUR);

	return false;
}



static bool OldDecode(FILE* _File, uint8_t* _ScanLine, size_t _Width)
{
	size_t _Len = 0;
	size_t _RightShift = 0;

	while (_Len < _Width)
	{
		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _R = fgetc(_File);

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _G = fgetc(_File);

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _B = fgetc(_File);

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _E = fgetc(_File);

		if (_R == 1 && _G == 1 && _B == 1)
		{
			for (size_t _Index = (size_t)(_E) << _RightShift; _Index > 0; _Index--)
			{
				if (_Len == _Width || _Len == 0)
				{
					return false;
				}
				_ScanLine[_Len * 4 + 0] = _ScanLine[(_Len - 1) * 4 + 0];
				_ScanLine[_Len * 4 + 1] = _ScanLine[(_Len - 1) * 4 + 1];
				_ScanLine[_Len * 4 + 2] = _ScanLine[(_Len - 1) * 4 + 2];
				_ScanLine[_Len * 4 + 3] = _ScanLine[(_Len - 1) * 4 + 3];
				_Len++;
			}
			_RightShift += 8;
		}
		else
		{
			_ScanLine[_Len * 4 + 0] = _R;
			_ScanLine[_Len * 4 + 1] = _G;
			_ScanLine[_Len * 4 + 2] = _B;
			_ScanLine[_Len * 4 + 3] = _E;

			_Len++;
			_RightShift = 0;
		}
	}

	return true;
}

static bool Decode(FILE* _File, uint8_t* _ScanLine, size_t _Width)
{
	if (_Width < 8 || _Width > 32767)
	{
		return OldDecode(_File, _ScanLine, _Width);
	}

	{
		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _Next1 = fgetc(_File);

		if (_Next1 != '\x02')
		{
			fseek(_File, -1, SEEK_CUR);
			return OldDecode(_File, _ScanLine, _Width);
		}

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _Next2 = fgetc(_File);

		if (_Next2 != '\x02')
		{
			fseek(_File, -2, SEEK_CUR);
			return OldDecode(_File, _ScanLine, _Width);
		}

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _Next3 = fgetc(_File);

		if ((_Next3 & 0b10000000) != 0)
		{
			fseek(_File, -3, SEEK_CUR);
			return OldDecode(_File, _ScanLine, _Width);
		}

		if (ReachedFileEnd(_File))
		{
			return false;
		}
		uint8_t _Next4 = fgetc(_File);

		if (((unsigned short)(_Next3) << 8) + (unsigned short)(_Next4) != (unsigned short)(_Width))
		{
			return false;
		}
	}

	for (size_t _Component = 0; _Component < 4; _Component++)
	{
		size_t _X = 0;

		while (_X < _Width)
		{
			if (ReachedFileEnd(_File))
			{
				return false;
			}
			uint8_t _Len = fgetc(_File);

			if (_Len > 128)
			{
				_Len &= 127;

				if (ReachedFileEnd(_File))
				{
					return false;
				}
				uint8_t _Value = fgetc(_File);

				while (_Len)
				{
					_ScanLine[_X * 4 + _Component] = _Value;

					_Len--;
					_X++;
					if ((_X == _Width) && (_Len != 0))
					{
						return false;
					}
				}
			}
			else
			{
				while (_Len)
				{
					if (ReachedFileEnd(_File))
					{
						return false;
					}
					_ScanLine[_X * 4 + _Component] = fgetc(_File);

					_Len--;
					_X++;
					if ((_X == _Width) && (_Len != 0))
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}



static float ConvertComponent(uint8_t _Component, uint8_t _Exponent)
{
	return (float)(_Component) / 256.0f * powf(2.0f, (float)(_Exponent)-128.0f);
}



static float GetRFromXYZ(float _X, float _Y, float _Z)
{
	float _Result = 3.2406f * _X - 1.5372f * _Y - 0.4986f * _Z;
	return _Result * (_Result >= 0.0f);
}

static float GetGFromXYZ(float _X, float _Y, float _Z)
{
	float _Result = -0.9689f * _X + 1.8758f * _Y + 0.0415f * _Z;
	return _Result * (_Result >= 0.0f);
}

static float GetBFromXYZ(float _X, float _Y, float _Z)
{
	float _Result = 0.0557f * _X - 0.2040f * _Y + 1.0570f * _Z;
	return _Result * (_Result >= 0.0f);
}



static void PlaceScanLine(float* _Image, uint8_t* _ScanLine, size_t _Width, size_t _Y, bool _FlipX)
{
	if (_FlipX)
	{
		for (size_t _X = 0; _X < _Width; _X++)
		{
			_Image[(_Width - _X - 1 + _Y * _Width) * 4 + 0] = ConvertComponent(_ScanLine[_X * 4 + 0], _ScanLine[_X * 4 + 3]);
			_Image[(_Width - _X - 1 + _Y * _Width) * 4 + 1] = ConvertComponent(_ScanLine[_X * 4 + 1], _ScanLine[_X * 4 + 3]);
			_Image[(_Width - _X - 1 + _Y * _Width) * 4 + 2] = ConvertComponent(_ScanLine[_X * 4 + 2], _ScanLine[_X * 4 + 3]);
			_Image[(_Width - _X - 1 + _Y * _Width) * 4 + 3] = 1.0f;
		}
	}
	else
	{
		for (size_t _X = 0; _X < _Width; _X++)
		{
			_Image[(_X + _Y * _Width) * 4 + 0] = ConvertComponent(_ScanLine[_X * 4 + 0], _ScanLine[_X * 4 + 3]);
			_Image[(_X + _Y * _Width) * 4 + 1] = ConvertComponent(_ScanLine[_X * 4 + 1], _ScanLine[_X * 4 + 3]);
			_Image[(_X + _Y * _Width) * 4 + 2] = ConvertComponent(_ScanLine[_X * 4 + 2], _ScanLine[_X * 4 + 3]);
			_Image[(_X + _Y * _Width) * 4 + 3] = 1.0f;
		}
	}
}



float* Image::LoadHdr(const wchar_t* _Path, size_t& _Width, size_t& _Height)
{
	if (!_Path)
	{
		return nullptr;
	}

	FILE* _File = nullptr;
	_wfopen_s(&_File, _Path, L"rb");

	if (!_File)
	{
		return nullptr;
	}

	{
		char _RadianceMagicNumbers[12];

		if (fread(_RadianceMagicNumbers, 1, 11, _File) != 11)
		{
			fclose(_File);
			return nullptr;
		}

		_RadianceMagicNumbers[11] = '\0';

		if (strcmp(_RadianceMagicNumbers, "#?RADIANCE\x0A") != 0)
		{
			fclose(_File);
			return nullptr;
		}
	}

	bool _IsXYZ = false;

	{
		size_t _FormatXYZEStringCount = 0;
		size_t _FormatRGBEStringCount = 0;
		Vector<char> _Data;

		if (ReachedFileEnd(_File))
		{
			fclose(_File);
			return nullptr;
		}
		_Data.PushBack(fgetc(_File));

		while (_Data[0] != '\x0A')
		{
			while (_Data[_Data.GetSize() - 1] != '\x0A')
			{
				if (ReachedFileEnd(_File))
				{
					_Data.Clear();
					fclose(_File);
					return nullptr;
				}
				_Data.PushBack(fgetc(_File));
			}

			_Data[_Data.GetSize() - 1] = '\0';

			if (strcmp(_Data.GetData(), "FORMAT=32-bit_rle_xyze") == 0)
			{
				_FormatXYZEStringCount++;
			}
			if (strcmp(_Data.GetData(), "FORMAT=32-bit_rle_rgbe") == 0)
			{
				_FormatRGBEStringCount++;
			}

			_Data.Clear();

			if (ReachedFileEnd(_File))
			{
				fclose(_File);
				return nullptr;
			}
			_Data.PushBack(fgetc(_File));
		}

		_Data.Clear();

		bool _Good = (_FormatXYZEStringCount == 1 && _FormatRGBEStringCount == 0) || (_FormatRGBEStringCount == 1 && _FormatXYZEStringCount == 0);

		if (!_Good)
		{
			fclose(_File);
			return nullptr;
		}

		_IsXYZ = _FormatXYZEStringCount != 0;
	}

	bool _FlipX = false;
	bool _FlipY = false;
	bool _ColumnMajor = false;

	{
		Vector<char> _Data;

		if (ReachedFileEnd(_File))
		{
			fclose(_File);
			return nullptr;
		}
		_Data.PushBack(fgetc(_File));

		while (_Data[_Data.GetSize() - 1] != '\x0A')
		{
			if (ReachedFileEnd(_File))
			{
				_Data.Clear();
				fclose(_File);
				return nullptr;
			}
			_Data.PushBack(fgetc(_File));
		}

		_Data[_Data.GetSize() - 1] = '\0';

		if (_Data.GetSize() < 4)
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[0] != '+' && _Data[0] != '-')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[1] != 'X' && _Data[1] != 'Y')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[2] != ' ')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[3] == '\0' || strchr("0123456789", _Data[3]) == nullptr)
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		size_t _IndexSecondSize = 4;

		while (_Data[3] != '\0' && strchr("0123456789", _Data[_IndexSecondSize]) != nullptr)
		{
			_IndexSecondSize++;
		}

		if (_Data[_IndexSecondSize] != ' ')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		_Data[_IndexSecondSize] = '\0';

		_IndexSecondSize++;

		if (_Data[_IndexSecondSize] != '+' && _Data[_IndexSecondSize] != '-')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if ((_Data[_IndexSecondSize + 1] != 'X' && _Data[_IndexSecondSize + 1] != 'Y') || (_Data[1] == _Data[_IndexSecondSize + 1]))
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[_IndexSecondSize + 2] != ' ')
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		if (_Data[_IndexSecondSize + 3] == '\0' || strchr("0123456789", _Data[_IndexSecondSize + 3]) == nullptr)
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		{
			size_t _Index = _IndexSecondSize + 4;

			while (_Data[_Index] != '\0')
			{
				if (_Data[_Index] == '\0' || strchr("0123456789", _Data[_Index]) == nullptr)
				{
					_Data.Clear();
					fclose(_File);
					return nullptr;
				}

				_Index++;
			}
		}

		if (_Data[1] == 'Y')
		{
			if (_Data[0] == '+')
			{
				_FlipY = true;
			}
			if (_Data[_IndexSecondSize] == '-')
			{
				_FlipX = true;
			}
		}
		else
		{
			_ColumnMajor = true;
			if (_Data[0] == '-')
			{
				_FlipX = true;
			}
			if (_Data[_IndexSecondSize] == '+')
			{
				_FlipY = true;
			}
		}

		{
			std::stringstream _StrStream;
			_StrStream << _Data.GetData() + 3;
			_StrStream >> _Height;
		}

		{
			std::stringstream _StrStream;
			_StrStream << _Data.GetData() + _IndexSecondSize + 3;
			_StrStream >> _Width;
		}

		if (!_Width || !_Height || _Width > 0b1111111111111111 || _Height > 0b1111111111111111)
		{
			_Data.Clear();
			fclose(_File);
			return nullptr;
		}

		_Data.Clear();
	}

	float* _Image = new float[_Width * _Height * 4];

	if (!_Image)
	{
		fclose(_File);
		return nullptr;
	}

	uint8_t* _ScanLine = new uint8_t[_Width * 4];

	if (!_ScanLine)
	{
		delete[] _Image;
		fclose(_File);
		return nullptr;
	}

	for (size_t _Y = 0; _Y < _Height; _Y++)
	{
		if (!Decode(_File, _ScanLine, _Width))
		{
			delete[] _Image;
			delete[] _ScanLine;
			fclose(_File);
			return nullptr;
		}

		if (!_FlipY)
		{
			PlaceScanLine(_Image, _ScanLine, _Width, _Height - _Y - 1, _FlipX);
		}
		else
		{
			PlaceScanLine(_Image, _ScanLine, _Width, _Y, _FlipX);
		}
	}

	delete[] _ScanLine;

	if (!ReachedFileEnd(_File))
	{
		delete[] _Image;
		fclose(_File);
		return nullptr;
	}

	if (_ColumnMajor)
	{
		{
			size_t _AuxSize = _Width;
			_Width = _Height;
			_Height = _AuxSize;
		}

		float* _NewImage = new float[_Width * _Height * 4];

		if (!_NewImage)
		{
			delete[] _Image;
			fclose(_File);
			return nullptr;
		}

		for (size_t _Y = 0; _Y < _Height; _Y++)
		{
			for (size_t _X = 0; _X < _Width; _X++)
			{
				_NewImage[(_X + _Y * _Width) * 4 + 0] = _Image[(_Y + _X * _Height) * 4 + 0];
				_NewImage[(_X + _Y * _Width) * 4 + 1] = _Image[(_Y + _X * _Height) * 4 + 1];
				_NewImage[(_X + _Y * _Width) * 4 + 2] = _Image[(_Y + _X * _Height) * 4 + 2];
				_NewImage[(_X + _Y * _Width) * 4 + 3] = _Image[(_Y + _X * _Height) * 4 + 3];
			}
		}

		delete[] _Image;
		_Image = _NewImage;
	}

	if (_IsXYZ)
	{
		for (size_t _Y = 0; _Y < _Height; _Y++)
		{
			for (size_t _X = 0; _X < _Width; _X++)
			{
				float _ColorX = _Image[(_X + _Y * _Width) * 4 + 0];
				float _ColorY = _Image[(_X + _Y * _Width) * 4 + 1];
				float _ColorZ = _Image[(_X + _Y * _Width) * 4 + 2];

				_Image[(_X + _Y * _Width) * 4 + 0] = GetRFromXYZ(_ColorX, _ColorY, _ColorZ);
				_Image[(_X + _Y * _Width) * 4 + 1] = GetGFromXYZ(_ColorX, _ColorY, _ColorZ);
				_Image[(_X + _Y * _Width) * 4 + 2] = GetBFromXYZ(_ColorX, _ColorY, _ColorZ);
			}
		}
	}

	fclose(_File);

	return _Image;
}



bool Image::SaveBmp(const wchar_t* _Path, const uint8_t* _Img, const size_t _Width, const size_t _Height)
{
	if (!_Path || !_Img || !_Width || !_Height)
	{
		return false;
	}

	FILE* _File = nullptr;

	_wfopen_s(&_File, _Path, L"wb");

	if (!_File)
	{
		return false;
	}

	BITMAPFILEHEADER _FileHeader = { 0 };

	_FileHeader.bfType = (unsigned short)('B') | ((unsigned short)('M') << (unsigned short)(8));
	_FileHeader.bfSize = (unsigned long)(sizeof(BITMAPFILEHEADER)) + (unsigned long)(sizeof(BITMAPINFOHEADER)) + (unsigned long)(_Width * _Height * 4);
	_FileHeader.bfReserved1 = 0;
	_FileHeader.bfReserved2 = 0;
	_FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	if (fwrite(&_FileHeader, 1, sizeof(BITMAPFILEHEADER), _File) != sizeof(BITMAPFILEHEADER))
	{
		fclose(_File);
		_wremove(_Path);
		return false;
	}

	BITMAPINFOHEADER _BmpInfo = { 0 };

	_BmpInfo.biSize = sizeof(BITMAPINFOHEADER);
	_BmpInfo.biWidth = (LONG)(_Width);
	_BmpInfo.biHeight = (LONG)(_Height);
	_BmpInfo.biPlanes = 1;
	_BmpInfo.biBitCount = 32;
	_BmpInfo.biCompression = BI_RGB;
	_BmpInfo.biSizeImage = 0;
	_BmpInfo.biXPelsPerMeter = 0;
	_BmpInfo.biYPelsPerMeter = 0;
	_BmpInfo.biClrUsed = 0;
	_BmpInfo.biClrImportant = 0;

	if (fwrite(&_BmpInfo, 1, sizeof(BITMAPINFOHEADER), _File) != sizeof(BITMAPINFOHEADER))
	{
		fclose(_File);
		_wremove(_Path);
		return false;
	}

	uint8_t* _NewImg = new uint8_t[_Width * _Height * 4];

	if (!_NewImg)
	{
		fclose(_File);
		_wremove(_Path);
		return false;
	}

	for (size_t _Index = 0; _Index < _Width * _Height; _Index++)
	{
		_NewImg[_Index * 4 + 0] = _Img[_Index * 4 + 2];
		_NewImg[_Index * 4 + 1] = _Img[_Index * 4 + 1];
		_NewImg[_Index * 4 + 2] = _Img[_Index * 4 + 0];
		_NewImg[_Index * 4 + 3] = 0;
	}

	for (size_t y = 0; y < _Height; y++)
	{
		if (fwrite(_NewImg + y * _Width * 4, 1, _Width * 4, _File) != _Width * 4)
		{
			delete[] _NewImg;
			fclose(_File);
			_wremove(_Path);
			return false;
		}
	}

	delete[] _NewImg;

	fclose(_File);

	return true;
}



uint8_t* Image::LoadBmp(const wchar_t* _Path, size_t& _Width, size_t& _Height)
{
	uint8_t* _Data = nullptr;
	_Width = 0;
	_Height = 0;

	if (!_Path)
	{
		return nullptr;
	}

	HBITMAP _hBmp = (HBITMAP)(LoadImage(NULL, _Path, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));

	if (!_hBmp)
	{
		return nullptr;
	}

	BITMAP _Bmp = { 0 };

	if (!GetObject(_hBmp, sizeof(BITMAP), &_Bmp))
	{
		DeleteObject(_hBmp);
		return nullptr;
	}

	_Data = new uint8_t[(size_t)(_Bmp.bmWidth) * (size_t)(_Bmp.bmHeight) * 4];

	if (!_Data)
	{
		DeleteObject(_hBmp);
		return nullptr;
	}

	if (!GetBitmapBits(_hBmp, _Bmp.bmWidth * _Bmp.bmHeight * 4, _Data))
	{
		delete[] _Data;
		DeleteObject(_hBmp);
		return nullptr;
	}

	for (size_t _Index = 0; _Index < (size_t)(_Bmp.bmWidth) * (size_t)(_Bmp.bmHeight); _Index++)
	{
		uint8_t _Aux = _Data[_Index * 4 + 0];
		_Data[_Index * 4 + 0] = _Data[_Index * 4 + 2];
		_Data[_Index * 4 + 2] = _Aux;
		_Data[_Index * 4 + 3] = 255;
	}

	for (size_t _Y = 0; _Y < (size_t)(_Bmp.bmHeight / 2); _Y++)
	{
		for (size_t _X = 0; _X < (size_t)(_Bmp.bmWidth); _X++)
		{
			uint8_t _Aux0 = _Data[(_X + _Y * _Bmp.bmWidth) * 4 + 0];
			uint8_t _Aux1 = _Data[(_X + _Y * _Bmp.bmWidth) * 4 + 1];
			uint8_t _Aux2 = _Data[(_X + _Y * _Bmp.bmWidth) * 4 + 2];
			uint8_t _Aux3 = _Data[(_X + _Y * _Bmp.bmWidth) * 4 + 3];

			_Data[(_X + _Y * _Bmp.bmWidth) * 4 + 0] = _Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 0];
			_Data[(_X + _Y * _Bmp.bmWidth) * 4 + 1] = _Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 1];
			_Data[(_X + _Y * _Bmp.bmWidth) * 4 + 2] = _Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 2];
			_Data[(_X + _Y * _Bmp.bmWidth) * 4 + 3] = _Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 3];

			_Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 0] = _Aux0;
			_Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 1] = _Aux1;
			_Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 2] = _Aux2;
			_Data[(_X + (_Bmp.bmHeight - _Y - 1) * _Bmp.bmWidth) * 4 + 3] = _Aux3;
		}
	}

	_Width = _Bmp.bmWidth;
	_Height = _Bmp.bmHeight;

	DeleteObject(_hBmp);

	return _Data;
}
