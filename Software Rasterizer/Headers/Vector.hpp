#ifndef Vector_hpp

#define Vector_hpp



#include "Main.hpp"



template <typename T> class Vector
{

public:

	Vector() : Size(0), Data(nullptr)
	{

	}

	~Vector()
	{

	}

private:

	size_t Size;
	T* Data;

};



#endif
