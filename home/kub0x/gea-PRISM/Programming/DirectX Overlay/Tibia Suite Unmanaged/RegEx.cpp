#include "stdafx.h"
#include <vector>
#include "RegEx.h"

void RegEx::ParseGuildStats(string data, CharacterList *pGuild){
	data = data.substr(0, data.find("Invi"));
	ReplaceAll(data, "group</A>", "");
	vector<string>  vNames = ReturnMatches(data, "([a-z-A-Z;061#&]*)(<[/]A>)", regex::ECMAScript),
					vVocs = ReturnMatches(data, "<TD>.*knight*.*|<TD>.*paladin*.*|<TD>.*sorcerer*.*|<TD>.*druid*.*", regex::icase),
					vLevels = ReturnMatches(data, "<TD>\\d.*", regex::icase);
	for (unsigned int i = 0; i < vNames.size(); i++)
		pGuild->push_back(new Character(vNames.at(i), vLevels.at(i), vVocs.at(i)));
}
void RegEx::ParseWorldStats(string data, CharacterList *pWorld){
	data = data.substr(data.find("<a name=\"A\""));
	vector<string>  vNames = ReturnMatches(data, "([a-z-A-Z;061#&]*)(<[/]a>)(<[/]td>)|([a-z-A-Z']*)(<[/]a>)(<[/]td)>", regex::ECMAScript),
					vVocs = ReturnMatches(data, "([a-z-A-Z;061#&]*)(<[/]td><[/]tr>)", regex::icase),
					vLevels = ReturnMatches(data, "[\\d]*(<[/]td><td style=\"width:20%)", regex::icase);
	for (unsigned int i = 0; i < vNames.size(); i++)
		pWorld->push_back(new Character(vNames.at(i), vLevels.at(i), vVocs.at(i)));
}

vector<string> RegEx::ReturnMatches(string data, string pattern, regex::flag_type flag){
	vector<string> vMatches;
	regex r(pattern, flag);
	auto it_start = sregex_iterator(data.begin(), data.end(), r);
	auto it_end = sregex_iterator();
	for (auto it = it_start; it != it_end; ++it){
		smatch strmatch = *it;
		string match = strmatch.str();
		ReplaceAll(match, "<TD>", ""); ReplaceAll(match, "</A>", "");
		ReplaceAll(match, "</a>", ""); ReplaceAll(match, "</TD>", "");
		ReplaceAll(match, "</td>", ""); ReplaceAll(match, "</tr>", "");
		ReplaceAll(match, "&#160;", " "); ReplaceAll(match, "<td style=\"width:20%", "");
		vMatches.push_back(match);
	}
	return vMatches;
}

void RegEx::ReplaceAll(string &word, string oldVal, string newVal){
	size_t start_pos = 0;
	while ((start_pos = word.find(oldVal, start_pos)) != std::string::npos) {
		word.replace(start_pos, oldVal.length(), newVal);
		//start_pos += "".length(); // Handles case where 'to' is a substring of 'from'
	}
}