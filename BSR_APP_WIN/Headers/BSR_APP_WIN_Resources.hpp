#ifndef BSR_APP_WIN_Resources_hpp

#define BSR_APP_WIN_Resources_hpp



#define BSR_APP_WIN_VERSION_LIST 1, 0, 0, 0



#define BSR_APP_WIN_VERSION_STRING_A "1.0.0.0"
#define BSR_APP_WIN_VERSION_STRING_W L"1.0.0.0"

#ifdef _UNICODE

#define BSR_APP_WIN_VERSION_STRING BSR_APP_WIN_VERSION_STRING_W

#endif

#ifndef _UNICODE

#define BSR_APP_WIN_VERSION_STRING BSR_APP_WIN_VERSION_STRING_A

#endif



#define BSR_APP_WIN_APP_NAME_A "BSR_APP_WIN"
#define BSR_APP_WIN_APP_NAME_W L"BSR_APP_WIN"

#ifdef _UNICODE

#define BSR_APP_WIN_APP_NAME BSR_APP_WIN_APP_NAME_W

#endif

#ifndef _UNICODE

#define BSR_APP_WIN_APP_NAME BSR_APP_WIN_APP_NAME_A

#endif



#define BSR_APP_WIN_FILE_NAME_A "BSR_APP_WIN.exe"
#define BSR_APP_WIN_FILE_NAME_W L"BSR_APP_WIN.exe"

#ifdef _UNICODE

#define BSR_APP_WIN_FILE_NAME BSR_APP_WIN_FILE_NAME_W

#endif

#ifndef _UNICODE

#define BSR_APP_WIN_FILE_NAME BSR_APP_WIN_FILE_NAME_A

#endif



#define BSR_APP_WIN_COMPANY_NAME_A "BSW"
#define BSR_APP_WIN_COMPANY_NAME_W L"BSW"

#ifdef _UNICODE

#define BSR_APP_WIN_COMPANY_NAME BSR_APP_WIN_COMPANY_NAME_W

#endif

#ifndef _UNICODE

#define BSR_APP_WIN_COMPANY_NAME BSR_APP_WIN_COMPANY_NAME_A

#endif



#define BSR_APP_WIN_COPY_RIGHT_EN_US_A "Copyright BSW. All rights reserved."
#define BSR_APP_WIN_COPY_RIGHT_EN_US_W L"Copyright © BSW™. All rights reserved."

#ifdef _UNICODE

#define BSR_APP_WIN_COPY_RIGHT_EN_US BSR_APP_WIN_COPY_RIGHT_EN_US_W

#endif

#ifndef _UNICODE

#define BSR_APP_WIN_COPY_RIGHT_EN_US BSR_APP_WIN_COPY_RIGHT_EN_US_A

#endif



#define BSR_APP_WIN_IDI_MAIN_ICON 1



#define BSR_APP_WIN_IDM_MAIN_MANIFEST 1



#endif
