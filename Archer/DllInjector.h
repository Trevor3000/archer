#pragma once
#include "windows.h"
#include "tlhelp32.h"
#include <string>
#include <iostream>

class DllInjector
{
public:
	DllInjector(std::string dll);
	~DllInjector() = default;

	void Inject(std::string processName);
private:
	std::string dll;

	DWORD ProcessNameToPID(std::string processName);
};

