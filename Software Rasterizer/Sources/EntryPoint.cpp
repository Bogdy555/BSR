#include "..\Headers\Main.hpp"



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	{
		Rasterizer::Model _Model;

		if (!_Model.Load(L".\\Cubes.wfobj"))
		{
			std::wcout << L"Could not load the 3D model!\n";
			return -1;
		}

		std::wcout << L"The 3D model was loaded.\n";

		if (!_Model.Save(L".\\Cubes.obj"))
		{
			std::wcout << L"Could not save the 3D model!\n";
			return -1;
		}

		std::wcout << L"The 3D model was saved.\n";
	}

	{
		Image::Image _Img;

		_Img.Data = Image::LoadBmp(L".\\Texture.bmp", _Img.Width, _Img.Height);

		if (!_Img.Data)
		{
			std::wcout << L"Could not load the texture!\n";
			return -1;
		}

		std::wcout << L"The texture was loaded.\n";

		Rasterizer::Texture_RGBA _Texture;

		_Texture.SetLerpType(Rasterizer::_LerpNearest);
		_Texture.SetWrapType(Rasterizer::_WrapMirror);
		_Texture.AddDirectMip(std::move(_Img));

		_Img.Width *= 9;
		_Img.Height *= 9;
		_Img.Data = new uint8_t[_Img.Width * _Img.Height * 4];

		if (!_Img.Data)
		{
			std::wcout << L"Runtime error! (no more memory...)\n";
			return -1;
		}

		for (size_t _Y = 0; _Y < _Img.Height; _Y++)
		{
			for (size_t _X = 0; _X < _Img.Width; _X++)
			{
				Math::Vec2f _TextureCoords;

				_TextureCoords.x = ((float)(_X) / (float)(_Img.Width) + 0.5f / (float)(_Img.Width)) * 9.0f - 4.0f;
				_TextureCoords.y = ((float)(_Y) / (float)(_Img.Height) + 0.5f / (float)(_Img.Height)) * 9.0f - 4.0f;

				Math::Vec4f _Rez = _Texture.SampleRGBA(_TextureCoords);

				_Img.Data[(_X + _Y * _Img.Width) * 4 + 0] = (uint8_t)(_Rez.x * 255.0f);
				_Img.Data[(_X + _Y * _Img.Width) * 4 + 1] = (uint8_t)(_Rez.y * 255.0f);
				_Img.Data[(_X + _Y * _Img.Width) * 4 + 2] = (uint8_t)(_Rez.z * 255.0f);
				_Img.Data[(_X + _Y * _Img.Width) * 4 + 3] = (uint8_t)(_Rez.w * 255.0f);
			}
		}

		if (!Image::SaveBmp(L".\\OutputTexture.bmp", _Img.Data, _Img.Width, _Img.Height))
		{
			std::wcout << L"Could not save the output texture!\n";
			delete[] _Img.Data;
			return -1;
		}

		std::wcout << L"The output textured was saved.\n";

		delete[] _Img.Data;
	}

	return 0;
}
