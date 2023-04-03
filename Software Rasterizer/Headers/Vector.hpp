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

			ASSERT(Data);

			if (!Data)
			{
				Size = 0;
				return;
			}

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}
		}
	}

	Vector(Vector<T>&& _Other) noexcept : Size(_Other.Size), Data(_Other.Data)
	{
		_Other.Size = 0;
		_Other.Data = nullptr;
	}

	Vector(const std::initializer_list<T>& _List) : Size(0), Data(nullptr)
	{
		for (const T& _Elem : _List)
		{
			PushBack(_Elem);
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

		ASSERT(_NewData);

		if (!_NewData)
		{
			return;
		}

		for (size_t _Index = 0; _Index < _NewSize - 1; _Index++)
		{
			_NewData[_Index] = (T&&)(Data[_Index]);
		}

		_NewData[_NewSize - 1] = _Object;

		if (Data)
		{
			delete[] Data;
		}

		Data = _NewData;
		Size = _NewSize;
	}

	void EmplaceBack(T&& _Object)
	{
		size_t _NewSize = Size + 1;
		T* _NewData = new T[_NewSize];

		ASSERT(_NewData);

		if (!_NewData)
		{
			return;
		}

		for (size_t _Index = 0; _Index < _NewSize - 1; _Index++)
		{
			_NewData[_Index] = (T&&)(Data[_Index]);
		}

		_NewData[_NewSize - 1] = (T&&)(_Object);

		if (Data)
		{
			delete[] Data;
		}

		Data = _NewData;
		Size = _NewSize;
	}

	template <typename... TParams> void EmplaceBack(TParams&&... _Object)
	{
		size_t _NewSize = Size + 1;
		T* _NewData = new T[_NewSize];

		ASSERT(_NewData);

		if (!_NewData)
		{
			return;
		}

		for (size_t _Index = 0; _Index < _NewSize - 1; _Index++)
		{
			_NewData[_Index] = (T&&)(Data[_Index]);
		}

		_NewData[_NewSize - 1] = (T&&)(T(_Object...));

		if (Data)
		{
			delete[] Data;
		}

		Data = _NewData;
		Size = _NewSize;
	}

	void Erase(const size_t _ObjIndex)
	{
		ASSERT(_ObjIndex < Size);

		if (_ObjIndex < Size)
		{
			return;
		}

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
			_NewData[_Index] = (T&&)(Data[_Index]);
		}

		for (size_t _Index = _ObjIndex; _Index < _NewSize; _Index++)
		{
			_NewData[_Index] = (T&&)(Data[_Index + 1]);
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

			ASSERT(Data);

			if (!Data)
			{
				Size = 0;
				return *this;
			}

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}
		}

		return *this;
	}

	const Vector<T>& operator= (Vector<T>&& _Other) noexcept
	{
		if (Data)
		{
			delete[] Data;
		}

		Size = _Other.Size;
		Data = _Other.Data;

		_Other.Size = 0;
		_Other.Data = nullptr;

		return *this;
	}

private:

	size_t Size;
	T* Data;

};



template <typename T> std::ostream& operator<< (std::ostream& _OutStream, const Vector<T>& _Vec)
{
	_OutStream << _Vec.GetSize() << '\n';

	for (size_t _Index = 0; _Index < _Vec.GetSize() - 1; _Index++)
	{
		_OutStream << _Vec[_Index] << ' ';
	}

	if (_Vec.GetSize())
	{
		_OutStream << _Vec[_Vec.GetSize() - 1];
	}

	return _OutStream;
}



template <typename T> std::istream& operator>> (std::istream& _InStream, Vector<T>& _Vec)
{
	size_t _Size = 0;

	_InStream >> _Size;

	for (size_t _Index = 0; _Index < _Size; _Index++)
	{
		T _Elem;

		_InStream >> _Elem;

		_Vec.EmplaceBack(std::move(_Elem));
	}

	return _InStream;
}



template <typename T> std::wostream& operator<< (std::wostream& _OutStream, const Vector<T>& _Vec)
{
	_OutStream << _Vec.GetSize() << '\n';

	for (size_t _Index = 0; _Index < _Vec.GetSize() - 1; _Index++)
	{
		_OutStream << _Vec[_Index] << ' ';
	}

	if (_Vec.GetSize())
	{
		_OutStream << _Vec[_Vec.GetSize() - 1];
	}

	return _OutStream;
}



template <typename T> std::wistream& operator>> (std::wistream& _InStream, Vector<T>& _Vec)
{
	size_t _Size = 0;

	_InStream >> _Size;

	for (size_t _Index = 0; _Index < _Size; _Index++)
	{
		T _Elem;

		_InStream >> _Elem;

		_Vec.EmplaceBack(std::move(_Elem));
	}

	return _InStream;
}



#endif
