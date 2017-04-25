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

#include "../Headers/Crypto.h"
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <openssl/sha.h>

std::string Crypto::ToHex(unsigned const char *str, int size){
	std::ostringstream stream;
	stream << std::hex << std::setfill('0');
	for (int k = 0; k < size; k++)
		stream << std::setw(2) << (unsigned int)str[k];
	return stream.str();
}

void Crypto::ComputeMD5(std::string& buffer, const std::string& word){
	unsigned char omd5[MD5_DIGEST_LENGTH];
	MD5((unsigned const char*) word.c_str(), word.size(), omd5);
	buffer.append(ToHex(omd5, MD5_DIGEST_LENGTH));
	buffer += "\n";
}

void Crypto::ComputeSHA1(std::string& buffer, const std::string& word){
	unsigned char osha1[SHA_DIGEST_LENGTH];
	SHA1((unsigned const char*) word.c_str(), word.size(), osha1);
	buffer.append(ToHex(osha1, SHA_DIGEST_LENGTH));
	buffer += "\n";
}

void Crypto::ComputeSHA256(std::string& buffer, const std::string& word){
	unsigned char osha256[SHA256_DIGEST_LENGTH];
	SHA256((unsigned const char*)word.c_str(), word.size(), osha256);
	buffer.append(ToHex(osha256, SHA256_DIGEST_LENGTH));
	buffer += "\n";
}

void Crypto::HashWord(std::string& buffer, const std::string& word, int nhash){
	ComputeMD5(buffer, word);
	if (nhash > 1)
		ComputeSHA1(buffer, word);
	if (nhash == 3)
		ComputeSHA256(buffer, word);
}