#include <Winhttp.h>

#pragma once

class HttpManager final : public Singleton<HttpManager> {

	friend class Singleton <HttpManager>;

private:

	enum SEARCH_TYPE
	{
		SEARCH_TYPE_GUILD = 0x0,
		SEARCH_TYPE_WORLD = 0x1,
		SEARCH_TYPE_CHARACTER = 0x2
	};

	const string URL_SITE = "http://www.tibia.com/";
	const string URL_GUILD = "/community/?subtopic=guilds&page=view&onlyshowonline=1&GuildName=";
	const string URL_WORLD = "/community/?subtopic=worlds&world=";
	const string URL_CHARACTER = "/community/?subtopic=characters&name=";

	HttpManager() : Singleton<HttpManager>() {}

	class HttpRequest final {

	private:

		const wchar_t* USER_AGENT = L"Mozilla/5.0 (Windows NT 6.3; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0";
		string keyWord, url;
		SEARCH_TYPE type;
		HINTERNET winHttp, httpConnect;

		static void GetRequestCallBack(LPVOID pVoid);

	public:

		HINTERNET httpRequest;

		HttpRequest(string _keyWord, string _url, SEARCH_TYPE _type);
		~HttpRequest() { }

		void GetRequest();
		void UpdateView(string html);

	};

public:

	void GetCharacterOnline(string charName);
	void GetGuildPlayersOnline(string guildName);
	void GetWorldStats(string worldName);

};