#include "../Headers/ParamHandler.h"
#include <iostream>
#include <algorithm>

char* ParamHandler::ParseArgs(char **begin, char **end, const std::string& option){
	char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool ParamHandler::ExistOption(char **begin, char **end, const std::string& option){
    return std::find(begin, end, option) != end;
}

bool ParamHandler::ParseCondition(bool parsevalue, int argc, char** argv, const std::string& option, const std::string& option1, std::string& value, bool *repeat, bool *nofile){
	bool present = false;
	char *retptr = nullptr;
	*repeat = false;
	*nofile = true;

	if (ExistOption(argv, argv+argc, option)){
		present = true;
		retptr = ParseArgs(argv, argv+argc, option);
	}
	if (ExistOption(argv, argv+argc, option1))
		if (!present){
			present = true;
			retptr = ParseArgs(argv, argv+argc, option1);
		}else{
			std::cout << "Parameter repetition detected for " <<  option1 << std::endl;
			*repeat = true;
			return true;
		}

	if (parsevalue)
		if (present)
			if (retptr){
				value = std::string(retptr);
				*nofile = false;
			}else{
				std::cout << "No value found for " << option1 << std::endl;
			}
		else
			*nofile = false;
	
	return present;

}