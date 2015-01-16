// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
//#include "HttpManager.h"
//#include "RegEx.h"
#include "Engine.h"

extern "C"{
	__declspec(dllexport) void __stdcall EntryPoint(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow){
		//HttpManager::getInstance()->GetWorldStats("Calva");
		Sleep(10000);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Engine::getInstance()->Run();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

