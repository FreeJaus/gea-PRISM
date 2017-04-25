#include <iostream>
#include <exception>
#include "Headers/Engine.h"

int main (int argc, char *argv[]){
	std::ios::sync_with_stdio(false);
	if (argc > 1)
		try{
			Engine::GetInstance()->ExecuteArgs(argc,argv);
		}catch (std::exception& ex){
			std::cout << ex.what() << std::endl;
		}
	else
		Engine::GetInstance()->PrintMenu();
	return 0;
}