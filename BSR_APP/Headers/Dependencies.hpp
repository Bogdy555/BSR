#ifndef BSR_APP_Dependencies_hpp

#define BSR_APP_Dependencies_hpp



#pragma comment(lib, "BSR.lib")



#pragma pack(pop)



#include <BSR.hpp>



#pragma pack(push, 1)



#include "Macros.hpp"



namespace BSR_APP
{

	struct WindowData;

	namespace RunTime
	{

		class Application;

		enum MenuTypes : const uint64_t;

		class MainMenu;

	}

}



#include "Templates.hpp"



#endif
