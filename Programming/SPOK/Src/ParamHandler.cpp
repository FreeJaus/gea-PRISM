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

#include "../Headers/ParamHandler.h"
#include <algorithm>

Param* ParamHandler::IsInList(const std::vector<Param*>& commands, const std::string& arg){
	Param *p = nullptr;
	auto it = commands.begin();
	while (it != commands.end() && p == nullptr){
		if ((*it)->GetCommand() == arg || (*it)->GetAlias() == arg)
			p = *it;
		it++;
	}
	return p;
}

bool ParamHandler::ParseArguments(const std::vector<std::string>& args, int* paramcount, bool *verbose, bool *version, std::string& dumpfile, 
	std::string& loadfile, std::string& savefile, std::string& charset, std::string& interval, std::string& hash, std::string& lastword){

	bool ret = true;
	std::vector<Param*> commands;

	Param *vParam = new Param("-v", "--verbose", false); commands.push_back(vParam);
	Param *verParam = new Param("-ver","--version", false); commands.push_back(verParam);
	Param *gParam = new Param("-g", "--generate", true); commands.push_back(gParam);
	Param *lParam = new Param("-l", "--load", true); commands.push_back(lParam);
	Param *iParam = new Param("-i", "--interval", true); commands.push_back(iParam);
	Param *cParam = new Param("-c", "--charset", true); commands.push_back(cParam);
	Param *sParam = new Param("-s", "--save", true); commands.push_back(sParam);
	Param *hParam = new Param("-h", "--hash", true); commands.push_back(hParam);
	Param *lwParam = new Param("-w", "--lastword", true); commands.push_back(lwParam);

	Param *curParam = nullptr, *c = nullptr;
	auto it = args.begin();

	while (it != args.end() && ret){
		c = IsInList(commands, *(it));
		if (c!= nullptr){
			if (curParam == nullptr){
				if (c->HasArg())
					curParam = c; //Expect command argument in next iteration
				if (!c->IsMarked()){
					c->SetMarked();
					(*paramcount)++;
				}else{
					ret = false; //Command repetition
				}
			}else{
				ret = false; //Expected command argument
			}
		}else{
			if (curParam != nullptr){
				curParam->SetArgument(*it);
				curParam = nullptr;
			}else{
				ret = false; //Command not in list
			}
		}

		it++;
	}

	*verbose = vParam->IsMarked(), *version = verParam->IsMarked();
	dumpfile = gParam->GetArgument(), loadfile = lParam->GetArgument(), savefile = lParam->GetArgument();
	charset = cParam->GetArgument(), interval = iParam->GetArgument(), hash = hParam->GetArgument(), lastword = lwParam->GetArgument();

	for (auto it = commands.begin(); it != commands.end(); it++)
		delete *it;

	return ret;
}