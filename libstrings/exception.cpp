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

#include "exception.h"
#include "libstrings.h"
#include <boost/format.hpp>

namespace libstrings {

	error::error() : errCode(0) {
		lastException = *this;
	}

	error::error(uint32_t code) : errCode(code) {
		lastException = *this;
	}

	error::error(uint32_t code, std::string subject) 
		: errCode(code), errSubject(subject) {
		lastException = *this;
	}

	error::error(uint32_t code, std::string subject, std::string detail) 
		: errCode(code), errSubject(subject), errDetail(detail) {
		lastException = *this;
	}

	std::string	error::what()			const {
		if (errCode == LIBSTRINGS_ERROR_INVALID_ARGS)
			return errSubject;
		else if (errCode == LIBSTRINGS_ERROR_NO_MEM)
			return "Memory allocation failed.";
		else if (errCode == LIBSTRINGS_ERROR_FILE_READ_FAIL)
			return (boost::format("\"%1%\" cannot be read!") % errSubject).str(); 
		else if (errCode == LIBSTRINGS_ERROR_FILE_WRITE_FAIL)
			return (boost::format("\"%1%\" cannot be written to!") % errSubject).str();
		else if (errCode == LIBSTRINGS_ERROR_BAD_STRING)
			return (boost::format("\"%1%\" cannot be converted from UTF-8 to \"%2%\".") % errSubject % errDetail).str();
		else
			return "No error.";
	}
	uint32_t	error::code()			const {
		return errCode;
	}

	error lastException = error();
}