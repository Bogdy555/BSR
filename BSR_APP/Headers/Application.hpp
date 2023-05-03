#ifndef BSR_APP_Application_hpp

#define BSR_APP_Application_hpp



#include "..\Headers\BSR_APP.hpp"



namespace BSR_APP
{

	namespace RunTime
	{

		class Application : public BSR::RunTime::Application
		{

		public:

			Application();
			Application(const Application& _Other) = delete;
			Application(Application&& _Other) noexcept;
			~Application();

			void operator= (const Application& _Other) = delete;
			void operator= (Application&& _Other) noexcept;

		private:

			void Setup() override;
			void Update() override;
			void Stop() override;

		};

	}

}



#endif
