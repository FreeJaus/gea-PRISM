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
