#include "stdafx.h"
#include "Engine.h"
#include "d3dHook.h"
#include <Psapi.h>
#include <TlHelp32.h>

void Engine::Run(){
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RunCallBack, 0, 0, 0);
}

/* TODO: Check SHA-256 Tibia Signature!!! Only official client is allowed */

void Engine::RunCallBack(){
	Injector::getInstance()->LoadDLL();
}

bool Engine::Injector::IsInjected(){
	bool injected = false;
	wchar_t buff[100];
	DWORD dwLen = GetModuleFileNameW(GetModuleHandleW(NULL), buff, 100);
	wchar_t *image = new wchar_t[dwLen+1];
	lstrcpynW(image, buff, (int)dwLen+1);
	if (wcsstr(image, L"WildStar32.exe"))
		injected = true;
	delete[] image;
	return injected;
}

DWORD Engine::Injector::GetProcessIdByExeName(LPWSTR szExeName){
	HANDLE        hSnapProcess = NULL;
	PROCESSENTRY32  pe32 = { 0 };

	if (!(hSnapProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)))
		return 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnapProcess, &pe32))
	{
		do
		{
			if (lstrcmpW(pe32.szExeFile, szExeName) == 0)
			{
				CloseHandle(hSnapProcess);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnapProcess, &pe32));
	}
	CloseHandle(hSnapProcess);
	return 0;
}

void Engine::Injector::LoadDLL(){
	if (IsInjected()){
		d3dHook::getInstance()->Attach();
	}
	else{
		char dll[] = "C:\\Users\\kub0x\\Documents\\Visual Studio 2013\\Projects\\Tibia Suite Managed\\Release\\tsuite.dll";
		//Allocate VM on target
		//Write DLL Path to VM on target
		//CreateRemoteThread that executes LoadLibrary using VM allocated memory as argument
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIdByExeName(L"WildStar32.exe"));
		void *alloc = VirtualAllocEx(hProcess, 0, sizeof(dll), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		DWORD dwBytes = 0;
		WriteProcessMemory(hProcess, alloc, (LPCVOID)dll, sizeof(dll), &dwBytes);
		CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, alloc, 0, 0);
		CloseHandle(hProcess);
	}
}

void Engine::Injector::UnloadDLL(){
	if (!IsInjected()) return;
}