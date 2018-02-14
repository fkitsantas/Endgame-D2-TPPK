#include "Houdini.h"

DWORD DriverCreateThread(LPVOID lpAddress, LPVOID lpParam)
{
	DWORD dwThreadId;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)lpAddress, lpParam, NULL, &dwThreadId);

	return dwThreadId;
}
VOID DriverDestroyThread(DWORD dwThreadId)
{
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);

	TerminateThread((HANDLE)hThread, 0);

	CloseHandle(hThread);
}