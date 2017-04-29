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

#include "../Headers/FileHandler.h"
#include "../Headers/ParamHandler.h"
#include <iostream>

void FileHandler::OpenDumpFile(const std::string& filename){
	dumpfile.open(filename, std::ofstream::app | std::ofstream::binary);
}

void FileHandler::WriteStateFile(const std::string& filename, const std::string& params){
	std::ofstream statefile; 
	statefile.open(filename, std::ofstream::trunc | std::ofstream::binary);
	statefile << params;
	statefile.close();
	std::cout << "[+] Session state saved." << std::endl;
}

std::string FileHandler::ReadStateFile(const std::string& filename){
	std::string params;
	std::ifstream statefile;
	statefile.open(filename, std::ofstream::in | std::ofstream::binary);
	std::getline(statefile, params);
	statefile.close();
	return params;
}

long double FileHandler::GetFileSize(const std::string& filename){
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	return file.tellg();
}

//Multi Buffer approach
void FileHandler::LogFileMB(const std::string buffer, const std::string& filename, const std::string& params, int j){
	mwrite.lock(); 
	dumpfile << buffer;
	//WHEN BUFFER IS COPIED TO FILE, SAVE PARAMS
	if (!filename.empty()){
		auto it = buffer.end();
		std::string lastword(it -j - 1, it);
		WriteStateFile(filename, params + lastword);
	}
	//std::cout << "[+] Words saved to file." << std::endl;
	mwrite.unlock();
}

void FileHandler::LogFile(std::string& buffer, const std::string& filename, const std::string& params, int j){
	dumpfile << buffer;
	//WHEN BUFFER IS COPIED TO FILE, SAVE PARAMS
	if (!filename.empty()){
		auto it = buffer.end();
		std::string lastword(it -j - 1, it);
		WriteStateFile(filename, params + lastword);
	}
	//std::cout << "[+] Words saved to file." << std::endl;
	buffer = "";//PTR AS REF. -> FREE HERE
}