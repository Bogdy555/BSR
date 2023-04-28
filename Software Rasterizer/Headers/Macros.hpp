#ifndef BSR_Macros_hpp

#define BSR_Macros_hpp



#include "BSR.hpp"



#ifdef UNICODE

#define BSR_STRING_TYPE(X) L ## X

#endif

#ifndef UNICODE

#define BSR_STRING_TYPE(X) X

#endif



#define BSR_PRINT_A(X) std::cout << X
#define BSR_PRINT_W(X) std::wcout << X

#ifdef UNICODE

#define BSR_PRINT(X) BSR_PRINT_W(X)

#endif

#ifndef UNICODE

#define BSR_PRINT(X) BSR_PRINT_A(X)

#endif

#define BSR_PRINT_LINE_A(X) std::cout << X << '\n'
#define BSR_PRINT_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define BSR_PRINT_LINE(X) BSR_PRINT_LINE_W(X)

#endif

#ifndef UNICODE

#define BSR_PRINT_LINE(X) BSR_PRINT_LINE_A(X)

#endif



#ifdef _DEBUG

#define BSR_LOG_A(X) std::cout << X
#define BSR_LOG_W(X) std::wcout << X

#ifdef UNICODE

#define BSR_LOG(X) BSR_LOG_W(X)

#endif

#ifndef UNICODE

#define BSR_LOG(X) BSR_LOG_A(X)

#endif

#define BSR_LOG_LINE_A(X) std::cout << X << '\n'
#define BSR_LOG_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define BSR_LOG_LINE(X) BSR_LOG_LINE_W(X)

#endif

#ifndef UNICODE

#define BSR_LOG_LINE(X) BSR_LOG_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define BSR_LOG_A(X)
#define BSR_LOG_W(X)
#define BSR_LOG(X)

#define BSR_LOG_LINE_A(X)
#define BSR_LOG_LINE_W(X)
#define BSR_LOG_LINE(X)

#endif



#ifdef _DEBUG

#define BSR_OUTPUT_DEBUG_STR_A(X) OutputDebugStringA(X)
#define BSR_OUTPUT_DEBUG_STR_W(X) OutputDebugStringW(X)

#ifdef UNICODE

#define BSR_OUTPUT_DEBUG_STR(X) BSR_OUTPUT_DEBUG_STR_W(X)

#endif

#ifndef UNICODE

#define BSR_OUTPUT_DEBUG_STR(X) BSR_OUTPUT_DEBUG_STR_A(X)

#endif

#define BSR_OUTPUT_DEBUG_STR_LINE_A(X) OutputDebugStringA(X); OutputDebugStringA("\n")
#define BSR_OUTPUT_DEBUG_STR_LINE_W(X) OutputDebugStringW(X); OutputDebugStringW(L"\n")

#ifdef UNICODE

#define BSR_OUTPUT_DEBUG_STR_LINE(X) BSR_OUTPUT_DEBUG_STR_LINE_W(X)

#endif

#ifndef UNICODE

#define BSR_OUTPUT_DEBUG_STR_LINE(X) BSR_OUTPUT_DEBUG_STR_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define BSR_OUTPUT_DEBUG_STR_A(X)
#define BSR_OUTPUT_DEBUG_STR_W(X)
#define BSR_OUTPUT_DEBUG_STR(X)

#define BSR_OUTPUT_DEBUG_STR_LINE_A(X)
#define BSR_OUTPUT_DEBUG_STR_LINE_W(X)
#define BSR_OUTPUT_DEBUG_STR_LINE(X)

#endif



#ifdef _DEBUG

#define BSR_DEBUG_BREAK() __debugbreak()

#define BSR_DEBUG_BREAK_MSG_A(Msg) MessageBoxA(NULL, Msg, "Debug break!", MB_OK | MB_ICONERROR); __debugbreak()
#define BSR_DEBUG_BREAK_MSG_W(Msg) MessageBoxW(NULL, Msg, L"Debug break!", MB_OK | MB_ICONERROR); __debugbreak()

#ifdef UNICODE

#define BSR_DEBUG_BREAK_MSG(Msg) BSR_DEBUG_BREAK_MSG_W(Msg)

#endif

#ifndef UNICODE

#define BSR_DEBUG_BREAK_MSG(Msg) BSR_DEBUG_BREAK_MSG_A(Msg)

#endif

#endif

#ifndef _DEBUG

#define BSR_DEBUG_BREAK()
#define BSR_DEBUG_BREAK_MSG_A(Msg)
#define BSR_DEBUG_BREAK_MSG_W(Msg)
#define BSR_DEBUG_BREAK_MSG(Msg)

#endif



#ifdef _DEBUG

#define BSR_ASSERT(Condition) if (!(Condition))\
{\
	BSR_DEBUG_BREAK();\
}

#define BSR_ASSERT_MSG_A(Condition, Msg) if (!(Condition))\
{\
	BSR_DEBUG_BREAK_MSG_A(Msg);\
}

#define BSR_ASSERT_MSG_W(Condition, Msg) if (!(Condition))\
{\
	BSR_DEBUG_BREAK_MSG_W(Msg);\
}

#ifdef UNICODE

#define BSR_ASSERT_MSG(Condition, Msg) BSR_ASSERT_MSG_W(Condition, Msg)

#endif

#ifndef UNICODE

#define BSR_ASSERT_MSG(Condition, Msg) BSR_ASSERT_MSG_A(Condition, Msg)

#endif

#endif

#ifndef _DEBUG

#define BSR_ASSERT(Condition)
#define BSR_ASSERT_MSG_A(Condition, Msg)
#define BSR_ASSERT_MSG_W(Condition, Msg)
#define BSR_ASSERT_MSG(Condition, Msg)

#endif



#ifdef _DEBUG

#define BSR_DEBUG_CALL(X) X
#define BSR_RELEASE_CALL(X)

#endif

#ifndef _DEBUG

#define BSR_DEBUG_CALL(X)
#define BSR_RELEASE_CALL(X) X

#endif



#ifdef _WIN64

#define BSR_X86_CALL(X)
#define BSR_X64_CALL(X) X

#endif

#ifndef _WIN64

#define BSR_X86_CALL(X) X
#define BSR_X64_CALL(X)

#endif



#endif
