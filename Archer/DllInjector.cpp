#include "DllInjector.h"

DllInjector::DllInjector(std::string dll)
	: dll{ dll }
{
}

void DllInjector::Inject(std::string processName)
{
	DWORD pid = this->ProcessNameToPID(processName);
	std::cout << "[pid] " << pid << std::endl;

	HANDLE processHandle = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		pid
	);

	HMODULE dllHandle = GetModuleHandle("Kernel32");

	FARPROC loadLibraryAddress = GetProcAddress(
		dllHandle,
		"LoadLibraryA"
	);

	LPVOID dllPathAddress = VirtualAllocEx(
		processHandle,
		NULL,
		this->dll.size() + 1,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);

	BOOL hasSucceeded = WriteProcessMemory(
		processHandle,
		dllPathAddress,
		this->dll.c_str(),
		this->dll.size() + 1,
		NULL
	);

	std::cout << "[has succeeded] " << hasSucceeded << std::endl;

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

	return pid;
}