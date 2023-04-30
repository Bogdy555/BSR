#ifndef BSR_MultiProcessing_hpp

#define BSR_MultiProcessing_hpp



#include "BSR.hpp"



namespace BSR
{

	namespace MultiProcessing
	{

		enum ReturnValues : const int32_t
		{
			_ReturnNoError = 0,
			_ReturnError = -1,
			_ReturnStillRunning = 259
		};

		class SharedMemory
		{

		public:

			SharedMemory();
			SharedMemory(const SharedMemory& _Other) = delete;
			SharedMemory(SharedMemory&& _Other) noexcept;
			~SharedMemory();

			bool Create(const wchar_t* _Name, const size_t _Size);
			void Destroy();

			const bool CheckCreated() const;

			const HANDLE GetHandle() const;
			const size_t GetSize() const;
			uint8_t* GetData();
			const uint8_t* GetData() const;

			operator uint8_t* ();
			operator const uint8_t* () const;

			void operator= (const SharedMemory& _Other) = delete;
			void operator= (SharedMemory&& _Other) noexcept;

		private:

			HANDLE Handle;

			size_t Size;
			uint8_t* Data;

		};

		class SharedMutex
		{

		public:

			SharedMutex();
			SharedMutex(const SharedMutex& _Other) = delete;
			SharedMutex(SharedMutex&& _Other) noexcept;
			~SharedMutex();

			bool Create(const wchar_t* _Name);
			void Destroy();

			void Lock();
			void Unlock();

			const bool CheckCreated() const;

			const HANDLE GetHandle() const;

			void operator= (const SharedMutex& _Other) = delete;
			void operator= (SharedMutex&& _Other) noexcept;

		private:

			HANDLE Handle;

		};

		class Process
		{

		public:

			Process();
			Process(const Process& _Other) = delete;
			Process(Process&& _Other) noexcept;
			~Process();

			bool Create(const wchar_t* _Path, const wchar_t* _WorkingDir, const wchar_t* _CmdLine, const int32_t _ShowCmd);
			bool CreateElevated(const wchar_t* _Path, const wchar_t* _WorkingDir, const wchar_t* _CmdLine, const int32_t _ShowCmd);
			void Destroy(const int32_t _ReturnValue);
			void Release();
			int32_t UpdateStatus();

			const bool CheckCreated() const;

			const HANDLE GetHandle() const;

			void operator= (const Process& _Other) = delete;
			void operator= (Process&& _Other) noexcept;

		private:

			HANDLE Handle;

		};

	}

}



#endif
