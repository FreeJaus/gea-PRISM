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


#include "../Headers/Engine.h"
#include "../Headers/FileHandler.h"
#include "../Headers/ParamHandler.h"
#include "../Headers/Crypto.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ratio>
#include <thread>
#include <sstream>

Engine::Engine() : Singleton<Engine>(){
	i=1, j=5, nhash = 0;
	totalstorage = 0;
	verbose = false;
	CHARSET = "abcdefghijklmnopqrstuvwxyz";
}

void Engine::ExecuteArgs(int argc, char **argv){
	int paramcount = 0;
	bool version = false;

	std::vector<std::string> args(argv, argv+argc); //split argv

	if (args.at(0) == "./spok") args.erase(args.begin());

	std::string _charset;

	 //Check if no repetition, no ilegal commands and exist commands with arguments
	if (ParamHandler::GetInstance()->ParseArguments(args, &paramcount, &verbose, &version, 
		dumpfile, loadfile, savefile, _charset, interval, hash, lastword)){

		if (version)
			if (paramcount == 1){
				std::cout << VERSION << std::endl;
				return;
			}else{
				PrintMenu();
				return; //version with more than 1 command considered ilegal
			}
		
		if (!loadfile.empty()){
			if (paramcount == 1){
				std::string params  = FileHandler::GetInstance()->ReadStateFile(loadfile);
				if (params.empty()) return;
				//split params by whitespace
				args = Split(params);
				//Check if load file params are correct
				if (!ParamHandler::GetInstance()->ParseArguments(args, &paramcount, &verbose, &version, 
					dumpfile, loadfile, savefile, _charset, interval, hash, lastword)){
					std::cout << "Failed to load state file!" << std::endl;
					return;
				}
			}else{
				PrintMenu(); //Load present but more than 1 command
				return;
			}
		}
		FillParams(_charset);
		BeginExecution();
	}else{
		PrintMenu(); //ilegal cmds, repetition or no args for cmds
	}

}

std::vector<std::string> Engine::Split(const std::string& str){
	std::vector<std::string> v;
	std::stringstream strstream(str);
	std::string buff;
	while (strstream >> buff)
		v.push_back(buff);
	return v;
}

void Engine::FillParams(const std::string& _charset){

	if (dumpfile.empty()){
			PrintMenu();
			return;
		}

	if (!_charset.empty())
		CHARSET = _charset;
	else
		std::cout << "Using default charset." << std::endl;

	if (!interval.empty()){
		std::string concat;
		for (auto it=interval.begin(); it != interval.end(); it++){
			if (*it == ','){
				i = std::stoi(concat);
				concat.clear();
			}else{
				concat += *it;
			}
		}
		j = std::stoi(concat);
		if (i > j || j <= 0 || i <= 0){
			PrintMenu();
			return;
		}
	}else{
		i = 1, j = 5;
	}

	if (!hash.empty())
			nhash = hash.at(0) - '0';
	else
		nhash = 0;

	if (!savefile.empty()){
		if (verbose)
			saveparams = "-v";
		saveparams += "-g" + dumpfile + "-c" + CHARSET + "-i" + interval + "-h" + hash + "-w";
	}

}

void Engine::BeginExecution(){
	if (!nhash){
		for (int k = i; k <= j; k++)
			totalstorage += std::pow(CHARSET.size(), k)*(k+1);
	}else{
		int m = 32;
		for (int k = i; k <= j; k++)
			totalstorage += std::pow(CHARSET.size(), k);
		if (nhash > 1)
			m+=40;
		if (nhash == 3)
			m+=64;
		m+=nhash;
		totalstorage *= m;
		std::cout << "Hashing with " << nhash << " algorithm(s)" << std::endl;
	}

	totalstorage /= (1024*1024);

	std::cout << "Total storage needed: " << totalstorage << " MB" << std::endl;

	FileHandler::GetInstance()->OpenDumpFile(dumpfile);

	buffer.reserve(BUFFSIZE);

	FillNodeList();

	Permute();

	FileHandler::GetInstance()->LogFile(buffer, std::string(), std::string()); //WRITE REMAINING WORDS OF BUFFER

	std::cout << "All words of length " << i << "," << j << " have been generated." << std::endl;
}

void Engine::FillNodeList(){
	bool signaled = false;
	char value = CHARSET[0];
	int valuepos = 0;
	Node *node = nullptr, *prev = nullptr;

	for (int k=0; k < j; k++){
		if (lastword.size()){
			value = lastword[k];
			valuepos = GetLetterPos(value);
		}
		node = new Node(value, CHARSET[0], valuepos, signaled, prev);
		nodes.push_back(node);
		prev = node;
		if (value != CHARSET[0])
			signaled = true;
	}

	if (!lastword.size()) //AVOIDS REPEATING LAST WORD
		GenerateWords(); //WRITE FIRST WORDS

}

int Engine::GetLetterPos(char c){
	int pos = 0, ctr = 0;
	while (ctr < CHARSET.size() && !pos){
		if (CHARSET[ctr] == c)
			pos = ctr;
		ctr++;
	}
	return pos;
}

void Engine::Permute(){
	Node *node = nodes.back();
	long words = 0;
	auto t1 = std::chrono::high_resolution_clock::now();
	while(node != nullptr){
		if (node->getValuePos() < CHARSET.size()-1){
			node->Permute(CHARSET.at(node->getValuePos() + 1));
			node = nodes.back();
			if (verbose){
				words++;
				ShowVerbose(t1, &words);
			}

			GenerateWords();
		}else{
			node->Reset();
			node = node->Prev();
		}
	}
}

void Engine::ShowVerbose(std::chrono::high_resolution_clock::time_point& t1, long *words){
	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	if (ms.count() > 60*1000){
		long double curstorage = FileHandler::GetInstance()->GetFileSize(dumpfile);
		std::cout << "Words per minute: " << *words << "\nWords per second: " << *words/60 << std::endl;
		std::cout << "Current storage: " << curstorage/(1024*1024) << "/" << totalstorage << " MB" << std::endl;
		t1 = std::chrono::high_resolution_clock::now();
		*words = 0;
	}
}

void Engine::GenerateWords(){
		std::string str, substr;
		for (auto it = nodes.begin(); it != nodes.end(); ++it)
			str += (*it)->getValue();
		buffer.append(str);
		buffer += "\n"; //SAVE 1 ITERATION
		int ctr = 1;
		for (int k = i; k <= j-1; k++){
			if (!nodes.at(ctr)->IsSignaled()){
				substr = str.substr(ctr++,str.size());
				if (nhash){
					Crypto::GetInstance()->HashWord(buffer, substr, nhash);
				}else{
					buffer.append(substr);
					buffer += "\n";
				}
			}
		}
		if (buffer.size() > BUFFSIZE-50){ //BUFF > 400 MB
			std::string _buffer(buffer);
			buffer = "";
			std::thread t(&FileHandler::LogFile, FileHandler::GetInstance(), _buffer, savefile, saveparams + str);
			t.detach();
		}
}

void Engine::PrintMenu(){
	std::cout << "\n	S.P.O.K (Simple Production Of Keys) Copyright (C) 2017 Borja Gomez - kub0x@elhacker.net\n" << std::endl;
	std::cout << "usage: spok [options] command [ARGUMENTS]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "	-ver, --version	(show version)" << std::endl;
	std::cout << "	-v, --verbose	(show advanced console log)" << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "	-g [FILE], --generate [FILE]		(generate dictionary in specified FILE)" << std::endl;
	std::cout << "	-l [SAVEFILE], --load [SAVEFILE]	(load previous session state)" << std::endl; 
	std::cout << "Arguments:" << std::endl;
	std::cout << "	-c [CHARSET], --charset [CHARSET]	(use specified charset, default is: 'abcdefghijklmnopqrstuwvxyz')" << std::endl;
	std::cout << "	-i <min,max>, --interval <min,max>	(generate all words of length min & max, default is: <0,5>)" <<std::endl;
	std::cout << "	-h <number>, --hash <number>		(hash output words into FILE, algorithms: MD5, SHA-1, SHA-256)" << std::endl;
	std::cout << "	-s [SAVEFILE], --save [SAVEFILE]	(save current session state in SAVEFILE)" << std::endl;
	std::cout << "\n Example: spok --generate words.txt --save save.sav --interval 4,8 --charset '01234567890ABCDEF' --hash 3" << std::endl;
}