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

#include "gameManager.hpp"
#include <cstring>
#include <unistd.h>

#define CRCPOLY 0xEDB88320

/* Returns the game's Gamecode as an uint32_t. */
uint32_t GameManager::Gamecode(const char *aGameCode) {
	uint32_t gameCode;
	memcpy(&gameCode, aGameCode, sizeof(gameCode));
	return gameCode;
}

/* Returns the game's CRC32. */
uint32_t GameManager::CRC32(const uint8_t *p, size_t len) {
	uint32_t crc = -1;

	while(len--) {
		crc ^= *p++;

		for(int ii = 0; ii < 8; ++ii) crc = (crc >> 1) ^ ((crc & 1) ? CRCPOLY : 0);
	}

	return crc;
}