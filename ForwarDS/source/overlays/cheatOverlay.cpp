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
#include "common.hpp"
#include <vector>

#define CHEATS_PER_SCREEN 8

/* Draw the Cheat list. */
static void Draw(const std::vector<cParsedItem *> &list, uint curPos, uint screenPos, uint scrollPos) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);

	/* Display cheat comment if exist. */
	if (list[curPos]->_comment != "") {
		Gui::DrawStringCentered(0, 110, 0.5f, WHITE, list[curPos]->_comment, 360, 115, nullptr, C2D_WordWrap);

	/* Else display cheat manager instruction. */
	} else {
		if (list[curPos]->_flags & cParsedItem::EFolder) { // Cheat folder.
			Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("CM_INSTRUCTION_1"), 360, 115, nullptr, C2D_WordWrap);

		} else {
			if (list[curPos]->_flags & cParsedItem::ESelected) { // Cheat activated.
				Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("CM_INSTRUCTION_2"), 360, 115, nullptr, C2D_WordWrap);

			} else { // Cheats deactivated.
				Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("CM_INSTRUCTION_3"), 360, 115, nullptr, C2D_WordWrap);
			}
		}
	}

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);
	Gui::DrawStringCentered(0, 33, 0.5f, BLACK, Lang::Get("CM_TEXT"), 275);

	/* Display cheat entries. */
	for(uint32_t i = 0; i < CHEATS_PER_SCREEN && i < list.size(); i++) {
		if (screenPos + i == curPos) Gui::DrawString(18, 53 + (i * 20), 0.48f, BLACK, ">");

		if (list[screenPos + i]->_flags & cParsedItem::EFolder) { // This is a folder.
			Gui::DrawString(28, 53 + (i * 20), 0.48f, BLACK, list[screenPos + i]->_title.substr((screenPos + i == curPos) ? scrollPos : 0), 275);

		} else {
			if (list[screenPos + i]->_flags & cParsedItem::ESelected) { // This is a cheat and activated.
				Gui::DrawString(28, 53 + (i * 20), 0.48f, GREEN, list[screenPos + i]->_title.substr((screenPos + i == curPos) ? scrollPos : 0), 275);

			} else { // This is a cheat and deactivated.
				Gui::DrawString(28, 53 + (i * 20), 0.48f, RED, list[screenPos + i]->_title.substr((screenPos + i == curPos) ? scrollPos : 0), 275);
			}
		}
	}

	C3D_FrameEnd(0);
}

/* Cheat Manager Logic. */
void Overlays::selectCheats(const std::string &filename, CheatManager &cheatManager) {
	bool cheatsFound = true;
	uint32_t hDown = 0;
	uint32_t hRepeat = 0;
  	cheatManager.parse(filename); // Parse cheat list for the ROM.

	/* If no cheats are found. */
	if (cheatManager._data.size() == 0) {
		Msg::waitMsg(Lang::Get("CM_NO_CHEATS_FOUND"));
		return;
	}

	/* Make list of all cheats not in folders. */
	std::vector<cParsedItem *> currentList;

	for(uint i = 0; i < cheatManager._data.size();i++) {
		if (!(cheatManager._data[i]._flags & cParsedItem::EInFolder)) currentList.push_back(&cheatManager._data[i]);
	}

	int mainListCurPos = -1, mainListScreenPos = -1,
	cheatWnd_cursorPosition = 0, cheatWnd_screenPosition = 0,
	cheatWnd_scrollPosition = 0;

	/* While cheats are found, we can do the logic. */
	while(cheatsFound) {
		/* Scroll screen if needed. */
		if (cheatWnd_cursorPosition < cheatWnd_screenPosition) {
			cheatWnd_screenPosition = cheatWnd_cursorPosition;

		} else if (cheatWnd_cursorPosition > cheatWnd_screenPosition + CHEATS_PER_SCREEN - 1) {
			cheatWnd_screenPosition = cheatWnd_cursorPosition - CHEATS_PER_SCREEN + 1;
		}

		Draw(currentList, cheatWnd_cursorPosition, cheatWnd_screenPosition, cheatWnd_scrollPosition);

		do {
			hidScanInput();
			hDown = hidKeysDown();
			hRepeat = hidKeysDownRepeat();

			gspWaitForVBlank();
		} while(!hDown && !hRepeat);

		if (hRepeat & KEY_UP) {
			if (cheatWnd_cursorPosition > 0) {
				cheatWnd_cursorPosition--;
				cheatWnd_scrollPosition = 0;
			}

		} else if (hRepeat & KEY_DOWN) {
			if (cheatWnd_cursorPosition < ((int)currentList.size() - 1)) {
				cheatWnd_cursorPosition++;
				cheatWnd_scrollPosition = 0;
			}

		} else if(hRepeat & KEY_LEFT) {
			if (cheatWnd_cursorPosition != 0) {
				cheatWnd_cursorPosition -= (cheatWnd_cursorPosition > CHEATS_PER_SCREEN ? CHEATS_PER_SCREEN : cheatWnd_cursorPosition);
				cheatWnd_scrollPosition = 0;
			}
		} else if (hRepeat & KEY_RIGHT) {
			if (cheatWnd_cursorPosition != (int)currentList.size() - 1) {
				cheatWnd_cursorPosition += (cheatWnd_cursorPosition < (int)(currentList.size() - CHEATS_PER_SCREEN) ? CHEATS_PER_SCREEN : currentList.size() - cheatWnd_cursorPosition - 1);
				cheatWnd_scrollPosition = 0;
			}

		/* Toggle cheat or access directory. */
		} else if(hDown & KEY_A) {
			/* We're in a folder, so parse the cheats from that folder. */
			if (currentList[cheatWnd_cursorPosition]->_flags & cParsedItem::EFolder) {
				uint i = std::distance(&cheatManager._data[0], currentList[cheatWnd_cursorPosition]) + 1;
				currentList.clear();

				for(; !(cheatManager._data[i]._flags & cParsedItem::EFolder) && i < cheatManager._data.size(); i++) {
					currentList.push_back(&cheatManager._data[i]);
				}

				mainListCurPos = cheatWnd_cursorPosition;
				mainListScreenPos = cheatWnd_screenPosition;
				cheatWnd_cursorPosition = 0;
				cheatWnd_scrollPosition = 0;

			/* Toggle the cheat. */
			} else {
				cParsedItem &cheat = *currentList[cheatWnd_cursorPosition];
				bool select = !(cheat._flags & cParsedItem::ESelected);
				if (cheat._flags & cParsedItem::EOne) cheatManager.deselectFolder(std::distance(&cheatManager._data[0], currentList[cheatWnd_cursorPosition]));
				if (select || !(cheat._flags & cParsedItem::EOne)) cheat._flags ^= cParsedItem::ESelected;
			}

		/* Go a folder back, or exit. */
		} else if(hDown & KEY_B) {
			if (mainListCurPos != -1) {
				currentList.clear();

				for(uint i = 0; i < cheatManager._data.size(); i++) {
					if (!(cheatManager._data[i]._flags & cParsedItem::EInFolder)) {
						currentList.push_back(&cheatManager._data[i]);
					}
				}

				cheatWnd_cursorPosition = mainListCurPos;
				cheatWnd_screenPosition = mainListScreenPos;
				mainListCurPos = -1;
				cheatWnd_scrollPosition = 0;

			} else {
				break;
			}

		/* Save the cheats in the usrcheat.dat file. */
		} else if (hDown & KEY_X) {
			Msg::displayMsg(Lang::Get("CM_SAVING_CHEATS"));
			cheatManager.onGenerate();
			break;

			/* Deselect all cheats. */
		} else if (hDown & KEY_L) {
			/* Deselect all in the actual data so it doesn't just get the folder. */
			for(auto itr = cheatManager._data.begin(); itr != cheatManager._data.end(); itr++) {
				(*itr)._flags &= ~cParsedItem::ESelected;
			}

			/* Also deselect them in the current list so that it updates the display. */
			for(auto itr = currentList.begin(); itr != currentList.end(); itr++) {
				(*itr)->_flags &= ~cParsedItem::ESelected;
			}
		}
	}
}