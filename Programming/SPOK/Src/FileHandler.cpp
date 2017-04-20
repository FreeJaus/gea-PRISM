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
	statefile >> params;
	statefile.close();
	return params;
}

long double FileHandler::GetFileSize(const std::string& filename){
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	return file.tellg();
}

void FileHandler::LogFile(const std::string& buffer, const std::string& filename, const std::string& params){
	mwrite.lock();
	dumpfile << buffer;
	std::cout << "[+] Words saved to file." << std::endl;
	//WHEN BUFFER IS COPIED TO FILE SAVE PARAMS
	if (!filename.empty())
		WriteStateFile(filename, params);
	mwrite.unlock();
}

void FileHandler::ParseStateFile(const std::string& filename, bool *verbose, std::string& dumpfile, std::string& strinterval, std::string& strhash, std::string& charset, std::string& lastword){
	char v = 0;
	std::string stateparams = ReadStateFile(filename);
	bool found = false;
	for (int i = 0; i < stateparams.size(); i++){
		if (stateparams[i] == '-'){
			v = stateparams[++i];
			if (stateparams[i] == 'v')
				*verbose = true;
		}else{
			if (v == 'v')
				*verbose = true;
			if (v == 'g')
				dumpfile += stateparams[i];
			if (v == 'c')
				charset += stateparams[i];
			if (v == 'i')
				strinterval += stateparams[i];
			if (v == 'h')
				strhash += stateparams[i];
			if (v == 'w')
				lastword += stateparams[i];
		}
	}
}