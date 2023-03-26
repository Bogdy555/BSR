#ifndef Macros_hpp

#define Macros_hpp



#include "Main.hpp"



#ifdef _DEBUG

#define DEBUG_CALL(X) X

#define RELEASE_CALL(X)

#endif



#ifndef _DEBUG

#define DEBUG_CALL(X)

#define RELEASE_CALL(X) X

#endif



#ifndef nullptr

#define nullptr NULL

#endif



#endif
