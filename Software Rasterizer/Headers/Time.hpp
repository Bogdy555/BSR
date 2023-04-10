#ifndef Time_hpp

#define Time_hpp



#include "Main.hpp"



namespace Time
{

	class Timer
	{

	public:

		Timer();
		Timer(const Timer& _Other);
		Timer(Timer&& _Other) noexcept;
		~Timer();

		void Start();
		void Stop();
		void Restart();

		operator const float() const;

		void operator= (const Timer& _Other);
		void operator= (Timer&& _Other) noexcept;

	private:

		std::chrono::system_clock::time_point Begin;
		std::chrono::system_clock::time_point End;

	};

	bool Init();
	void Stop();

}



#endif
