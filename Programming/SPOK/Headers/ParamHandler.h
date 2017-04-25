#pragma once

#include "Singleton.h"
#include "Param.h"
#include <string>
#include <vector>

class ParamHandler final : public Singleton<ParamHandler> {

private:

	friend class Singleton<ParamHandler>;
	
	ParamHandler() : Singleton<ParamHandler>() {}

	Param* IsInList(const std::vector<Param*>& commands, const std::string& arg);

public:

	bool ParseArguments(const std::vector<std::string>& args, int* paramcount, bool *verbose, bool *version, std::string& dumpfile, 
	std::string& loadfile, std::string& savefile, std::string& charset, std::string& interval, std::string& hash);

};