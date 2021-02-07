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
#include "settings.hpp"
#include "sound.hpp"
#include <dirent.h>
#include <string>
#include <unistd.h>

Forwarder forwarder = { };
CheatManager cheatManager;
DefaultGameSettings defaults = { };

C2D_SpriteSheet sprites;
uint32_t hDown = 0, hHeld = 0;
touchPosition touch;
std::unique_ptr<Sound> Music = nullptr;
bool dspfirmFound = false;

/* If Position pressed -> Do something. */
bool touching(touchPosition t, Structs::ButtonPos button) {
	if (t.px >= button.x && t.px <= (button.x + button.w) && t.py >= button.y && t.py <= (button.y + button.h)) return true;
	return false;
}

/* Init the music. */
static void InitMusic() {
	if (access("sdmc:/3ds/dspfirm.cdc", F_OK) == 0) { // Ensure dspfirm dump exist.
		if (access("sdmc:/3ds/ForwarDS/music.wav", F_OK) == 0) { // Ensure music.wav exist.
			dspfirmFound = true;
			ndspInit();
			Music = std::make_unique<Sound>("sdmc:/3ds/ForwarDS/music.wav");

			Music->play();
		}
	}
}

/* Exit the music. */
static void ExitMusic() {
	if (dspfirmFound) {
		Music->stop();
		Music = nullptr;
		ndspExit();
	}
}

int main(int argc, char **argv) {
	gfxInitDefault();
	romfsInit();
	Gui::init();
	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);

	/* Create TWiLight Menu++ per games settings folder. */
	mkdir("sdmc:/_nds", 0x777); // Base NDS folder.
	mkdir("sdmc:/_nds/nds-bootstrap", 0x777); // For the cheatData.bin.
	mkdir("sdmc:/_nds/TWiLightMenu", 0x777); // TWiLightMenu's main dir.
	mkdir("sdmc:/_nds/TWiLightMenu/gamesettings", 0x777); // The game settings from TWiLightMenu.

	/* Create ForwarDS' basic folders. */
	mkdir("sdmc:/3ds", 0x777);
	mkdir("sdmc:/3ds/ForwarDS", 0x777);
	mkdir("sdmc:/3ds/ForwarDS/forwarders", 0x777);

	hidSetRepeatParameters(10, 8);
	Settings::Load();
	Lang::LoadLang(Settings::Language);

	if (!Settings::FirstStart) {
		Settings::FirstStart = true;

		Msg::waitMsg(Lang::Get("MSG_FIRST_WARN"));
	}


	if (Settings::DirectBoot) {
		bool directboot = true;

		hidScanInput();
		if (hidKeysHeld() & KEY_SELECT) directboot = false;

		if (directboot) {
			if (Utils::InitForwarder(forwarder, (argc == 2 ? argv[1] : Settings::LastForwarder), true, cheatManager)) {
				exiting = true;
			}
		}
	}

	if (exiting) {
		Gui::unloadSheet(sprites);
		Settings::Save();
		Gui::exit();
		romfsExit();
		gfxExit();
		return 0;
	}

	InitMusic();
	Utils::ReadTWLMenuDefaults(defaults); // Load defaults from TWiLight Menu's config.
	Utils::InitForwarder(forwarder, (argc == 2 ? argv[1] : Settings::LastForwarder), false, cheatManager);
	Gui::setScreen(std::make_unique<MainScreen>(), false, true);

	while(aptMainLoop() && !exiting) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, TRANSPARENT);
		C2D_TargetClear(Bottom, TRANSPARENT);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, false, true);
		C3D_FrameEnd(0);

		hidScanInput();
		hDown = hidKeysDown();
		hHeld = hidKeysHeld();
		hidTouchRead(&touch);
	}

	ExitMusic();
	Gui::unloadSheet(sprites);
	Settings::Save();
	Gui::exit();
	romfsExit();
	gfxExit();

	return 0;
}