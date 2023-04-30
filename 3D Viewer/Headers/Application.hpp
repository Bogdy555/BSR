#ifndef Application_hpp

#define Application_hpp



#include "BSR_APP.hpp"



namespace BSR_APP
{

	class Application : public BSR::Application
	{

	public:

		Application();
		Application(const Application& _Other) = delete;
		Application(Application&& _Other) noexcept = delete;
		~Application();

		void Setup() override;
		void Update() override;
		void Stop() override;

		void operator= (const Application& _Other) = delete;
		void operator= (Application&& _Other) noexcept = delete;

	private:



	};

}



#endif
