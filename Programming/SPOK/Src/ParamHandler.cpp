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
	std::string& loadfile, std::string& savefile, std::string& charset, std::string& interval, std::string& hash){

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
	charset = cParam->GetArgument(), interval = iParam->GetArgument(), hash = hParam->GetArgument();

	for (auto it = commands.begin(); it != commands.end(); it++)
		delete *it;

	return ret;
}