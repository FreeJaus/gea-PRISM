#pragma once

#include <string>

class Param {

private:

	std::string command, alias;
	std::string argument;
	bool marked, arg;

public:

	Param(std::string _command, std::string _alias, bool hasArg);

	std::string GetCommand();
	std::string GetAlias();
	bool IsMarked();
	bool HasArg();

	void SetArgument(std::string _argument);
	std::string GetArgument();
	void SetMarked();

};
