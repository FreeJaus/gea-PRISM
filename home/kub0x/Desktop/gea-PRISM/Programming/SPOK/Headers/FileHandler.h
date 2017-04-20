#pragma once

#include "Singleton.h"
#include <fstream>
#include <string>
#include <mutex>


class FileHandler final : public Singleton<FileHandler> {

private:

	friend class Singleton<FileHandler>;

	std::mutex mwrite;

	std::ofstream dumpfile;

	std::string ReadStateFile(const std::string& filename);

	void WriteStateFile(const std::string& filename, const std::string& params);

	FileHandler() : Singleton<FileHandler>() { }

public:

	long double GetFileSize(const std::string& filename);
	void OpenDumpFile(const std::string &filename);
	void ParseStateFile(const std::string& filename, bool *verbose, std::string& dumpfile, std::string& strinterval, std::string& strhash, std::string& charset, std::string& lastword);
	void LogFile(const std::string& buffer, const std::string& filename, const std::string& params);

};