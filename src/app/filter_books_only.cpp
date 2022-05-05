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
    st_strings_handle sh_main;
    st_strings_handle sh_english;
    const string mode_name = "Skyrim";
//    const string mode_name = "Dawnguard";
//    const string mode_name = "Dragonborn";
//    const string mode_name = "HearthFires";
//    const string mode_name = "Update";
    const string path_english =
            "/home/hvt/Code/skyrim/data/SkyrimSE_Strings_English/Strings/" + mode_name + "_English.DLSTRINGS";
    const string path_main =
            "/home/hvt/Code/skyrim/data/SkyrimSE_Strings_Chinese/Strings/" + mode_name + "_chinese.DLSTRINGS";
    const string newPath =
            "/home/hvt/Code/skyrim/data/SkyrimSE_chinese_books_only/Strings/" + mode_name + "_english.DLSTRINGS";
    unsigned int ret;
    st_string_data * dataArr;
    size_t dataArrSize;

    libstrings::ofstream out(boost::filesystem::path("libstrings-tester.txt"));
    if (!out.good()){
        cout << "File could not be opened for reading.";
        return 1;
    }

    out << "Using path: " << path_main << " " << path_english << endl;

    out << "TESTING st_open(...)" << endl;
    ret = st_open(&sh_main, path_main.c_str(), "UTF-8"); // in skyrim AE, all of the default encodings are UTF-8
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_open(...) failed! Return code: " << ret << endl;
    else
        out << '\t' << "st_open(...) successful!" << endl;
    ret = st_open(&sh_english, path_english.c_str(), "UTF-8"); // in skyrim AE, all of the default encodings are UTF-8
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_open(...) failed! Return code: " << ret << endl;
    else
        out << '\t' << "st_open(...) successful!" << endl;

    out << "\n\n\n\n\nTESTING display all books:\n" << endl;
    ret = st_get_strings(sh_main, &dataArr, &dataArrSize);
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_get_strings(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_get_strings(...) successful! Number of strings: " << dataArrSize << endl;
        out << '\t' << "ID" << '\t' << "Book Content" << endl;
        // check if this item is a book
        for (size_t i=0; i < dataArrSize; i++) {
            if (string(dataArr[i].data).substr(0, 5) == "<font"
                || string(dataArr[i].data).substr(0, 4) == "<div"
                || string(dataArr[i].data).substr(0, 2) == "<p"
                || string(dataArr[i].data).substr(0, 3) == "<br"
                || string(dataArr[i].data).substr(0, 5) == "[page") {
                out << "\nBOOKS & LETTERS ================================================================================================================================\n";
                out << dataArr[i].id << ":\n";
                out << dataArr[i].data << endl;
                out << "\n\n";
            }
            else{
                out << "\nOther item ================================================================================================================================\n";
                out << dataArr[i].id << ":\n";
                out << dataArr[i].data << endl;
                out << "\n\n";

                // if ont a book, use native English to replace it
                char * str;
                ret = st_get_string(sh_english, dataArr[i].id, &str);
                if (ret != LIBSTRINGS_OK)
                    cout << '\t' << "st_get_string(...) failed! Return code: " << ret << " ID: " << dataArr[i].id << endl;
                else {
//                    cout << '\t' << "st_get_string(...) successful!"  << endl;
//                    cout << '\t' << "String fetched: ENG: " << str << ", CN: " << dataArr[i].data << endl;
                }

                ret = st_replace_string(sh_main, dataArr[i].id, str);
                if (ret != LIBSTRINGS_OK)
                    cout << '\t' << "st_replace_string(...) failed! Return code: " << ret << endl;
                else {
//                    cout << '\t' << "st_replace_string(...) successful!"  << endl;
                }

//                out << "TESTING st_remove_string(...)" << endl;
//                ret = st_remove_string(sh_main, dataArr[i].id);
//                if (ret != LIBSTRINGS_OK)
//                    cout << '\t' << "st_remove_string(...) failed! Return code: " << ret << endl;
//                else {
//                    cout << '\t' << "st_remove_string(...) successful!"  << endl;
//                }
            }
        }
    }

    // save to new strings file
    out << "TESTING st_save(...)" << endl;
    ret = st_save(sh_main, newPath.c_str(), "UTF-8");
    if (ret != LIBSTRINGS_OK)
        out << '\t' << "st_save(...) failed! Return code: " << ret << endl;
    else {
        out << '\t' << "st_save(...) successful!" << endl;
    }

    out << "TESTING st_close(...)" << endl;
    st_close(sh_main);
    st_close(sh_english);

    out.close();
    return 0;
}
