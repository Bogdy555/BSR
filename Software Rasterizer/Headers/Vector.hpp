#ifndef Vector_hpp

#define Vector_hpp



#include "Main.hpp"



template <typename T> class Vector
{

public:

	Vector() : Size(0), Data(nullptr)
	{

	}

	Vector(const Vector<T>& _Other) : Size(_Other.Size), Data(nullptr)
	{
		if (Size)
		{
			Data = new T[Size];

			assert(Data);

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}
		}
	}

	~Vector()
	{
		if (Data)
		{
			delete[] Data;
		}
	}

	void PushBack(const T& _Object)
	{
		size_t _NewSize = Size + 1;
		T* _NewData = new T[_NewSize];

		assert(_NewData);

		for (size_t _Index = 0; _Index < Size; _Index++)
		{
			_NewData[_Index] = Data[_Index];
		}

		_NewData[_NewSize - 1] = _Object;

		if (Data)
		{
			delete[] Data;
		}

		Data = _NewData;
		Size = _NewSize;
	}

	void Erase(const size_t _ObjIndex)
	{
		assert(_ObjIndex < Size);

		if (Size == 1)
		{
			delete[] Data;
			Data = nullptr;

			Size = 0;

			return;
		}

		size_t _NewSize = Size - 1;
		T* _NewData = new T[_NewSize];

		for (size_t _Index = 0; _Index < _ObjIndex; _Index++)
		{
			_NewData[_Index] = Data[_Index];
		}

		for (size_t _Index = _ObjIndex; _Index < _NewSize; _Index++)
		{
			_NewData[_Index] = Data[_Index + 1];
		}

		if (Data)
		{
			delete[] Data;
		}

		Data = _NewData;
		Size = _NewSize;
	}

	void Clear()
	{
		if (Data)
		{
			delete[] Data;
			Data = nullptr;
		}

		Size = 0;
	}

	const size_t GetSize() const
	{
		return Size;
	}

	T* GetData()
	{
		return Data;
	}

	const T* GetData() const
	{
		return Data;
	}

	operator T* ()
	{
		return Data;
	}

	operator const T* () const
	{
		return Data;
	}

	T* operator-> ()
	{
		return Data;
	}

	const T* operator-> () const
	{
		return Data;
	}

	T& operator* ()
	{
		return *Data;
	}

	const T& operator* () const
	{
		return *Data;
	}

	T& operator[] (size_t _Index)
	{
		return Data[_Index];
	}

	const T& operator[] (size_t _Index) const
	{
		return Data[_Index];
	}

	const Vector<T>& operator= (const Vector<T>& _Other)
	{
		if (Data)
		{
			delete[] Data;
			Data = nullptr;
		}

		Size = _Other.Size;

		if (Size)
		{
			Data = new T[Size];

			assert(Data);

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}
		}

		return *this;
	}

private:

	size_t Size;
	T* Data;

};



#endif
