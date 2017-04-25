#include "../Headers/Param.h"

Param::Param(std::string _command, std::string _alias, bool hasArg){
	this->command = _command;
	this->alias = _alias;
	this->marked = false;
	this->arg = hasArg;
}

std::string Param::GetCommand() { return this->command; }
std::string Param::GetAlias() { return this->alias; }
std::string Param::GetArgument() { return this->argument; }

bool Param::IsMarked() { return this->marked; }
bool Param::HasArg() { return this->arg; }

void Param::SetArgument(std::string _argument){
	this->argument = _argument;
}
void Param::SetMarked() { this->marked = true; }
