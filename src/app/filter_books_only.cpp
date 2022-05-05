/*  libstrings

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

#include "libstrings.h"
#include "streams.h"

#include <stdint.h>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;

int main() {
    st_strings_handle sh;
    const char * path = "/home/hvt/Code/skyrim/data/skyrimae_chinese_book/Strings/Skyrim_chinese.DLSTRINGS";
    const char * newPath = "/home/hvt/Code/skyrim/data/skyrimae_chinese_book/Strings/Skyrim_hvtese.DLSTRINGS";
//    const char * path = "/home/hvt/Code/skyrim/data/skyrimae_chinese_book/Strings/Dawnguard_chinese.DLSTRINGS";
//    const char * newPath = "/home/hvt/Code/skyrim/data/skyrimae_chinese_book/Strings/Dawnguard_hvtese.DLSTRINGS";
    const char * testMessage = "This is a test message.";
    unsigned int ret;
    st_string_data * dataArr;
    size_t dataArrSize;
    char * str;
    const char * error;
    uint32_t id;
    char ** stringArr;
    size_t stringArrSize;

    libstrings::ofstream out(boost::filesystem::path("libstrings-tester.txt"));
    if (!out.good()){
        cout << "File could not be opened for reading.";
        return 1;
    }

    out << "Using path: " << path << endl;

    out << "TESTING st_open(...)" << endl;
    ret = st_open(&sh, path, "Windows-1252");
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_open(...) failed! Return code: " << ret << endl;
    else
        out << '\t' << "st_open(...) successful!" << endl;

    out << "\n\n\n\n\nTESTING display all books:\n" << endl;
    ret = st_get_strings(sh, &dataArr, &dataArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_strings(...) successful! Number of strings: " << dataArrSize << endl;
        out << '\t' << "ID" << '\t' << "Book Content" << endl;
        for (size_t i=0; i < dataArrSize; i++) {
            if (string(dataArr[i].data).substr(0, 5) == "<font") {
                out << "\n================================================================================================================================\n";
                out << dataArr[i].id << ":\n";
                out << dataArr[i].data << endl;
                out << "\n\n";
            }
        }
    }

    out << "TESTING st_get_strings(...)" << endl;
    ret = st_get_strings(sh, &dataArr, &dataArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_strings(...) successful! Number of strings: " << dataArrSize << endl;
        out << '\t' << "ID" << '\t' << "String" << endl;
        for (size_t i=0; i < dataArrSize; i++) {
            out << '\t' << dataArr[i].id << '\t' << dataArr[i].data << endl;
        }
    }

    id = dataArr[500].id;

    out << "TESTING st_get_string(...)" << endl;
    ret = st_get_string(sh, id, &str);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_string(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_string(...) successful!"  << endl;
        out << '\t' << "String fetched: " << str << endl;
    }

    out << "TESTING st_get_unref_strings(...)" << endl;
    ret = st_get_unref_strings(sh, &stringArr, &stringArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_unref_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_unref_strings(...) successful! Number of strings: " << stringArrSize << endl;
        for (size_t i=0; i < stringArrSize; i++) {
            out << '\t' << stringArr[i] << endl;
        }
    }

    out << "TESTING st_replace_string(...)" << endl;
    ret = st_replace_string(sh, id, testMessage);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_replace_string(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_replace_string(...) successful!"  << endl;
    }

    out << "TESTING st_get_string(...)" << endl;
    ret = st_get_string(sh, id, &str);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_string(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_string(...) successful!"  << endl;
        out << '\t' << "String fetched: " << str << endl;
    }

    out << "TESTING st_add_string(...)" << endl;
    ret = st_add_string(sh, 500000, testMessage);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_add_string(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_add_string(...) successful!"  << endl;
    }

    out << "TESTING st_remove_string(...)" << endl;
    ret = st_remove_string(sh, 500000);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_remove_string(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_remove_string(...) successful!"  << endl;
    }

    out << "TESTING st_get_string(...)" << endl;
    ret = st_get_string(sh, 500000, &str);
    if (ret != LIBSTRINGS_OK) {
        out << '\t' << "st_get_string(...) failed! Return code: " << ret << endl;
        ret = st_get_error_message(&error);
        if (ret != LIBSTRINGS_OK)
            out << '\t' << "Failed to get error message." << endl;
        else
            out << '\t' << "Error message: " << error << endl;
    } else {
        out << '\t' << "st_get_string(...) successful!"  << endl;
        out << '\t' << "String fetched: " << str << endl;
    }

    out << "TESTING st_set_strings(...)" << endl;
    ret = st_set_strings(sh, dataArr, dataArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_set_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_set_strings(...) successful!" << endl;
    }

    out << "TESTING st_get_strings(...)" << endl;
    ret = st_get_strings(sh, &dataArr, &dataArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_strings(...) successful! Number of strings: " << dataArrSize << endl;
        out << '\t' << "ID" << '\t' << "String" << endl;
        for (size_t i=0; i < dataArrSize; i++) {
            out << '\t' << dataArr[i].id << '\t' << dataArr[i].data << endl;
        }
    }

    out << "TESTING st_save(...)" << endl;
    ret = st_save(sh, newPath, "Windows-1252");
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_save(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_save(...) successful!" << endl;
    }

    out << "TESTING st_close(...)" << endl;
    st_close(sh);

    out.close();
    return 0;
}
