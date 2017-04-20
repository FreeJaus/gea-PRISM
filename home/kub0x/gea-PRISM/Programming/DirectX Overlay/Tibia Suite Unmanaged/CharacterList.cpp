#include "stdafx.h"
#include <vector>
#include <algorithm>
#include "CharacterList.h"

Character::Character(string _name, string _lvl, string _vocation){
	promoted = false;
	name = _name;
	lvl = atoi(_lvl.c_str());
	vocation = _vocation;
	if (vocation.find("Elite") != -1 || vocation.find("Royal") != -1 ||
		vocation.find("Elder") != -1 || vocation.find("Master") != -1)
		promoted = true;
}

World::World(string _worldName, SERVER_LOCATION _loc) : CharacterList() {
	worldName = _worldName;
	loc = _loc;
}

void CharacterList::SortByLevel() { sort(this->begin(), this->end(), LevelComparator); }

bool CharacterList::LevelComparator(Character *a, Character *b) { return a->GetLevel() > b->GetLevel(); }

unsigned short CharacterList::GetCharacterPos(string charName){
	unsigned short pos = 1;
	bool found = false;
	auto itr = this->begin();
	auto itr_end = this->end();
	while (!found && itr != itr_end){
		if(!strcmp(charName.c_str(), (*itr)->GetName().c_str()))
			found = true;
		itr++;
		pos++;
	}
	if (itr == itr_end) pos = 0;
	return pos;
}

unsigned short CharacterList::GetCharacterPosByVoc(string charName){
	unsigned short pos = GetCharacterPos(charName);
	if (!pos) return 0;
	bool found = false;
	Character *player = this->at(pos-1);
	string charVoc = player->GetVocation();
	if (player->IsPromoted()) 
		charVoc = charVoc.substr(charVoc.find(" ") + 1);
	pos = 1;
	auto itr = this->begin();
	auto itr_end = this->end();
	while (!found && itr != itr_end){
		if (!strcmp(charName.c_str(), (*itr)->GetName().c_str()))
			found = true;
		if ((*itr)->GetVocation().find(charVoc) != string::npos)
			pos++;
		itr++;
	}
	return pos;
}

unsigned short CharacterList::GetPlayersCountByVoc(string vocation, bool promoted){
	unsigned short count = 0;
	for (auto itr = this->begin(); itr != this->end(); ++itr)
		if ((*itr)->GetVocation().find(vocation) != string::npos && (*itr)->IsPromoted() == promoted)
			count++;
	return count;
}

unsigned short CharacterList::GetPromotedPlayersCount(){
	unsigned short count = 0;
	for (auto itr = this->begin(); itr != this->end(); ++itr)
		if ((*itr)->IsPromoted())
			count++;
	return count;
}