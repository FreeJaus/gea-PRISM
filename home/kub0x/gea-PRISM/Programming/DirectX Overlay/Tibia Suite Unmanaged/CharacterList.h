#pragma once

class Character {

private:

	bool promoted;
	string name, vocation;
	int lvl;

public:

	Character(string _name, string _lvl, string _vocation);
	~Character() {}

	string GetName() { return name; }
	string GetVocation() { return vocation; }
	unsigned short GetLevel() { return lvl; }
	bool IsPromoted() { return promoted; }

};


class CharacterList : public vector<Character*> {

protected:

	CharacterList() : vector<Character*>() {}

	static bool LevelComparator(Character *a, Character *b);

public:

	void SortByLevel();
	unsigned short GetPlayersCount() { return this->size(); }
	unsigned short GetPlayersCountByVoc(string vocation, bool promoted);
	unsigned short GetPromotedPlayersCount();
	unsigned short GetCharacterPos(string charName);
	unsigned short GetCharacterPosByVoc(string charName);
	unsigned short GetMaxLevel() { return this->at(0)->GetLevel(); }
	unsigned short GetMinLevel() { return this->at(this->size() -1)->GetLevel(); }

};

class World final : public CharacterList {

public:

	enum SERVER_LOCATION
	{
		SERVER_LOCATION_EUROPE = 0x0,
		SERVER_LOCATION_NORTH_AMERICA = 0x1
	};

	World(string _worldName, SERVER_LOCATION _loc);
	~World() {}

	string GetWorldName() { return worldName; }
	SERVER_LOCATION GetWorldLocation() { return loc; }

private:

	string worldName;
	SERVER_LOCATION loc;

};

class Guild final : public CharacterList {

private:

	string guildName;

public:

	Guild(string guildName) : CharacterList() {}
	~Guild() {}

	string GetGuildName() { return guildName; }

};