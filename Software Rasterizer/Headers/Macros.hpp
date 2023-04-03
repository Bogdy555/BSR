#ifndef Macros_hpp

#define Macros_hpp



#include "Main.hpp"



#ifdef UNICODE

#define STRING_TYPE(X) L ## X

#endif

#ifndef UNICODE

#define STRING_TYPE(X) X

#endif



#define PRINT_A(X) std::cout << X
#define PRINT_W(X) std::wcout << X

#ifdef UNICODE

#define PRINT(X) PRINT_W(X)

#endif

#ifndef UNICODE

#define PRINT(X) PRINT_A(X)

#endif

#define PRINT_LINE_A(X) std::cout << X << '\n'
#define PRINT_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define PRINT_LINE(X) PRINT_LINE_W(X)

#endif

#ifndef UNICODE

#define PRINT_LINE(X) PRINT_LINE_A(X)

#endif



#ifdef _DEBUG

#define LOG_A(X) std::cout << X
#define LOG_W(X) std::wcout << X

#ifdef UNICODE

#define LOG(X) LOG_W(X)

#endif

#ifndef UNICODE

#define LOG(X) LOG_A(X)

#endif

#define LOG_LINE_A(X) std::cout << X << '\n'
#define LOG_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define LOG_LINE(X) LOG_LINE_W(X)

#endif

#ifndef UNICODE

#define LOG_LINE(X) LOG_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define LOG_A(X)
#define LOG_W(X)
#define LOG(X)

#define LOG_LINE_A(X)
#define LOG_LINE_W(X)
#define LOG_LINE(X)

#endif



#ifdef _DEBUG

#define OUTPUT_DEBUG_STR_A(X) OutputDebugStringA(X)
#define OUTPUT_DEBUG_STR_W(X) OutputDebugStringW(X)

#ifdef UNICODE

#define OUTPUT_DEBUG_STR(X) OUTPUT_DEBUG_STR_W(X)

#endif

#ifndef UNICODE

#define OUTPUT_DEBUG_STR(X) OUTPUT_DEBUG_STR_A(X)

#endif

#define OUTPUT_DEBUG_STR_LINE_A(X) OutputDebugStringA(X); OutputDebugStringA("\n")
#define OUTPUT_DEBUG_STR_LINE_W(X) OutputDebugStringW(X); OutputDebugStringW(L"\n")

#ifdef UNICODE

#define OUTPUT_DEBUG_STR_LINE(X) OUTPUT_DEBUG_STR_LINE_W(X)

#endif

#ifndef UNICODE

#define OUTPUT_DEBUG_STR_LINE(X) OUTPUT_DEBUG_STR_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define OUTPUT_DEBUG_STR_A(X)
#define OUTPUT_DEBUG_STR_W(X)
#define OUTPUT_DEBUG_STR(X)

#define OUTPUT_DEBUG_STR_LINE_A(X)
#define OUTPUT_DEBUG_STR_LINE_W(X)
#define OUTPUT_DEBUG_STR_LINE(X)

#endif



#ifdef _DEBUG

#define DEBUG_BREAK() __debugbreak()

#define DEBUG_BREAK_MSG_A(Msg) MessageBoxA(NULL, Msg, "Debug break!", MB_OK | MB_ICONERROR); __debugbreak()
#define DEBUG_BREAK_MSG_W(Msg) MessageBoxW(NULL, Msg, L"Debug break!", MB_OK | MB_ICONERROR); __debugbreak()

#ifdef UNICODE

#define DEBUG_BREAK_MSG(Msg) DEBUG_BREAK_MSG_W(Msg)

#endif

#ifndef UNICODE

#define DEBUG_BREAK_MSG(Msg) DEBUG_BREAK_MSG_A(Msg)

#endif

#endif

#ifndef _DEBUG

#define DEBUG_BREAK()
#define DEBUG_BREAK_MSG_A(Msg)
#define DEBUG_BREAK_MSG_W(Msg)
#define DEBUG_BREAK_MSG(Msg)

#endif



#ifdef _DEBUG

#define ASSERT(Condition) if (!(Condition))\
{\
	DEBUG_BREAK();\
}

#define ASSERT_MSG_A(Condition, Msg) if (!(Condition))\
{\
	DEBUG_BREAK_MSG_A(Msg);\
}

#define ASSERT_MSG_W(Condition, Msg) if (!(Condition))\
{\
	DEBUG_BREAK_MSG_W(Msg);\
}

#ifdef UNICODE

#define ASSERT_MSG(Condition, Msg) ASSERT_MSG_W(Condition, Msg)

#endif

#ifndef UNICODE

#define ASSERT_MSG(Condition, Msg) ASSERT_MSG_A(Condition, Msg)

#endif

#endif

#ifndef _DEBUG

#define ASSERT(Condition)
#define ASSERT_MSG_A(Condition, Msg)
#define ASSERT_MSG_W(Condition, Msg)
#define ASSERT_MSG(Condition, Msg)

#endif



#ifdef _DEBUG

#define DEBUG_CALL(X) X
#define RELEASE_CALL(X)

#endif

#ifndef _DEBUG

#define DEBUG_CALL(X)
#define RELEASE_CALL(X) X

#endif



#ifdef _WIN64

#define X86_CALL(X)
#define X64_CALL(X) X

#endif

#ifndef _WIN64

#define X86_CALL(X) X
#define X64_CALL(X)

#endif



#endif
