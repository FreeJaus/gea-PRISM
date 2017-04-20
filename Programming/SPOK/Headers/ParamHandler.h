#pragma once

#include "Singleton.h"
#include <string>

class ParamHandler final : public Singleton<ParamHandler> {

private:

	friend class Singleton<ParamHandler>;
	
	ParamHandler() : Singleton<ParamHandler>() {}

	char* ParseArgs(char **begin, char **end, const std::string& option);
	bool ExistOption(char **begin, char **end, const std::string& option);

public:

	bool ParseCondition(bool parsevalue, int argc, char** argv, const std::string& option, const std::string& option1, std::string& value, bool *repeat, bool *nofile);

};