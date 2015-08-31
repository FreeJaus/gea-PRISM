// WinUpdate Removal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <wuapi.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#pragma comment(lib, "Wuguid.lib")

using namespace std;

void UninstallUpdate(wchar_t *params){
	SHELLEXECUTEINFOW info = { 0 };

	info.cbSize = sizeof(info);
	info.lpVerb = L"runas";
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpFile = L"C:\\Windows\\System32\\wusa.exe";
	info.lpDirectory = L"C:\\Windows\\System32\\";
	info.lpParameters = params;
	info.nShow = SW_SHOW;
	if (ShellExecuteExW(&info) && info.hProcess){
		WaitForSingleObject(info.hProcess, INFINITE);
		CloseHandle(info.hProcess);
	}
}

vector<wstring> GetMaliciousKBs(){
	vector<wstring> kbs;
	std::ifstream file("C:\\Users\\kub0x\\Documents\\Visual Studio 2013\\Projects\\WinUpdate Removal\\Release\\kbs.txt");
	std::string str;
	while (std::getline(file, str))
		kbs.push_back(wstring(str.begin(), str.end()));
	return kbs;
}

IUpdateCollection *GetUninstallList(IUpdateSearcher *iSearch){
	IUpdateCollection *retUpdates = nullptr;
	_GUID clsid = { 0 };
	CLSIDFromProgID(OLESTR("Microsoft.Update.UpdateColl"), &clsid);
	if (SUCCEEDED(CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&retUpdates)))){
		ISearchResult *res = nullptr;
		BSTR criteria = SysAllocString(L"(IsInstalled = 1)");
		if (SUCCEEDED(iSearch->Search(criteria, &res))){
			IUpdateCollection *updates = nullptr;
			long lcount = 0, lindex = 0;
			IUpdate *update = nullptr;
			BSTR kb = nullptr;
			res->get_Updates(&updates);
			updates->get_Count(&lcount);
			vector<wstring> kbs = GetMaliciousKBs();
			for (long i = 0; i < lcount; i++){
				updates->get_Item(i, &update);
				IStringCollection *c = nullptr;
				update->get_KBArticleIDs(&c);
				c->get_Item(0, &kb);
				for (auto itr = kbs.begin(); itr != kbs.end(); ++itr){
					const wchar_t *wcur = (*itr).c_str();
					if (!lstrcmpW(kb, wcur)){
						wcout << L"Malicious update found: KB" << kb << endl;
						retUpdates->Add(update, &lindex);
					}
					SysFreeString(kb);
				}
				c->Release();
			}
			SysFreeString(criteria);
			res->Release();
			updates->Release();
		}
	}
	return retUpdates;
}

void UninstallUpdates(IUpdateCollection *updates){
	long lcount = 0;
	updates->get_Count(&lcount);
	(lcount > 0) ? wcout << L"Uninstalling " << to_wstring(lcount) << L" updates. Please wait..." << endl : wcout << "No malicious updates found" << endl;
	BSTR kb;
	IUpdate *update = nullptr;
	for (long i = 0; i < lcount; i++){
		updates->get_Item(i, &update);
		IStringCollection *c = nullptr;
		update->get_KBArticleIDs(&c);
		c->get_Item(0, &kb);
		wstring wkb(kb);
		wchar_t *buf = new wchar_t[50];
		wsprintfW(buf, L"/kb:%s% /uninstall", wkb.c_str());
		SysFreeString(kb);
		c->Release();
		update->Release();
		UninstallUpdate(buf);
		delete[]buf;
	}
}

int _tmain(int argc, _TCHAR* argv[]){
	wcout << L"** Malicious update removal by kub0x (kub0x@elhacker.net)**" << endl;
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))){
		IUpdateSession *iSession = nullptr;
		_GUID clsid = { 0 };
		CLSIDFromProgID(OLESTR("Microsoft.Update.Session.1"), &clsid); //CLSID From Reg -> HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{4CB43D7F-7EEE-4906-8698-60DA1C38F2FE}
		if (SUCCEEDED(CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&iSession)))){
			IUpdateSearcher *iSearch = nullptr;
			if (SUCCEEDED(iSession->CreateUpdateSearcher(&iSearch))){
				wcout << L"Searching for malicious updates..." << endl;
				IUpdateCollection *updates = GetUninstallList(iSearch);
				iSearch->Release();
				iSession->Release();
				UninstallUpdates(updates);
				updates->Release();
			}
		}
	}
	CoUninitialize();
	wcout << L"Thanks for using the software." << endl;
	wcout << L"Remember to checkout the project periodically to receive new updates" << endl;
	wcout << L"Press any key to exit..." << endl;
	getchar();
}