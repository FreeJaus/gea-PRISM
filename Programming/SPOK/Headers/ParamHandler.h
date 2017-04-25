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