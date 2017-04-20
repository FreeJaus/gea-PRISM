#pragma once

#include "Singleton.h"
#include <string>

class Crypto final : public Singleton<Crypto> {

private:

	friend class Singleton<Crypto>;
	
	Crypto() : Singleton<Crypto>() {}

	std::string ToHex(unsigned const char *str, int size);

	void ComputeMD5(std::string& buffer, const std::string& word);
	void ComputeSHA1(std::string& buffer, const std::string& word);
	void ComputeSHA256(std::string& buffer, const std::string& word);

public:

	void HashWord(std::string& buffer, const std::string& word, int nhash);

};