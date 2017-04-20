#pragma once

#include <vector>
#include <string>
#include <chrono>
#include "Singleton.h"
#include "Node.h"

class Engine final : public Singleton<Engine> {

private:

	friend class Singleton<Engine>;

	Engine() : Singleton<Engine>() {}

	const unsigned int BUFFSIZE = 400*1024*1024;
	const std::string VERSION = "S.P.O.K v1.0 April 2017";

	int i=1, j=5, nhash = 0;
	long double totalstorage = 0;
	bool verbose = false;

	std::vector<Node*> nodes;

	std::string CHARSET = "abcdefghijklmnopqrstuvwxyz", dumpfile, savefile, loadfile, buffer, saveparams, lastword;

	void ShowVerbose(std::chrono::high_resolution_clock::time_point& t1, long *words);
	void BeginExecution();
	void GenerateWords();
	void Permute();
	void FillNodeList();
	int GetLetterPos(char c);

public:

	void ExecuteArgs(int argc, char **argv);
	void PrintMenu();

};