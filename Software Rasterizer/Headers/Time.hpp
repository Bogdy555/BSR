#ifndef BSR_Time_hpp

#define BSR_Time_hpp



#include "BSR.hpp"



namespace BSR
{

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

}



#endif
