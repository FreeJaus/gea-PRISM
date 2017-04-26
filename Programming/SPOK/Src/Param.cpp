/*
Copyright 2017 Borja Gomez - kub0x@elhacker.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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