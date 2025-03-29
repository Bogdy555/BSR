#ifndef BSR_RunTime_hpp

#define BSR_RunTime_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace RunTime
	{

		class Application
		{

		public:

			Application();
			Application(const Application& _Other) = delete;
			Application(Application&& _Other) noexcept = delete;
			virtual ~Application();

			int32_t Run(const HINSTANCE _InstanceHandle, const LPWSTR _CmdLine, const int32_t _ShowCmd, const wchar_t* _SharedMemoryName, const wchar_t* _SharedMutexName);

			void TurnOn();
			void Close(const int32_t _ReturnValue);

			void SetCurrentMenu(const uint64_t _CurrentMenu);

			void UpdateCurrentFrameTime();
			void SetLagTime(const float _LagTime);
			void SetSimulationSpeed(const float _SimulationSpeed);
			void SetSync(const uint64_t _Sync);

			const bool CheckOn() const;
			const int32_t GetReturnValue() const;

			const uint64_t GetCurrentMenu() const;

			const MultiProcessing::SharedMemory& GetSharedInstanceMemory() const;
			const MultiProcessing::SharedMutex& GetSharedInstanceMutex() const;

			Time::Timer& GetFrameTime(const size_t _Index);
			const Time::Timer& GetFrameTime(const size_t _Index) const;
			const float GetTimeStep() const;
			const float GetLagTime() const;
			const float GetSimulationSpeed() const;
			const uint64_t GetSync() const;

			const HINSTANCE GetInstanceHandle() const;
			const LPWSTR GetCmdLine() const;
			const int32_t GetShowCmd() const;
			const size_t GetArgC() const;
			const wchar_t* GetArgV(const size_t _Index) const;

			void operator= (const Application& _Other) = delete;
			void operator= (Application&& _Other) noexcept = delete;

		protected:

			virtual void Setup() = 0;
			virtual void Update() = 0;
			virtual void Stop() = 0;

		private:

			bool On;
			int32_t ReturnValue;

			uint64_t CurrentMenu;

			MultiProcessing::SharedMemory SharedInstanceMemory;
			MultiProcessing::SharedMutex SharedInstanceMutex;

			Time::Timer FrameTime[2];
			float LagTime;
			float SimulationSpeed;
			uint64_t Sync;

			HINSTANCE InstanceHandle;
			LPWSTR CmdLine;
			int32_t ShowCmd;

		};

		enum MenuTypes : const uint64_t
		{
			_NullMenu = 0
		};

		class Menu
		{

		public:

			Menu();
			Menu(const Menu& _Other) = delete;
			Menu(Menu&& _Other) noexcept = delete;
			virtual ~Menu();

			void Run(Application* _ApplicationObj);

			virtual const uint64_t GetType() const = 0;

			void operator= (const Menu& _Other) = delete;
			void operator= (Menu&& _Other) noexcept = delete;

		protected:

			void TurnOn();
			void Close(const uint64_t _NextMenu);

			void UpdateCurrentFrameTime();
			void SetLagTime(const float _LagTime);
			void SetSimulationSpeed(const float _SimulationSpeed);
			void SetSync(const uint64_t _Sync);

			const bool CheckOn() const;
			const uint64_t GetNextMenu() const;

			Application* GetApplicationObj();
			const Application* GetApplicationObj() const;

			Time::Timer& GetFrameTime(const size_t _Index);
			const Time::Timer& GetFrameTime(const size_t _Index) const;
			const float GetTimeStep() const;
			const float GetLagTime() const;
			const float GetSimulationSpeed() const;
			const uint64_t GetSync() const;

			virtual void Setup() = 0;
			virtual void Update() = 0;
			virtual void Stop() = 0;

		private:

			bool On;
			uint64_t NextMenu;

			Application* ApplicationObj;

		};

	}

}



#endif
