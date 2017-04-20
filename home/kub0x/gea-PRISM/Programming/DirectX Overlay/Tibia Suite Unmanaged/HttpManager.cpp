#include "stdafx.h"
#include <sstream>
#include <vector>
#include "HttpManager.h"
#include "RegEx.h"

#pragma comment(lib, "Winhttp.lib")

void HttpManager::GetCharacterOnline(string charName){
	HttpRequest *http = new HttpRequest(charName, URL_CHARACTER + charName, SEARCH_TYPE_CHARACTER);
	http->GetRequest();
	//delete http;
}
void HttpManager::GetGuildPlayersOnline(string guildName) {
	HttpRequest *http = new HttpRequest(guildName, URL_GUILD + guildName, SEARCH_TYPE_GUILD);
	http->GetRequest();
	//delete http;
}
void HttpManager::GetWorldStats(string worldName) {
	HttpRequest *http = new HttpRequest(worldName, URL_WORLD + worldName, SEARCH_TYPE_WORLD);
	http->GetRequest();
	//delete http;
}

HttpManager::HttpRequest::HttpRequest(string _keyWord, string _url, SEARCH_TYPE _type){
	keyWord = _keyWord;
	url = _url;
	type = _type;
	winHttp = NULL;
	httpConnect = NULL;
	httpRequest = NULL;
	winHttp = WinHttpOpen(
		USER_AGENT,
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0
	);
}

void HttpManager::HttpRequest::GetRequest(){
	if (winHttp)
		httpConnect = WinHttpConnect(winHttp, L"www.tibia.com", INTERNET_DEFAULT_HTTP_PORT, 0);
	if (httpConnect)
		httpRequest = WinHttpOpenRequest(httpConnect, L"GET", wstring(url.begin(), url.end()).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH);
	if (httpRequest)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetRequestCallBack, (LPVOID)this, 0, 0);
}

void HttpManager::HttpRequest::GetRequestCallBack(LPVOID pVoid){
	HttpRequest *obj = static_cast<HttpRequest*>(pVoid);
	HINTERNET req = obj->httpRequest;
	if (WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
		if (WinHttpReceiveResponse(req, NULL)){
			string html;
			char *buff;
			DWORD dwDataLen = 0, dwReadBytes = 0;
			do{
				dwDataLen = 0;
				dwReadBytes = 0;
				if (!WinHttpQueryDataAvailable(req, &dwDataLen)) return;
				buff = new char[dwDataLen+1]();
				ZeroMemory(buff, dwDataLen + 1);
				if (!WinHttpReadData(req, (LPVOID)buff, dwDataLen, &dwReadBytes)) return;
				html.append(buff);
				delete [] buff;
			} while (dwDataLen > 0);
			obj->UpdateView(html);
		}
}

void HttpManager::HttpRequest::UpdateView(string html){
	WinHttpCloseHandle(httpRequest);
	WinHttpCloseHandle(httpConnect);
	WinHttpCloseHandle(winHttp);
	CharacterList *charLst = nullptr;
	switch (type)
	{
		case SEARCH_TYPE_CHARACTER: break;
		case SEARCH_TYPE_GUILD:{
			charLst = new Guild(keyWord);
			RegEx::getInstance()->ParseGuildStats(html, charLst);
			MessageBoxA(0, to_string(charLst->GetPlayersCount()).c_str(), "", 0);
		}break;
		case SEARCH_TYPE_WORLD:{
			World::SERVER_LOCATION loc = World::SERVER_LOCATION_NORTH_AMERICA;
			if (html.find("Europe") != string::npos)
				loc = World::SERVER_LOCATION_EUROPE;
			charLst = new World(keyWord, loc);
			RegEx::getInstance()->ParseWorldStats(html, charLst);
			charLst->SortByLevel();
			//MessageBoxA(0, to_string(charLst->GetPlayersCount()).c_str(), "", 0);
			//MessageBoxA(0, to_string(charLst->GetPromotedPlayersCount()).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetCharacterPos("Guaschibo")).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetCharacterPosByVoc("Guaschibo")).c_str(), "", 0);
			/*MessageBoxA(0, to_string(charLst->GetMaxLevel()).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetMinLevel()).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetPlayersCountByVoc("Druid", true)).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetPlayersCountByVoc("Druid", false)).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetPlayersCountByVoc("Sorcerer", true)).c_str(), "", 0);
			MessageBoxA(0, to_string(charLst->GetPlayersCountByVoc("Sorcerer", false)).c_str(), "", 0);*/
		}break;
	}
}