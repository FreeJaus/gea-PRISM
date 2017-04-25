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
	const std::string VERSION = "S.P.O.K v1.0.1 April 2017";

	int i=1, j=5, nhash = 0;
	long double totalstorage = 0;
	bool verbose = false;

	std::vector<Node*> nodes;

	std::string CHARSET = "abcdefghijklmnopqrstuvwxyz", dumpfile, savefile, loadfile, buffer, saveparams, lastword, interval, hash;

	void ShowVerbose(std::chrono::high_resolution_clock::time_point& t1, long *words);
	void BeginExecution();
	void GenerateWords();
	void Permute();
	void FillNodeList();
	void FillParams(const std::string& _charset);
	int GetLetterPos(char c);
	std::vector<std::string> Split(const std::string& str);

public:

	void ExecuteArgs(int argc, char **argv);
	void PrintMenu();

};