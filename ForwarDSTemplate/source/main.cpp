/*
*   This file is part of ForwarDSTemplate
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

#include "inifile.hpp" // For CIniFile.
#include <3ds.h> // For aptMainLoop.
#include <cstring> // For memset.
#include <limits.h> // For PATH_MAX.

char INI[PATH_MAX] = "3ds/ForwarDS/forwarders/Test.ini"; // Change this to your Forwarders INI filename.
#define FORWARDS_TITLE_ID 0x0004000004391300

int main() {
	/* We'll set the Path into the config.ini. */
	CIniFile ini("sdmc:/3ds/ForwarDS/config.ini");
	ini.SetString("APP", "LAST_FORWARDER", INI);
	ini.SaveIniFile("sdmc:/3ds/ForwarDS/config.ini");

	/* We'll prepare for launching ForwarDS. */
	uint8_t param[0x300];
	uint8_t hmac[0x20];
	memset(param, 0, sizeof(param));
	memset(hmac, 0, sizeof(hmac));

	/* Launch / Jump to ForwarDS. */
	APT_PrepareToDoApplicationJump(0, FORWARDS_TITLE_ID, MEDIATYPE_SD);
	APT_DoApplicationJump(param, sizeof(param), hmac);

	aptMainLoop();
	return 0;
}