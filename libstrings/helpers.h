/*	libstrings
	
	A library for reading and writing STRINGS, ILSTRINGS and DLSTRINGS files.

    Copyright (C) 2012    WrinklyNinja

	This file is part of libstrings.

    libstrings is free software: you can redistribute 
	it and/or modify it under the terms of the GNU General Public License 
	as published by the Free Software Foundation, either version 3 of 
	the License, or (at your option) any later version.

    libstrings is distributed in the hope that it will 
	be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libstrings.  If not, see 
	<http://www.gnu.org/licenses/>.
*/

#ifndef LIBSTRINGS_HELPERS_H
#define LIBSTRINGS_HELPERS_H

#include <string>
#include <boost/unordered_map.hpp>
#include <boost/filesystem.hpp>

namespace libstrings {
	// std::string to null-terminated uint8_t string converter.
	uint8_t * ToUint8_tString(std::string str);

	// converts between encodings.
	class Transcoder {
	private:
		//0x81, 0x8D, 0x8F, 0x90, 0x9D in 1252 are undefined in UTF-8.
		boost::unordered_map<char, uint32_t> commonMap;  //1251/1252, UTF-8. 0-127, plus some more.
		boost::unordered_map<char, uint32_t> map1252toUtf8; //1252, UTF-8. 128-255, minus a few common characters.
		boost::unordered_map<uint32_t, char> utf8toEnc;
		boost::unordered_map<char, uint32_t> encToUtf8;
		uint32_t currentEncoding;
	public:
		Transcoder();
		void SetEncoding(const uint32_t inEncoding);
		uint32_t GetEncoding();

		std::string Utf8ToEnc(const std::string inString);
		std::string EncToUtf8(const std::string inString);
	};
}

#endif