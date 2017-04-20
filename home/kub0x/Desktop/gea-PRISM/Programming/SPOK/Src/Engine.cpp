#include "../Headers/Engine.h"
#include "../Headers/FileHandler.h"
#include "../Headers/ParamHandler.h"
#include "../Headers/Crypto.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ratio>
#include <thread>

void Engine::ExecuteArgs(int argc, char **argv){
	std::string dummy, strinterval, strhash;
	bool repeat=false, nofile=true;

	if (ParamHandler::GetInstance()->ParseCondition(false, argc, argv, "-v", "--verbose", dummy, &repeat, &nofile))
		verbose=true;

	if (ParamHandler::GetInstance()->ParseCondition(false, argc, argv, "-ver", "--version", dummy, &repeat, &nofile))
		if (argc == 2){
			std::cout << VERSION << std::endl; 
			return;
		}else{
			std::cout << "Invalid list of parameters for -ver,--version." << std::endl;
			return;
		}

	if (ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-l", "--load", loadfile, &repeat, &nofile)){
		if (repeat || nofile) return;
		if (argc==3){
			std::cout << "Loading previous session state" << std::endl;
			std::string _charset;
			savefile = loadfile; //ENABLE SAVING
			FileHandler::GetInstance()->ParseStateFile(loadfile, &verbose, dumpfile, strinterval, strhash, _charset, lastword);
			if (verbose)
				saveparams = "-v";
			if (_charset.size())
				CHARSET = _charset; //OVERRIDE DEFAULT CHARSET

			if (!strinterval.empty())
				i = strinterval.at(0) - '0', j = strinterval.at(2) - '0';
			else
				strinterval = "1,5";
			
			if (!strhash.empty())
				nhash = strhash.at(0) - '0';
			else
				strhash = "0";

			saveparams += "-g" + dumpfile + "-c" + CHARSET + "-i" + strinterval + "-h" + strhash + "-w";

			std::cout << i << "," << j << "," << nhash << "," << dumpfile << "," << CHARSET << "," << lastword << std::endl;;
			BeginExecution();
			return; //DO NOT EVALUATE REST OF ARGS
		}else{
			std::cout << "Invalid list of parameters for -l,--load." << std::endl;
			return;
		}
	}

	if (!ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-g","--generate", dumpfile, &repeat, &nofile)){
		std::cout << "No valid Input!" << std::endl; //NO LOAD NO GEN -> EXIT
		return;
	}

	if (repeat || nofile) return;

	if (!ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-c","--charset", CHARSET, &repeat, &nofile))
		std::cout << "Using default charset." << std::endl;

	if (repeat || nofile) return;

	if (ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-i","--interval", strinterval, &repeat, &nofile)){
		if (repeat || nofile) return;
		i = strinterval.at(0) - '0', j = strinterval.at(2) - '0';
	}else{
		strinterval = "1,5";
		std::cout << "Using default interval <1,5>" << std::endl;
	}

	if (ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-h","--hash", strhash, &repeat, &nofile)){
		if (repeat || nofile) return;
		nhash = strhash.at(0) - '0';
	}else{
		strhash = "0";
	}

	if (ParamHandler::GetInstance()->ParseCondition(true, argc, argv, "-s","--save", savefile, &repeat, &nofile)){
		if (repeat || nofile) return;
		if (verbose)
			saveparams = "-v";
		saveparams += "-g" + dumpfile + "-c" + CHARSET + "-i" + strinterval + "-h" + strhash + "-w";
	}

	BeginExecution();

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
		node = new Node(value, valuepos, signaled, prev);
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
		for (auto it = nodes.begin(); it != nodes.end(); it++)
			str += (*it)->getValue();
		
		int ctr = 0;
		for (int k = i; k <= j; k++){
			if (!nodes.at(ctr)->IsSignaled()){
				substr = str.substr(ctr++,str.size());
				if (nhash)
					Crypto::GetInstance()->HashWord(buffer, substr, nhash);
				else
					buffer += substr + "\n";
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
	std::cout << "\n	S.P.O.K (Simple Production Of Keys) by kub0x@elhacker.net\n" << std::endl;
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