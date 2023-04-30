#include "..\Headers\BSR.hpp"



BSR::MultiProcessing::SharedMemory::SharedMemory() : Handle(NULL), Size(0), Data(nullptr)
{

}

BSR::MultiProcessing::SharedMemory::SharedMemory(SharedMemory&& _Other) noexcept : Handle(_Other.Handle), Size(_Other.Size), Data(_Other.Data)
{
	_Other.Handle = NULL;
	_Other.Size = 0;
	_Other.Data = nullptr;
}

BSR::MultiProcessing::SharedMemory::~SharedMemory()
{
	Destroy();
}

bool BSR::MultiProcessing::SharedMemory::Create(const wchar_t* _Name, const size_t _Size)
{
	Destroy();

	if (!_Name || !_Size)
	{
		return false;
	}

	Handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (uint32_t)(_Size), _Name);

	if (!Handle)
	{
		return false;
	}

	Data = (uint8_t*)(MapViewOfFile(Handle, FILE_MAP_ALL_ACCESS, 0, 0, _Size));

	if (!Data)
	{
		CloseHandle(Handle);
		Handle = NULL;
		return false;
	}

	Size = _Size;

	return true;
}

void BSR::MultiProcessing::SharedMemory::Destroy()
{
	if (!Handle)
	{
		return;
	}

	UnmapViewOfFile(Data);
	CloseHandle(Handle);

	Handle = NULL;
	Size = 0;
	Data = nullptr;
}

const bool BSR::MultiProcessing::SharedMemory::CheckCreated() const
{
	return Handle != NULL;
}

const HANDLE BSR::MultiProcessing::SharedMemory::GetHandle() const
{
	return Handle;
}

const size_t BSR::MultiProcessing::SharedMemory::GetSize() const
{
	return Size;
}

uint8_t* BSR::MultiProcessing::SharedMemory::GetData()
{
	return Data;
}

const uint8_t* BSR::MultiProcessing::SharedMemory::GetData() const
{
	return Data;
}

BSR::MultiProcessing::SharedMemory::operator uint8_t* ()
{
	return Data;
}

BSR::MultiProcessing::SharedMemory::operator const uint8_t* () const
{
	return Data;
}

void BSR::MultiProcessing::SharedMemory::operator= (SharedMemory&& _Other) noexcept
{
	Destroy();

	Handle = _Other.Handle;
	Size = _Other.Size;
	Data = _Other.Data;

	_Other.Handle = NULL;
	_Other.Size = 0;
	_Other.Data = nullptr;
}



BSR::MultiProcessing::SharedMutex::SharedMutex() : Handle(NULL)
{

}

BSR::MultiProcessing::SharedMutex::SharedMutex(SharedMutex&& _Other) noexcept : Handle(_Other.Handle)
{
	_Other.Handle = NULL;
}

BSR::MultiProcessing::SharedMutex::~SharedMutex()
{
	Destroy();
}

bool BSR::MultiProcessing::SharedMutex::Create(const wchar_t* _Name)
{
	Destroy();

	Handle = CreateMutex(NULL, FALSE, _Name);

	if (!Handle)
	{
		return false;
	}

	return true;
}

void BSR::MultiProcessing::SharedMutex::Destroy()
{
	if (!Handle)
	{
		return;
	}

	BSR_ASSERT_MSG(CloseHandle(Handle), BSR_STRING_TYPE("Mutex destruction failed."));
	Handle = NULL;
}

void BSR::MultiProcessing::SharedMutex::Lock()
{
	if (!Handle)
	{
		return;
	}

	WaitForSingleObject(Handle, INFINITE);
}

void BSR::MultiProcessing::SharedMutex::Unlock()
{
	if (!Handle)
	{
		return;
	}

	ReleaseMutex(Handle);
}

const bool BSR::MultiProcessing::SharedMutex::CheckCreated() const
{
	return Handle != NULL;
}

const HANDLE BSR::MultiProcessing::SharedMutex::GetHandle() const
{
	return Handle;
}

void BSR::MultiProcessing::SharedMutex::operator= (SharedMutex&& _Other) noexcept
{
	Destroy();

	Handle = _Other.Handle;

	_Other.Handle = NULL;
}



BSR::MultiProcessing::Process::Process() : Handle(NULL)
{

}

BSR::MultiProcessing::Process::Process(Process&& _Other) noexcept : Handle(_Other.Handle)
{
	_Other.Handle = NULL;
}

BSR::MultiProcessing::Process::~Process()
{
	Release();
}

bool BSR::MultiProcessing::Process::Create(const wchar_t* _Path, const wchar_t* _WorkingDir, const wchar_t* _CmdLine, const int32_t _ShowCmd)
{
	Release();

	if (!_Path)
	{
		return false;
	}

	SHELLEXECUTEINFO _ShellExecInfo = { 0 };

	_ShellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	_ShellExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	_ShellExecInfo.lpFile = _Path;
	_ShellExecInfo.lpParameters = _CmdLine;
	_ShellExecInfo.lpDirectory = _WorkingDir;
	_ShellExecInfo.nShow = _ShowCmd;

	if (!ShellExecuteEx(&_ShellExecInfo))
	{
		return false;
	}

	if (!_ShellExecInfo.hProcess)
	{
		return false;
	}

	Handle = _ShellExecInfo.hProcess;

	return true;
}

bool BSR::MultiProcessing::Process::CreateElevated(const wchar_t* _Path, const wchar_t* _WorkingDir, const wchar_t* _CmdLine, const int32_t _ShowCmd)
{
	Release();

	if (!_Path)
	{
		return false;
	}

	SHELLEXECUTEINFO _ShellExecInfo = { 0 };

	_ShellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	_ShellExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	_ShellExecInfo.lpVerb = L"runas";
	_ShellExecInfo.lpFile = _Path;
	_ShellExecInfo.lpParameters = _CmdLine;
	_ShellExecInfo.lpDirectory = _WorkingDir;
	_ShellExecInfo.nShow = _ShowCmd;

	if (!ShellExecuteEx(&_ShellExecInfo))
	{
		return false;
	}

	if (!_ShellExecInfo.hProcess)
	{
		return false;
	}

	Handle = _ShellExecInfo.hProcess;

	return true;
}

void BSR::MultiProcessing::Process::Destroy(const int32_t _ReturnValue)
{
	if (!Handle)
	{
		return;
	}

	TerminateProcess(Handle, _ReturnValue);

	CloseHandle(Handle);
	Handle = NULL;
}

void BSR::MultiProcessing::Process::Release()
{
	if (!Handle)
	{
		return;
	}

	CloseHandle(Handle);
	Handle = NULL;
}

int32_t BSR::MultiProcessing::Process::UpdateStatus()
{
	if (!Handle)
	{
		return _ReturnError;
	}

	int32_t _ReturnValue = _ReturnError;

	if (!GetExitCodeProcess(Handle, (DWORD*)(&_ReturnValue)))
	{
		CloseHandle(Handle);
		Handle = NULL;

		return _ReturnError;
	}

	if (_ReturnValue != _ReturnStillRunning)
	{
		CloseHandle(Handle);
		Handle = NULL;
	}

	return _ReturnValue;
}

const bool BSR::MultiProcessing::Process::CheckCreated() const
{
	return Handle != NULL;
}

const HANDLE BSR::MultiProcessing::Process::GetHandle() const
{
	return Handle;
}

void BSR::MultiProcessing::Process::operator= (Process&& _Other) noexcept
{
	Release();

	Handle = _Other.Handle;

	_Other.Handle = NULL;
}
