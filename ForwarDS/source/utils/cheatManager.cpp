/*
	cheat.cpp
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

#include "cheatManager.hpp"
#include "gameManager.hpp"
#include <algorithm>
#include <cstring>

#define CHEAT_DATA_LOC "sdmc:/_nds/nds-bootstrap/cheatData.bin" // Cheatdata for nds-bootstrap.
#define USRCHEAT_LOC "sdmc:/_nds/TWiLightMenu/extras/usrcheat.dat" // The location of the usrcheat.dat.

bool CheatManager::parse(const std::string &aFileName){
	bool res = false;
	uint32_t romcrc32, gamecode;

	if (this->romData(aFileName, gamecode, romcrc32)) {
		FILE *dat = fopen(USRCHEAT_LOC, "rb");

		if (dat) {
			res = this->parseInternal(dat, gamecode, romcrc32);
			fclose(dat);
		}
	}

	return res;
}

bool CheatManager::searchCheatData(FILE *aDat, uint32_t gamecode, uint32_t crc32, long &aPos, size_t &aSize) {
	aPos = 0;
	aSize = 0;
	const char *KHeader = "R4 CheatCode";
	char header[12];
	fread(header, 12, 1, aDat);
	if (strncmp(KHeader, header, 12)) return false;

	sDatIndex idx, nidx;

	fseek(aDat, 0, SEEK_END);
	long fileSize = ftell(aDat);

	fseek(aDat, 0x100, SEEK_SET);
	fread(&nidx, sizeof(nidx), 1, aDat);

	bool done = false;

	while(!done) {
		memcpy(&idx, &nidx, sizeof(idx));
		fread(&nidx, sizeof(nidx), 1, aDat);

		if (gamecode == idx._gameCode && crc32 == idx._crc32) {
			aSize = ((nidx._offset) ? nidx._offset : fileSize) - idx._offset;
			aPos = idx._offset;
			done = true;
		}

		if (!nidx._offset) done = true;
	}

	return (aPos && aSize);
}

bool CheatManager::parseInternal(FILE *aDat, uint32_t gamecode, uint32_t crc32) {
	this->_data.clear();

	long dataPos; size_t dataSize;
	if (!this->searchCheatData(aDat, gamecode, crc32, dataPos, dataSize)) return false;
	fseek(aDat, dataPos, SEEK_SET);

	char *buffer = (char *)malloc(dataSize);
	if (!buffer) return false;
	fread(buffer, dataSize, 1, aDat);
	char *gameTitle = buffer;

	uint32_t *ccode = (uint32_t *)(((uint32_t)gameTitle + strlen(gameTitle) + 4) &~ 3);
	uint32_t cheatCount =* ccode;
	cheatCount &= 0x0fffffff;
	ccode += 9;

	uint32_t cc = 0;

	while(cc < cheatCount) {
		uint32_t folderCount = 1;
		char *folderName = nullptr;
		char *folderNote = nullptr;
		uint32_t flagItem = 0;

		if ((*ccode >> 28) & 1) {
			flagItem |= cParsedItem::EInFolder;
			if ((*ccode >> 24) == 0x11) flagItem |= cParsedItem::EOne;
			folderCount =* ccode & 0x00ffffff;
			folderName = (char *)((uint32_t)ccode + 4);
			folderNote = (char *)((uint32_t)folderName + strlen(folderName) + 1);
			this->_data.push_back(cParsedItem(folderName, folderNote, cParsedItem::EFolder));
			cc++;
			ccode = (uint32_t *)(((uint32_t)folderName + strlen(folderName) + 1 + strlen(folderNote) + 1 + 3) &~ 3);
		}

		uint32_t selectValue = cParsedItem::ESelected;

		for(size_t ii = 0; ii < folderCount; ++ii) {
			char *cheatName = (char *)((uint32_t)ccode + 4);
			char *cheatNote = (char *)((uint32_t)cheatName + strlen(cheatName) + 1);
			uint32_t *cheatData = (uint32_t *)(((uint32_t)cheatNote + strlen(cheatNote) + 1 + 3) &~ 3);
			uint32_t cheatDataLen =* cheatData++;

			if (cheatDataLen) {
				this->_data.push_back(cParsedItem(cheatName, cheatNote, flagItem | ((*ccode & 0xff000000) ? selectValue : 0), dataPos + (((char *)ccode + 3) - buffer)));
				if ((*ccode & 0xff000000) && (flagItem & cParsedItem::EOne)) selectValue = 0;
				this->_data.back()._cheat.resize(cheatDataLen);
				memcpy(this->_data.back()._cheat.data(), cheatData, cheatDataLen * 4);
			}

			cc++;
			ccode = (uint32_t *)((uint32_t)ccode + (((*ccode & 0x00ffffff) + 1) * 4));
		}
	}

	free(buffer);
	this->generateList();
	return true;
}

void CheatManager::generateList(void) {
	this->_indexes.clear();
	//this->_List.removeAllRows();

	std::vector<cParsedItem>::iterator itr = this->_data.begin();

	while(itr != _data.end()) {
		std::vector<std::string> row;
		row.push_back("");
		row.push_back((*itr)._title);
		//this->_List.insertRow(_List.getRowCount(),row);
		this->_indexes.push_back(itr - this->_data.begin());
		uint32_t flags = (*itr)._flags;
		++itr;

		if ((flags & cParsedItem::EFolder) && (flags & cParsedItem::EOpen) == 0) {
			while(((*itr)._flags & cParsedItem::EInFolder) && itr != this->_data.end()) ++itr;
		}
	}
}

void CheatManager::deselectFolder(size_t anIndex) {
	std::vector<cParsedItem>::iterator itr = this->_data.begin() + anIndex;

	while(--itr >= this->_data.begin()) {
		if ((*itr)._flags & cParsedItem::EFolder) {
			++itr;
			break;
		}
	}

	while(((*itr)._flags & cParsedItem::EInFolder) && itr != this->_data.end()) {
		(*itr)._flags &= ~cParsedItem::ESelected;
		++itr;
	}
}

bool CheatManager::romData(const std::string &aFileName, uint32_t &aGameCode, uint32_t &aCrc32) {
	bool res = false;
	FILE *ROM = fopen(aFileName.c_str(), "rb");

	if (ROM) {
		uint8_t header[512];

		if (1 == fread(header, sizeof(header), 1, ROM)) {
			aCrc32 = GameManager::CRC32(header, sizeof(header));
			aGameCode = GameManager::Gamecode((const char *)(header + 12));
			res = true;
		}

		fclose(ROM);
	}

	return res;
}

std::vector<uint32_t> CheatManager::getCheats() {
	std::vector<uint32_t> cheats;

	for(uint32_t i = 0; i < this->_data.size(); i++) {
		if (this->_data[i]._flags & cParsedItem::ESelected) {
			cheats.insert(cheats.end(), this->_data[i]._cheat.begin(), this->_data[i]._cheat.end());
		}
	}

	return cheats;
}

static void updateDB(uint8_t value, uint32_t offset, FILE *db) {
	uint8_t oldvalue;

	if (!db) return;
	if (!offset) return;
	if (fseek(db, offset, SEEK_SET)) return;
	if (fread(&oldvalue, sizeof(oldvalue), 1, db) != 1) return;

	if (oldvalue != value) {
		if (fseek(db, offset, SEEK_SET)) return;
		fwrite(&value, sizeof(value), 1, db);
	}
}

void CheatManager::onGenerate(void) {
	FILE *db = fopen(USRCHEAT_LOC, "r+b");

	if (db) {
		std::vector<cParsedItem>::iterator itr = this->_data.begin();

		while(itr != this->_data.end()) {
			updateDB(((*itr)._flags & cParsedItem::ESelected) ? 1 : 0, (*itr)._offset, db);
			++itr;
		}

		fclose(db);
	}
}

/* Write cheats to the file with that. */
void CheatManager::writeCheatsToFile(const std::string &path) {
	FILE *file = fopen(path.c_str(), "wb");

	if (file) {
		std::vector<uint32_t> cheats(this->getCheats());

		fwrite(cheats.data(), 4, cheats.size(), file);
		fwrite("\0\0\0\xCF", 4, 1, file);
		fclose(file);
	}
}

/* NOTE: This is, how TWiLight Menu++ handles cheats. */
void CheatManager::HandleCheats(const std::string &ROMPath) {
	uint32_t gameCode, crc32;
	bool cheatsEnabled = true;

	if (this->romData(ROMPath, gameCode, crc32)) {
		long cheatOffset; size_t cheatSize;

		FILE *dat = fopen(USRCHEAT_LOC, "rb");

		if (dat) {
			/* Search for cheat data. */
			if (this->searchCheatData(dat, gameCode, crc32, cheatOffset, cheatSize)) {
				this->parse(ROMPath);
				this->writeCheatsToFile(CHEAT_DATA_LOC); // Write cheatdata.

				FILE *cheatData = fopen(CHEAT_DATA_LOC, "rb");
				if (cheatData) {
					fseek(cheatData, 0, SEEK_END);
					const uint32_t size = ftell(cheatData);
					fseek(cheatData, 0, SEEK_SET);

					uint32_t check[2];
					fread(check, 1, 8, cheatData);
					fclose(cheatData);

					if (check[1] == 0xCF000000 || size > 0x8000) cheatsEnabled = false;
				}

			} else { // No cheat data available.
				cheatsEnabled = false;
			}

			fclose(dat);
		} else { // usrcheat.dat not found.
			cheatsEnabled = false;
		}

	} else { // ROM error.
		cheatsEnabled = false;
	}

	if (!cheatsEnabled) remove("sdmc:/_nds/nds-bootstrap/cheatData.bin"); // Because no cheats are enabled, we'll remove it.
}