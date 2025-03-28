#ifndef BSR_Vector_hpp

#define BSR_Vector_hpp



#include "BSR.hpp"



namespace BSR
{

	template <typename T> class Vector
	{

	public:

		Vector() : Capacity(0), Size(0), Data(nullptr)
		{

		}

		Vector(const Vector& _Other) : Capacity(_Other.Capacity), Size(_Other.Size), Data(nullptr)
		{
			if (!_Other.Capacity)
			{
				return;
			}

			Data = new T[_Other.Capacity];

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}
		}

		Vector(Vector&& _Other) noexcept : Capacity(_Other.Capacity), Size(_Other.Size), Data(_Other.Data)
		{
			_Other.Capacity = 0;
			_Other.Size = 0;
			_Other.Data = nullptr;
		}

		~Vector()
		{
			delete[] Data;
		}

		void PushBack(const T& _Obj)
		{
			if (Size == 0)
			{
				Capacity = 1;
				Size = 1;
				Data = new T[1];
				Data[0] = _Obj;
				return;
			}

			if (Size + 1 > Capacity)
			{
				T* _NewData = new T[Capacity * 2];

				for (size_t _Index = 0; _Index < Size; _Index++)
				{
					_NewData[_Index] = (T&&)(Data[_Index]);
				}

				_NewData[Size] = _Obj;

				Capacity *= 2;
				Size += 1;
				delete[] Data;
				Data = _NewData;
				return;
			}

			Data[Size] = _Obj;
			Size++;
		}

		void EmplaceBack(T&& _Obj)
		{
			if (Size == 0)
			{
				Capacity = 1;
				Size = 1;
				Data = new T[1];
				Data[0] = (T&&)(_Obj);
				return;
			}

			if (Size + 1 > Capacity)
			{
				T* _NewData = new T[Capacity * 2];

				for (size_t _Index = 0; _Index < Size; _Index++)
				{
					_NewData[_Index] = (T&&)(Data[_Index]);
				}

				_NewData[Size] = (T&&)(_Obj);

				Capacity *= 2;
				Size += 1;
				delete[] Data;
				Data = _NewData;
				return;
			}

			Data[Size] = (T&&)(_Obj);
			Size++;
		}

		void Erase(const size_t _EraseIndex)
		{
			if (Size == 0)
			{
				return;
			}

			if (Size - 1 == 0)
			{
				Clear();
				return;
			}

			if (Size - 1 == Capacity / 2)
			{
				T* _NewData = new T[Capacity / 2];

				for (size_t _Index = 0; _Index < _EraseIndex; _Index++)
				{
					_NewData[_Index] = (T&&)(Data[_Index]);
				}

				for (size_t _Index = _EraseIndex; _Index < Size - 1; _Index++)
				{
					_NewData[_Index] = (T&&)(Data[_Index + 1]);
				}

				Capacity /= 2;
				Size--;
				delete[] Data;
				Data = _NewData;

				return;
			}

			for (size_t _Index = _EraseIndex; _Index < Size - 1; _Index++)
			{
				Data[_Index] = (T&&)(Data[_Index + 1]);
			}

			Size--;
		}

		void Clear()
		{
			Capacity = 0;
			Size = 0;
			delete[] Data;
			Data = nullptr;
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

		T& operator[] (const size_t _Index)
		{
			return Data[_Index];
		}

		const T& operator[] (const size_t _Index) const
		{
			return Data[_Index];
		}

		Vector& operator= (const Vector& _Other)
		{
			delete[] Data;

			Capacity = _Other.Capacity;
			Size = _Other.Size;
			Data = nullptr;

			if (!_Other.Capacity)
			{
				return *this;
			}

			Data = new T[_Other.Capacity];

			for (size_t _Index = 0; _Index < Size; _Index++)
			{
				Data[_Index] = _Other.Data[_Index];
			}

			return *this;
		}

		Vector& operator= (Vector&& _Other) noexcept
		{
			delete[] Data;

			Capacity = _Other.Capacity;
			Size = _Other.Size;
			Data = _Other.Data;

			_Other.Capacity = 0;
			_Other.Size = 0;
			_Other.Data = nullptr;

			return *this;
		}

	private:

		size_t Capacity;
		size_t Size;
		T* Data;

	};

}



#endif
