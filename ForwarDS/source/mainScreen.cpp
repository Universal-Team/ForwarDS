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

#include "common.hpp"
#include "mainScreen.hpp"

extern bool touching(touchPosition t, Structs::ButtonPos button);
extern CheatManager cheatManager;

void MainScreen::Draw(void) const {
	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get(this->MainScreenDesc[this->selection]), 360, 115, nullptr, C2D_WordWrap);
	Gui::DrawString(336, 222, 0.50f, WHITE, V_STRING);
	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);


	switch(forwarder.State) {
		case ForwarderState::OK:
			Gui::DrawString(6, 5, 0.55f, WHITE, "ROM: " + forwarder.ROMPath, 310);
			break;

		case ForwarderState::NoIniFound:
			Gui::DrawString(6, 5, 0.55f, WHITE, Lang::Get("MA_INI_NOT_FOUND"), 395);
			break;

		case ForwarderState::NoForwarder:
			Gui::DrawString(6, 5, 0.55f, WHITE, Lang::Get("MA_NO_FORWARDER_LOADED"), 395);
			break;
	}

	for (uint8_t i = 0; i < 4; i++) {
		if (this->selection == i) Gui::DrawSprite(sprites, sprites_button2_idx, this->buttons[i].x, this->buttons[i].y);
		else GFX::DrawImageTint(sprites_button2_idx, this->buttons[i].x, this->buttons[i].y, GRAY);

		Gui::DrawStringCentered(0, this->buttons[i].y + 5, 0.7f, BLACK, Lang::Get(this->MainScreenText[i]));
	}
};

void MainScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_START) exiting = true;

	if (hDown & KEY_DOWN) {
		if (this->selection < 3) this->selection++;
	}
	if (hDown & KEY_UP) {
		if (this->selection > 0) this->selection--;
	}

	if (hDown & KEY_A) {
		switch(this->selection) {
			case 0:
				/* Game Settings. */
				if (forwarder.State == ForwarderState::OK) Overlays::GameSettings(forwarder, cheatManager);
				else Msg::waitMsg(Lang::Get("MA_NO_FORWARDER_LOADED_MSG"));
				break;

			case 1:
				/* Forwarder Creator. */
				Overlays::ForwarderCreator();
				break;

			case 2:
				/* App Settings. */
				Overlays::AppSettings(cheatManager);
				break;

			case 3:
				/* Game Booting. */
				if (forwarder.State == ForwarderState::OK) Utils::BootROM(forwarder, cheatManager);
				else Msg::waitMsg(Lang::Get("MA_NO_FORWARDER_LOADED_MSG"));
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		/* Game Settings. */
		if (touching(touch, this->buttons[0])) {
			if (forwarder.State == ForwarderState::OK) Overlays::GameSettings(forwarder, cheatManager);
			else Msg::waitMsg(Lang::Get("MA_NO_FORWARDER_LOADED_MSG"));

		/* Forwarder Creator. */
		} else if (touching(touch, this->buttons[1])) {
			Overlays::ForwarderCreator();

		/* App Settings. */
		} else if (touching(touch, this->buttons[2])) {
			Overlays::AppSettings(cheatManager);

		/* Game Booting. */
		} else if (touching(touch, this->buttons[3])) {
			if (forwarder.State == ForwarderState::OK) Utils::BootROM(forwarder, cheatManager);
			else Msg::waitMsg(Lang::Get("MA_NO_FORWARDER_LOADED_MSG"));
		}
	}
};