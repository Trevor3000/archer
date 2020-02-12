#include "DllInjector.h"

DllInjector::DllInjector(std::string dll)
	: dll{ dll }
{
}

void DllInjector::Inject(std::string processName)
{
	DWORD pid = this->ProcessNameToPID(processName);
	HANDLE processHandle = this->GetProcessHandle(pid);

	FARPROC loadLibraryAddress = this->GetLoadLibraryAddress();
	LPVOID dllPathAddress = this->WriteDllPathToProcessMemory(processHandle);

	CreateRemoteThread(
		processHandle,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) loadLibraryAddress,
		dllPathAddress,
		0,
		NULL
	);

	CloseHandle(processHandle);
}

DWORD DllInjector::ProcessNameToPID(std::string processName)
{
	DWORD pid = 0;

	HANDLE processSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	do {
		if (processName == entry.szExeFile) {
			pid = entry.th32ProcessID;
		}
	} while (Process32Next(processSnapshotHandle, &entry) && pid == 0);

	CloseHandle(processSnapshotHandle);

	if (pid == 0) {
		throw std::exception("Invalid process name.");
	}

	return pid;
}

HANDLE DllInjector::GetProcessHandle(DWORD pid)
{
	HANDLE processHandle = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		pid
	);

	if (!processHandle) {
		throw std::exception("Could not open process.");
	}

	return processHandle;
}

FARPROC DllInjector::GetLoadLibraryAddress()
{
	HMODULE kernel32Handle = GetModuleHandle("Kernel32");

	if (!kernel32Handle) {
		throw std::exception("Could not open Kernel32.dll.");
	}

	FARPROC loadLibraryAddress = GetProcAddress(
		kernel32Handle,
		"LoadLibraryA"
	);

	if (!loadLibraryAddress) {
		throw std::exception("Could not get the address of LoadLibrary.");
	}

	return loadLibraryAddress;
}

LPVOID DllInjector::WriteDllPathToProcessMemory(HANDLE processHandle)
{
	LPVOID dllPathAddress = VirtualAllocEx(
		processHandle,
		NULL,
		this->dll.size() + 1,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);

	if (!dllPathAddress) {
		throw std::exception("Could not allocate memory for the dll path.");
	}

	BOOL hasSucceeded = WriteProcessMemory(
		processHandle,
		dllPathAddress,
		this->dll.c_str(),
		this->dll.size() + 1,
		NULL
	);

	if (!hasSucceeded) {
		throw std::exception("Could not write the dll path into process' memory.");
	}

	return dllPathAddress;
}