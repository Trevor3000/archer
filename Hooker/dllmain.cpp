// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	FILE *file = fopen("D:\\temp.txt", "a");

	if (!file)
		return FALSE;

	char filename[256];
	GetModuleFileNameA(NULL, filename, 256);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		fprintf(file, "Hi! I was attached to %s.\n", filename);
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		fprintf(file, "Hi! I was detached from %s.\n", filename);
		break;
    }

	fclose(file);

    return TRUE;
}

