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
#include <string.h>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <iostream>

std::string Crypto::ToHex(unsigned const char *str, int size){
	std::ostringstream stream;
	stream << std::hex << std::setfill('0');
	for (int k = 0; k < size; k++)
		stream << std::setw(2) << (unsigned int)str[k];
	return stream.str().c_str();
}

void Crypto::ComputeMD5(char *buffer, const char *word){
	unsigned char omd5[MD5_DIGEST_LENGTH];
	MD5((unsigned const char*) word, strlen(word), omd5);

	char md5hash[32];
	std::string s = ToHex((const unsigned char*)omd5, MD5_DIGEST_LENGTH);
	strcpy(md5hash, s.c_str());
	int size = 32, p = 0;
	for (int i = *bfpos; i < *bfpos + size; i++){
		buffer[i] = md5hash[p];
		p++;
	} //32 40 64
	*bfpos +=size;
	buffer[(*bfpos)++]='\n';
}

void Crypto::ComputeSHA1(char *buffer, const char *word){
	unsigned char osha1[SHA_DIGEST_LENGTH];
	SHA1((unsigned const char*) word, strlen(word), osha1);

	char sha1hash[40];
	std::string s = ToHex((const unsigned char*)osha1, SHA_DIGEST_LENGTH);
	strcpy(sha1hash, s.c_str());
	int size = 40, p = 0;
	for (int i = *bfpos; i < *bfpos + size; i++){
		buffer[i] = sha1hash[p];
		p++;
	}
	*bfpos +=size;
	buffer[(*bfpos)++]='\n';
}

void Crypto::ComputeSHA256(char *buffer, const char *word){
	unsigned char osha256[SHA256_DIGEST_LENGTH];
	SHA256((unsigned const char*)word, strlen(word), osha256);

	char sha256hash[64];
	std::string s = ToHex((const unsigned char*)osha256, SHA256_DIGEST_LENGTH);
	strcpy(sha256hash, s.c_str());
	int size = 64, p = 0;
	for (int i = *bfpos; i < *bfpos + size; i++){
		buffer[i] = sha256hash[p];
		p++;
	}
	*bfpos +=size;
	buffer[(*bfpos)++]='\n';
}

void Crypto::HashWord(char *buffer, const char *word, int nhash, int *_bfpos){
	bfpos = _bfpos;
	ComputeMD5(buffer, word);
	if (nhash > 1)
		ComputeSHA1(buffer, word);
	if (nhash == 3)
		ComputeSHA256(buffer, word);
}