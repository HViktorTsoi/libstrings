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

#include "helpers.h"
#include "libstrings.h"
#include "exception.h"
#include <sstream>
#include <source/utf8.h>

using namespace std;

namespace libstrings {

    // std::string to null-terminated uint8_t string converter.
    uint8_t * ToUint8_tString(string str) {
        size_t length = str.length() + 1;
        uint8_t * p = new uint8_t[length];

        for (size_t j=0; j < str.length(); j++) {  //UTF-8, so this is code-point by code-point rather than char by char, but same result here.
            p[j] = str[j];
        }
        p[length - 1] = '\0';
        return p;
    }

    ////////////////////////////////
    // Transcoder Class Functions
    ////////////////////////////////

    Transcoder::Transcoder() {
        //Fill common character maps (1251/1252 -> UTF-8).
        boost::unordered_map<char, uint32_t> commonMap;
        commonMap.emplace('\x00', 0x0000);
        commonMap.emplace('\x01', 0x0001);
        commonMap.emplace('\x02', 0x0002);
        commonMap.emplace('\x03', 0x0003);
        commonMap.emplace('\x04', 0x0004);
        commonMap.emplace('\x05', 0x0005);
        commonMap.emplace('\x06', 0x0006);
        commonMap.emplace('\x07', 0x0007);
        commonMap.emplace('\x08', 0x0008);
        commonMap.emplace('\x09', 0x0009);
        commonMap.emplace('\x0A', 0x000A);
        commonMap.emplace('\x0B', 0x000B);
        commonMap.emplace('\x0C', 0x000C);
        commonMap.emplace('\x0D', 0x000D);
        commonMap.emplace('\x0E', 0x000E);
        commonMap.emplace('\x0F', 0x000F);
        commonMap.emplace('\x10', 0x0010);
        commonMap.emplace('\x11', 0x0011);
        commonMap.emplace('\x12', 0x0012);
        commonMap.emplace('\x13', 0x0013);
        commonMap.emplace('\x14', 0x0014);
        commonMap.emplace('\x15', 0x0015);
        commonMap.emplace('\x16', 0x0016);
        commonMap.emplace('\x17', 0x0017);
        commonMap.emplace('\x18', 0x0018);
        commonMap.emplace('\x19', 0x0019);
        commonMap.emplace('\x1A', 0x001A);
        commonMap.emplace('\x1B', 0x001B);
        commonMap.emplace('\x1C', 0x001C);
        commonMap.emplace('\x1D', 0x001D);
        commonMap.emplace('\x1E', 0x001E);
        commonMap.emplace('\x1F', 0x001F);
        commonMap.emplace('\x20', 0x0020);
        commonMap.emplace('\x21', 0x0021);
        commonMap.emplace('\x22', 0x0022);
        commonMap.emplace('\x23', 0x0023);
        commonMap.emplace('\x24', 0x0024);
        commonMap.emplace('\x25', 0x0025);
        commonMap.emplace('\x26', 0x0026);
        commonMap.emplace('\x27', 0x0027);
        commonMap.emplace('\x28', 0x0028);
        commonMap.emplace('\x29', 0x0029);
        commonMap.emplace('\x2A', 0x002A);
        commonMap.emplace('\x2B', 0x002B);
        commonMap.emplace('\x2C', 0x002C);
        commonMap.emplace('\x2D', 0x002D);
        commonMap.emplace('\x2E', 0x002E);
        commonMap.emplace('\x2F', 0x002F);
        commonMap.emplace('\x30', 0x0030);
        commonMap.emplace('\x31', 0x0031);
        commonMap.emplace('\x32', 0x0032);
        commonMap.emplace('\x33', 0x0033);
        commonMap.emplace('\x34', 0x0034);
        commonMap.emplace('\x35', 0x0035);
        commonMap.emplace('\x36', 0x0036);
        commonMap.emplace('\x37', 0x0037);
        commonMap.emplace('\x38', 0x0038);
        commonMap.emplace('\x39', 0x0039);
        commonMap.emplace('\x3A', 0x003A);
        commonMap.emplace('\x3B', 0x003B);
        commonMap.emplace('\x3C', 0x003C);
        commonMap.emplace('\x3D', 0x003D);
        commonMap.emplace('\x3E', 0x003E);
        commonMap.emplace('\x3F', 0x003F);
        commonMap.emplace('\x40', 0x0040);
        commonMap.emplace('\x41', 0x0041);
        commonMap.emplace('\x42', 0x0042);
        commonMap.emplace('\x43', 0x0043);
        commonMap.emplace('\x44', 0x0044);
        commonMap.emplace('\x45', 0x0045);
        commonMap.emplace('\x46', 0x0046);
        commonMap.emplace('\x47', 0x0047);
        commonMap.emplace('\x48', 0x0048);
        commonMap.emplace('\x49', 0x0049);
        commonMap.emplace('\x4A', 0x004A);
        commonMap.emplace('\x4B', 0x004B);
        commonMap.emplace('\x4C', 0x004C);
        commonMap.emplace('\x4D', 0x004D);
        commonMap.emplace('\x4E', 0x004E);
        commonMap.emplace('\x4F', 0x004F);
        commonMap.emplace('\x50', 0x0050);
        commonMap.emplace('\x51', 0x0051);
        commonMap.emplace('\x52', 0x0052);
        commonMap.emplace('\x53', 0x0053);
        commonMap.emplace('\x54', 0x0054);
        commonMap.emplace('\x55', 0x0055);
        commonMap.emplace('\x56', 0x0056);
        commonMap.emplace('\x57', 0x0057);
        commonMap.emplace('\x58', 0x0058);
        commonMap.emplace('\x59', 0x0059);
        commonMap.emplace('\x5A', 0x005A);
        commonMap.emplace('\x5B', 0x005B);
        commonMap.emplace('\x5C', 0x005C);
        commonMap.emplace('\x5D', 0x005D);
        commonMap.emplace('\x5E', 0x005E);
        commonMap.emplace('\x5F', 0x005F);
        commonMap.emplace('\x60', 0x0060);
        commonMap.emplace('\x61', 0x0061);
        commonMap.emplace('\x62', 0x0062);
        commonMap.emplace('\x63', 0x0063);
        commonMap.emplace('\x64', 0x0064);
        commonMap.emplace('\x65', 0x0065);
        commonMap.emplace('\x66', 0x0066);
        commonMap.emplace('\x67', 0x0067);
        commonMap.emplace('\x68', 0x0068);
        commonMap.emplace('\x69', 0x0069);
        commonMap.emplace('\x6A', 0x006A);
        commonMap.emplace('\x6B', 0x006B);
        commonMap.emplace('\x6C', 0x006C);
        commonMap.emplace('\x6D', 0x006D);
        commonMap.emplace('\x6E', 0x006E);
        commonMap.emplace('\x6F', 0x006F);
        commonMap.emplace('\x70', 0x0070);
        commonMap.emplace('\x71', 0x0071);
        commonMap.emplace('\x72', 0x0072);
        commonMap.emplace('\x73', 0x0073);
        commonMap.emplace('\x74', 0x0074);
        commonMap.emplace('\x75', 0x0075);
        commonMap.emplace('\x76', 0x0076);
        commonMap.emplace('\x77', 0x0077);
        commonMap.emplace('\x78', 0x0078);
        commonMap.emplace('\x79', 0x0079);
        commonMap.emplace('\x7A', 0x007A);
        commonMap.emplace('\x7B', 0x007B);
        commonMap.emplace('\x7C', 0x007C);
        commonMap.emplace('\x7D', 0x007D);
        commonMap.emplace('\x7E', 0x007E);
        commonMap.emplace('\x7F', 0x007F);
        commonMap.emplace('\x82', 0x201A);
        commonMap.emplace('\x84', 0x201E);
        commonMap.emplace('\x85', 0x2026);
        commonMap.emplace('\x86', 0x2020);
        commonMap.emplace('\x87', 0x2021);
        commonMap.emplace('\x89', 0x2030);
        commonMap.emplace('\x8B', 0x2039);
        commonMap.emplace('\x91', 0x2018);
        commonMap.emplace('\x92', 0x2019);
        commonMap.emplace('\x93', 0x201C);
        commonMap.emplace('\x94', 0x201D);
        commonMap.emplace('\x95', 0x2022);
        commonMap.emplace('\x96', 0x2013);
        commonMap.emplace('\x97', 0x2014);
        commonMap.emplace('\x99', 0x2122);
        commonMap.emplace('\x9B', 0x203A);
        commonMap.emplace('\xA0', 0x00A0);
        commonMap.emplace('\xA4', 0x00A4);
        commonMap.emplace('\xA6', 0x00A6);
        commonMap.emplace('\xA7', 0x00A7);
        commonMap.emplace('\xA9', 0x00A9);
        commonMap.emplace('\xAB', 0x00AB);
        commonMap.emplace('\xAC', 0x00AC);
        commonMap.emplace('\xAD', 0x00AD);
        commonMap.emplace('\xAE', 0x00AE);
        commonMap.emplace('\xB0', 0x00B0);
        commonMap.emplace('\xB1', 0x00B1);
        commonMap.emplace('\xB5', 0x00B5);
        commonMap.emplace('\xB6', 0x00B6);
        commonMap.emplace('\xB7', 0x00B7);
        commonMap.emplace('\xBB', 0x00BB);

        //Fill 1251 -> UTF-8 map.
        //0x98 is unused in Windows-1251.
        map1251toUTF8 = commonMap;  //Fill with common mapped characters.
        map1251toUTF8.emplace('\x80', 0x0402);
        map1251toUTF8.emplace('\x81', 0x0403);
        map1251toUTF8.emplace('\x83', 0x0453);
        map1251toUTF8.emplace('\x88', 0x20AC);
        map1251toUTF8.emplace('\x8A', 0x0409);
        map1251toUTF8.emplace('\x8C', 0x040A);
        map1251toUTF8.emplace('\x8D', 0x040C);
        map1251toUTF8.emplace('\x8E', 0x040B);
        map1251toUTF8.emplace('\x8F', 0x040F);
        map1251toUTF8.emplace('\x90', 0x0452);
        map1251toUTF8.emplace('\x9A', 0x0459);
        map1251toUTF8.emplace('\x9C', 0x045A);
        map1251toUTF8.emplace('\x9D', 0x045C);
        map1251toUTF8.emplace('\x9E', 0x045B);
        map1251toUTF8.emplace('\x9F', 0x045F);
        map1251toUTF8.emplace('\xA1', 0x040E);
        map1251toUTF8.emplace('\xA2', 0x045E);
        map1251toUTF8.emplace('\xA3', 0x0408);
        map1251toUTF8.emplace('\xA5', 0x0490);
        map1251toUTF8.emplace('\xA8', 0x0401);
        map1251toUTF8.emplace('\xAA', 0x0404);
        map1251toUTF8.emplace('\xAF', 0x0407);
        map1251toUTF8.emplace('\xB2', 0x0406);
        map1251toUTF8.emplace('\xB3', 0x0456);
        map1251toUTF8.emplace('\xB4', 0x0491);
        map1251toUTF8.emplace('\xB8', 0x0451);
        map1251toUTF8.emplace('\xB9', 0x2116);
        map1251toUTF8.emplace('\xBA', 0x0454);
        map1251toUTF8.emplace('\xBC', 0x0458);
        map1251toUTF8.emplace('\xBD', 0x0405);
        map1251toUTF8.emplace('\xBE', 0x0455);
        map1251toUTF8.emplace('\xBF', 0x0457);
        map1251toUTF8.emplace('\xC0', 0x0410);
        map1251toUTF8.emplace('\xC1', 0x0411);
        map1251toUTF8.emplace('\xC2', 0x0412);
        map1251toUTF8.emplace('\xC3', 0x0413);
        map1251toUTF8.emplace('\xC4', 0x0414);
        map1251toUTF8.emplace('\xC5', 0x0415);
        map1251toUTF8.emplace('\xC6', 0x0416);
        map1251toUTF8.emplace('\xC7', 0x0417);
        map1251toUTF8.emplace('\xC8', 0x0418);
        map1251toUTF8.emplace('\xC9', 0x0419);
        map1251toUTF8.emplace('\xCA', 0x041A);
        map1251toUTF8.emplace('\xCB', 0x041B);
        map1251toUTF8.emplace('\xCC', 0x041C);
        map1251toUTF8.emplace('\xCD', 0x041D);
        map1251toUTF8.emplace('\xCE', 0x041E);
        map1251toUTF8.emplace('\xCF', 0x041F);
        map1251toUTF8.emplace('\xD0', 0x0420);
        map1251toUTF8.emplace('\xD1', 0x0421);
        map1251toUTF8.emplace('\xD2', 0x0422);
        map1251toUTF8.emplace('\xD3', 0x0423);
        map1251toUTF8.emplace('\xD4', 0x0424);
        map1251toUTF8.emplace('\xD5', 0x0425);
        map1251toUTF8.emplace('\xD6', 0x0426);
        map1251toUTF8.emplace('\xD7', 0x0427);
        map1251toUTF8.emplace('\xD8', 0x0428);
        map1251toUTF8.emplace('\xD9', 0x0429);
        map1251toUTF8.emplace('\xDA', 0x042A);
        map1251toUTF8.emplace('\xDB', 0x042B);
        map1251toUTF8.emplace('\xDC', 0x042C);
        map1251toUTF8.emplace('\xDD', 0x042D);
        map1251toUTF8.emplace('\xDE', 0x042E);
        map1251toUTF8.emplace('\xDF', 0x042F);
        map1251toUTF8.emplace('\xE0', 0x0430);
        map1251toUTF8.emplace('\xE1', 0x0431);
        map1251toUTF8.emplace('\xE2', 0x0432);
        map1251toUTF8.emplace('\xE3', 0x0433);
        map1251toUTF8.emplace('\xE4', 0x0434);
        map1251toUTF8.emplace('\xE5', 0x0435);
        map1251toUTF8.emplace('\xE6', 0x0436);
        map1251toUTF8.emplace('\xE7', 0x0437);
        map1251toUTF8.emplace('\xE8', 0x0438);
        map1251toUTF8.emplace('\xE9', 0x0439);
        map1251toUTF8.emplace('\xEA', 0x043A);
        map1251toUTF8.emplace('\xEB', 0x043B);
        map1251toUTF8.emplace('\xEC', 0x043C);
        map1251toUTF8.emplace('\xED', 0x043D);
        map1251toUTF8.emplace('\xEE', 0x043E);
        map1251toUTF8.emplace('\xEF', 0x043F);
        map1251toUTF8.emplace('\xF0', 0x0440);
        map1251toUTF8.emplace('\xF1', 0x0441);
        map1251toUTF8.emplace('\xF2', 0x0442);
        map1251toUTF8.emplace('\xF3', 0x0443);
        map1251toUTF8.emplace('\xF4', 0x0444);
        map1251toUTF8.emplace('\xF5', 0x0445);
        map1251toUTF8.emplace('\xF6', 0x0446);
        map1251toUTF8.emplace('\xF7', 0x0447);
        map1251toUTF8.emplace('\xF8', 0x0448);
        map1251toUTF8.emplace('\xF9', 0x0449);
        map1251toUTF8.emplace('\xFA', 0x044A);
        map1251toUTF8.emplace('\xFB', 0x044B);
        map1251toUTF8.emplace('\xFC', 0x044C);
        map1251toUTF8.emplace('\xFD', 0x044D);
        map1251toUTF8.emplace('\xFE', 0x044E);
        map1251toUTF8.emplace('\xFF', 0x044F);

        //Now fill 1252 -> UTF-8 map.
        //0x81, 0x8D, 0x8F, 0x90, 0x9D are unused in Windows-1251.
        map1252toUTF8 = commonMap;  //Fill with common mapped characters.
        map1252toUTF8.emplace('\x80', 0x20AC);
        map1252toUTF8.emplace('\x83', 0x0192);
        map1252toUTF8.emplace('\x88', 0x02C6);
        map1252toUTF8.emplace('\x8A', 0x0160);
        map1252toUTF8.emplace('\x8C', 0x0152);
        map1252toUTF8.emplace('\x8E', 0x017D);
        map1252toUTF8.emplace('\x98', 0x02DC);
        map1252toUTF8.emplace('\x9A', 0x0161);
        map1252toUTF8.emplace('\x9C', 0x0153);
        map1252toUTF8.emplace('\x9E', 0x017E);
        map1252toUTF8.emplace('\x9F', 0x0178);
        map1252toUTF8.emplace('\xA1', 0x00A1);
        map1252toUTF8.emplace('\xA2', 0x00A2);
        map1252toUTF8.emplace('\xA3', 0x00A3);
        map1252toUTF8.emplace('\xA5', 0x00A5);
        map1252toUTF8.emplace('\xA8', 0x00A8);
        map1252toUTF8.emplace('\xAA', 0x00AA);
        map1252toUTF8.emplace('\xAF', 0x00AF);
        map1252toUTF8.emplace('\xB2', 0x00B2);
        map1252toUTF8.emplace('\xB3', 0x00B3);
        map1252toUTF8.emplace('\xB4', 0x00B4);
        map1252toUTF8.emplace('\xB8', 0x00B8);
        map1252toUTF8.emplace('\xB9', 0x00B9);
        map1252toUTF8.emplace('\xBA', 0x00BA);
        map1252toUTF8.emplace('\xBC', 0x00BC);
        map1252toUTF8.emplace('\xBD', 0x00BD);
        map1252toUTF8.emplace('\xBE', 0x00BE);
        map1252toUTF8.emplace('\xBF', 0x00BF);
        map1252toUTF8.emplace('\xC0', 0x00C0);
        map1252toUTF8.emplace('\xC1', 0x00C1);
        map1252toUTF8.emplace('\xC2', 0x00C2);
        map1252toUTF8.emplace('\xC3', 0x00C3);
        map1252toUTF8.emplace('\xC4', 0x00C4);
        map1252toUTF8.emplace('\xC5', 0x00C5);
        map1252toUTF8.emplace('\xC6', 0x00C6);
        map1252toUTF8.emplace('\xC7', 0x00C7);
        map1252toUTF8.emplace('\xC8', 0x00C8);
        map1252toUTF8.emplace('\xC9', 0x00C9);
        map1252toUTF8.emplace('\xCA', 0x00CA);
        map1252toUTF8.emplace('\xCB', 0x00CB);
        map1252toUTF8.emplace('\xCC', 0x00CC);
        map1252toUTF8.emplace('\xCD', 0x00CD);
        map1252toUTF8.emplace('\xCE', 0x00CE);
        map1252toUTF8.emplace('\xCF', 0x00CF);
        map1252toUTF8.emplace('\xD0', 0x00D0);
        map1252toUTF8.emplace('\xD1', 0x00D1);
        map1252toUTF8.emplace('\xD2', 0x00D2);
        map1252toUTF8.emplace('\xD3', 0x00D3);
        map1252toUTF8.emplace('\xD4', 0x00D4);
        map1252toUTF8.emplace('\xD5', 0x00D5);
        map1252toUTF8.emplace('\xD6', 0x00D6);
        map1252toUTF8.emplace('\xD7', 0x00D7);
        map1252toUTF8.emplace('\xD8', 0x00D8);
        map1252toUTF8.emplace('\xD9', 0x00D9);
        map1252toUTF8.emplace('\xDA', 0x00DA);
        map1252toUTF8.emplace('\xDB', 0x00DB);
        map1252toUTF8.emplace('\xDC', 0x00DC);
        map1252toUTF8.emplace('\xDD', 0x00DD);
        map1252toUTF8.emplace('\xDE', 0x00DE);
        map1252toUTF8.emplace('\xDF', 0x00DF);
        map1252toUTF8.emplace('\xE0', 0x00E0);
        map1252toUTF8.emplace('\xE1', 0x00E1);
        map1252toUTF8.emplace('\xE2', 0x00E2);
        map1252toUTF8.emplace('\xE3', 0x00E3);
        map1252toUTF8.emplace('\xE4', 0x00E4);
        map1252toUTF8.emplace('\xE5', 0x00E5);
        map1252toUTF8.emplace('\xE6', 0x00E6);
        map1252toUTF8.emplace('\xE7', 0x00E7);
        map1252toUTF8.emplace('\xE8', 0x00E8);
        map1252toUTF8.emplace('\xE9', 0x00E9);
        map1252toUTF8.emplace('\xEA', 0x00EA);
        map1252toUTF8.emplace('\xEB', 0x00EB);
        map1252toUTF8.emplace('\xEC', 0x00EC);
        map1252toUTF8.emplace('\xED', 0x00ED);
        map1252toUTF8.emplace('\xEE', 0x00EE);
        map1252toUTF8.emplace('\xEF', 0x00EF);
        map1252toUTF8.emplace('\xF0', 0x00F0);
        map1252toUTF8.emplace('\xF1', 0x00F1);
        map1252toUTF8.emplace('\xF2', 0x00F2);
        map1252toUTF8.emplace('\xF3', 0x00F3);
        map1252toUTF8.emplace('\xF4', 0x00F4);
        map1252toUTF8.emplace('\xF5', 0x00F5);
        map1252toUTF8.emplace('\xF6', 0x00F6);
        map1252toUTF8.emplace('\xF7', 0x00F7);
        map1252toUTF8.emplace('\xF8', 0x00F8);
        map1252toUTF8.emplace('\xF9', 0x00F9);
        map1252toUTF8.emplace('\xFA', 0x00FA);
        map1252toUTF8.emplace('\xFB', 0x00FB);
        map1252toUTF8.emplace('\xFC', 0x00FC);
        map1252toUTF8.emplace('\xFD', 0x00FD);
        map1252toUTF8.emplace('\xFE', 0x00FE);
        map1252toUTF8.emplace('\xFF', 0x00FF);
    }

    string Transcoder::ToUTF8(const string inString, const int inEncoding) {

        //Check if string is already valid UTF-8.
        if (utf8::is_valid(inString.begin(), inString.end()))
            return inString;

        //Not valid UTF-8, so select the appropriate code point map.
        boost::unordered_map<char, uint32_t> * mapToUse;
        if (inEncoding == 1251)
            mapToUse = &map1251toUTF8;
        else if (inEncoding == 1252)
            mapToUse = &map1252toUTF8;
        else
            throw error(LIBSTRINGS_ERROR_INVALID_ARGS, "Invalid encoding specified.");

        //Now transcode it.
        string outString;
        for (string::const_iterator iter = inString.begin(); iter != inString.end(); ++iter) {
            boost::unordered_map<char, uint32_t>::iterator mapIter = mapToUse->find(*iter);
            if (mapIter != mapToUse->end()) {
                try {
                    utf8::append(mapIter->second, back_inserter(outString));
                } catch (...) {
                    throw error(LIBSTRINGS_ERROR_BAD_STRING, inString);
                }
            } else
                throw error(LIBSTRINGS_ERROR_BAD_STRING, inString);
        }
        return outString;
    }
}
