/*
    cheat.h
    Portions copyright (C) 2008 Normmatt, www.normmatt.com, Smiths (www.emuholic.com)
    Portions copyright (C) 2008 bliss (bliss@hanirc.org)
    Copyright (C) 2009 yellow wood goblin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
*   This file is part of ForwarDS
*   Copyright (C) 2021 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#ifndef _FORWARDS_CHEAT_MANAGER_HPP
#define _FORWARDS_CHEAT_MANAGER_HPP

#include <string>
#include <vector>

class cParsedItem {
public:
	std::string _title;
	std::string _comment;
	std::vector<uint32_t> _cheat;
	uint32_t _flags;
	uint32_t _offset;

	cParsedItem(const std::string &title, const std::string &comment, uint32_t flags, uint32_t offset = 0) :
		_title(title), _comment(comment), _flags(flags), _offset(offset) { };

	enum {
		EFolder = 1,
		EInFolder = 2,
		EOne = 4,
		ESelected = 8,
		EOpen = 16
	};
};

class CheatManager {
public:
	CheatManager(void) { };
	~CheatManager() { };

	bool parse(const std::string &aFileName);

	bool searchCheatData(FILE *aDat, uint32_t gamecode, uint32_t crc32, long &aPos, size_t &aSize);

	bool parseInternal(FILE *aDat, uint32_t gamecode, uint32_t crc32);

	void generateList(void);

	void deselectFolder(size_t anIndex);

	bool romData(const std::string &aFileName, uint32_t &aGameCode, uint32_t &aCrc32);

	void writeList();

	void onGenerate(void);

	struct sDatIndex {
		uint32_t _gameCode;
		uint32_t _crc32;
		uint64_t _offset;
	};

	std::vector<cParsedItem> _data;
	std::vector<size_t> _indexes;

	std::vector<uint32_t> getCheats();
	void writeCheatsToFile(const std::string &path);
	void HandleCheats(const std::string &ROMPath);
};

#endif