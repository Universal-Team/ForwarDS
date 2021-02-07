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
#include "settings.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <vector>

extern bool touching(touchPosition t, Structs::ButtonPos button);
const std::vector<std::string> OptionText = { "AS_DIRECT_BOOT_TEXT", "AS_LOAD_FORWARDER_TEXT", "AS_CHANGE_LANGUAGE_TEXT", "AS_SHOW_CREDITS_TEXT" };
const std::vector<std::string> OptionDesc = { "AS_DIRECT_BOOT_DESC", "AS_LOAD_FORWARDER_DESC", "AS_CHANGE_LANGUAGE_DESC", "AS_SHOW_CREDITS_DESC" };
const std::vector<Structs::ButtonPos> buttons = {
	{ 42, 45, 236, 32 },
	{ 42, 95, 236, 32 },
	{ 42, 145, 236, 32 },
	{ 42, 195, 236, 32 }
};

/* Display Credits. */
static void DisplayCredits() {
	bool exit = false;

	while(!exit) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, TRANSPARENT);
		C2D_TargetClear(Bottom, TRANSPARENT);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		Gui::ScreenDraw(Top);
		Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
		Gui::DrawStringCentered(0, 115, 0.7f, WHITE, Lang::Get("CR_DESC"), 360, 115, nullptr, C2D_WordWrap);

		Gui::ScreenDraw(Bottom);
		Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
		Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);

		Gui::DrawStringCentered(0, 30, 0.6f, BLACK, Lang::Get("CR_DEV"), 275);

		Gui::DrawStringCentered(0, 80, 0.5f, BLACK, "devkitPro", 275);
		Gui::DrawStringCentered(0, 100, 0.4f, BLACK, "- devkitARM, Libctru, Citro2D & Citro3D", 275);

		Gui::DrawStringCentered(0, 130, 0.5f, BLACK, "RocketRobz", 275);
		Gui::DrawStringCentered(0, 150, 0.4f, BLACK, Lang::Get("CR_TWLMENU_LAUNCHER_GRAPHICS"), 275);

		Gui::DrawStringCentered(0, 180, 0.5f, BLACK, "Universal-Team", 275);
		Gui::DrawStringCentered(0, 200, 0.4f, BLACK, "- Universal-Core", 275);

		C3D_FrameEnd(0);

		hidScanInput();
		uint32_t hDown = hidKeysDown();
		if (hDown) exit = true;
	}
}

/* Draw the app settings. */
static void Draw(const uint8_t selection) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get(OptionDesc[selection]), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);

	for (int i = 0; i < 4; i++) {
		if (selection == i) Gui::DrawSprite(sprites, sprites_button2_idx, buttons[i].x, buttons[i].y);
		else GFX::DrawImageTint(sprites_button2_idx, buttons[i].x, buttons[i].y, GRAY);
	}

	Gui::DrawStringCentered(0, buttons[0].y + 6, 0.6f, BLACK, Lang::Get(OptionText[0]) + (Settings::DirectBoot ? Lang::Get("AS_DIRECT_BOOT_ON") : Lang::Get("AS_DIRECT_BOOT_OFF")), 230);
	Gui::DrawStringCentered(0, buttons[1].y + 6, 0.6f, BLACK, Lang::Get(OptionText[1]), 230);
	Gui::DrawStringCentered(0, buttons[2].y + 6, 0.6f, BLACK, Lang::Get(OptionText[2]), 230);
	Gui::DrawStringCentered(0, buttons[3].y + 6, 0.6f, BLACK, Lang::Get(OptionText[3]), 230);

	C3D_FrameEnd(0);
}

/* App Settings Overlay logic. */
void Overlays::AppSettings(CheatManager &cheatManager) {
	bool doOut = false;
	int selection = 0;

	while(!doOut) {
		Draw(selection);

		touchPosition t;
		hidScanInput();
		uint32_t hDown = hidKeysDown();
		hidTouchRead(&t);

		if (hDown & KEY_DOWN) {
			if (selection < 3) selection++;
		}

		if (hDown & KEY_UP) {
			if (selection > 0) selection--;
		}

		if (hDown & KEY_A) {
			switch(selection) {
				case 0:
					/* Direct Boot. */
					Settings::DirectBoot = !Settings::DirectBoot;
					break;

				case 1: {
						/* Forwarder load. */
						const std::string Path = Overlays::SelectFileNoBack("sdmc:/3ds/ForwarDS/forwarders/", { "ini" }, Lang::Get("AP_SELECT_FORWARDER"));
						if (Path != "") Utils::InitForwarder(forwarder, Path, false, cheatManager);
					}
					break;

				case 2:
					/* Language Selection. */
					Overlays::SelectLanguage();
					break;

				case 3:
					/* Credits display. */
					DisplayCredits();
					break;
			}
		}

		if (hDown & KEY_TOUCH) {
			/* Direct boot. */
			if (touching(t, buttons[0])) {
				selection = 0;

				Settings::DirectBoot = !Settings::DirectBoot;

				/* Forwarder loader. */
			} else if (touching(t, buttons[1])) {
				selection = 1;

				const std::string Path = Overlays::SelectFileNoBack("sdmc:/3ds/ForwarDS/forwarders/", { "ini" }, Lang::Get("AP_SELECT_FORWARDER"));
				if (Path != "") Utils::InitForwarder(forwarder, Path, false, cheatManager);

				/* Language Selection. */
			} else if (touching(t, buttons[2])) {
				selection = 2;

				Overlays::SelectLanguage();

				/* Credits display. */
			} else if (touching(t, buttons[3])) {
				selection = 3;

				DisplayCredits();
			}
		}

		if (hDown & KEY_B) doOut = true; // With B, we'll exit here.
	}
}