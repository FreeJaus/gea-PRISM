#pragma once

#include <regex>
#include "CharacterList.h"

class RegEx final : public Singleton<RegEx> {

	friend class Singleton <RegEx>;

private:

	vector<string> ReturnMatches(string data, string pattern, regex::flag_type flag);
	void ReplaceAll(string &word, string oldVal, string newVal);

	RegEx() : Singleton<RegEx>() {}

public:

	void ParseGuildStats(string data, CharacterList *pGuild);
	void ParseWorldStats(string data, CharacterList *pWorld);

};