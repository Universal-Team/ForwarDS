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
#include "utils.hpp"
#include <vector>

const std::vector<std::string> Descriptions = {
	"GS_LANG_DESC",
	"GS_SAV_NUMBER_DESC",
	"GS_RUN_IN_DESC",
	"GS_ARM9_CPU_SPEED_DESC",
	"GS_VRAM_BOOST_DESC",
	"GS_HEAP_SHRINK_DESC",
	"GS_BOOTSTRAP_DESC",
	"GS_CHEAT_DESC"
};

/* -1 -> System Language. */
const std::vector<std::string> Languages = {
	"GS_LANG_JPN", "GS_LANG_ENG", "GS_LANG_FRE", "GS_LANG_GER", "GS_LANG_ITA", "GS_LANG_SPA", "GS_LANG_CHN", "GS_LANG_KOR"
};

const std::vector<std::string> RunModes = { "GS_DS_MODE", "GS_DSI_MODE", "GS_DSI_MODE_FORCED" };
const std::vector<std::string> ARM9CPUSpeed = { "GS_67_MHZ", "GS_133_MHZ" };
const std::vector<std::string> VRAMBoost = { "GS_VBOOST_OFF", "GS_VBOOST_ON" };
const std::vector<std::string> HeapShrink = { "GS_HEAP_SHRINK_OFF", "GS_LO", "GS_HI" }; // -1 -> Auto.
const std::vector<std::string> Bootstrap = { "GS_RELEASE", "GS_NIGHTLY" };

static void Draw(int selection, const Forwarder &fwd) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get(Descriptions[selection]), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);

	Gui::DrawStringCentered(0, 33, 0.5f, BLACK, Lang::Get("GS_GAME_SETTINGS"), 275);

	Gui::DrawString(18, 53 + (selection * 20), 0.5f, BLACK, "> ");

	/* Language being the special case, cause handling -1 and -2. */
	if (fwd.Language == -2) {
		Gui::DrawString(28, 53, 0.5f, BLACK, Lang::Get("GS_LANG_TEXT") + Lang::Get("GS_DEFAULT"), 275);
	} else {
		Gui::DrawString(28, 53, 0.5f, BLACK, Lang::Get("GS_LANG_TEXT") + (fwd.Language == -1 ? Lang::Get("GS_LANG_SYS") : Lang::Get(Languages[fwd.Language])), 275);
	}

	Gui::DrawString(28, 73, 0.5f, BLACK, Lang::Get("GS_SAV_NUMBER_TEXT") + std::to_string(fwd.SaveNumber), 275);

	Gui::DrawString(28, 93, 0.5f, BLACK, Lang::Get("GS_RUN_IN_TEXT") + (fwd.DSiMode == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(RunModes[fwd.DSiMode])), 275);
	Gui::DrawString(28, 113, 0.5f, BLACK, Lang::Get("GS_ARM9_CPU_SPEED_TEXT") + (fwd.BoostCPU == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(ARM9CPUSpeed[fwd.BoostCPU])), 275);
	Gui::DrawString(28, 133, 0.5f, BLACK, Lang::Get("GS_VRAM_BOOST_TEXT") + (fwd.BoostVRAM == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(VRAMBoost[fwd.BoostVRAM])), 275);

	/* Heap shrink a special case too cause of handling -1 & -2. */
	if (fwd.HeapShrink == -2) {
		Gui::DrawString(28, 153, 0.5f, BLACK, Lang::Get("GS_HEAP_SHRINK_TEXT") + Lang::Get("GS_DEFAULT"), 275);
	} else {
		Gui::DrawString(28, 153, 0.5f, BLACK, Lang::Get("GS_HEAP_SHRINK_TEXT") + (fwd.HeapShrink == -1 ? Lang::Get("GS_HEAP_SHRINK_AUTO") : Lang::Get(HeapShrink[fwd.HeapShrink])), 275);
	}

	Gui::DrawString(28, 173, 0.5f, BLACK, Lang::Get("GS_BOOTSTRAP_TEXT") + (fwd.Bootstrap == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(Bootstrap[fwd.Bootstrap])), 275);
	Gui::DrawString(28, 193, 0.5f, BLACK, Lang::Get("GS_CHEAT_TEXT"), 275);

	C3D_FrameEnd(0);
}

void Overlays::GameSettings(Forwarder &fwd, CheatManager &cheatManager) {
	bool doOut = false, needWrite = false;

	int selection = 0;
	while(!doOut) {
		Draw(selection, fwd);

		hidScanInput();
		uint32_t hDown = hidKeysDown();


		if (hDown & KEY_DOWN) {
			if (selection < 7) selection++;
		}

		if (hDown & KEY_UP) {
			if (selection > 0) selection--;
		}

		if (hDown & KEY_A) {
			switch(selection) {
				case 0:
					/* Language. */
					if (fwd.Language < 7) fwd.Language++;
					else fwd.Language = -2;
					break;

				case 1:
					/* Sav Number. */
					if (fwd.SaveNumber < 9) fwd.SaveNumber++;
					else fwd.SaveNumber = 0;
					break;

				case 2:
					/* Run in. */
					if (fwd.DSiMode < 2) fwd.DSiMode++;
					else fwd.DSiMode = -1;

					/* DSi Mode also has some special handling, btw. */
					if (fwd.DSiMode > 0) {
						fwd.BoostCPU = 1;
						fwd.BoostVRAM = 1;
					}

					break;

				case 3:
					/* ARM9 CPU Speed. */
					if (fwd.DSiMode < 1) { // Only allowed to change in DS Mode. Like TWiLight Menu does it.
						if (fwd.BoostCPU < 1) fwd.BoostCPU++;
						else fwd.BoostCPU = -1;
					}
					break;

				case 4:
					/* VRAM Boost. */
					if (fwd.DSiMode < 1) { // Only allowed to change in DS Mode. Like TWiLight Menu does it.
						if (fwd.BoostVRAM < 1) fwd.BoostVRAM++;
						else fwd.BoostVRAM = -1;
					}
					break;

				case 5:
					/* Heap Shrink. */
					if (fwd.HeapShrink < 2) fwd.HeapShrink++;
					else fwd.HeapShrink = -2;
					break;

				case 6:
					/* Bootstrap version. */
					if (fwd.Bootstrap < 1) fwd.Bootstrap++;
					else fwd.Bootstrap = -1;
					break;

				case 7: {
						/* Cheat Selection. */
						const std::string path = "sdmc:" + fwd.ROMPath;
						Overlays::selectCheats(path, cheatManager);
					}
					break;
			}

			if (selection != 7) needWrite = true; // Only writes needed, if a GAMESETTINGS option has been changed.
		}

		if (hDown & KEY_B) doOut = true;
	};

	if (needWrite) {
		/* Per game settings set. */
		CIniFile iniWrite("sdmc:/_nds/TWiLightMenu/gamesettings/" + fwd.ROMName + ".ini");
		iniWrite.SetInt("GAMESETTINGS", "LANGUAGE", fwd.Language);
		iniWrite.SetInt("GAMESETTINGS", "DSI_MODE", fwd.DSiMode);
		iniWrite.SetInt("GAMESETTINGS", "SAVE_NUMBER", fwd.SaveNumber);
		iniWrite.SetInt("GAMESETTINGS", "BOOST_CPU", fwd.BoostCPU);
		iniWrite.SetInt("GAMESETTINGS", "BOOST_VRAM", fwd.BoostVRAM);
		iniWrite.SetInt("GAMESETTINGS", "HEAP_SHRINK", fwd.HeapShrink);
		iniWrite.SetInt("GAMESETTINGS", "BOOTSTRAP_FILE", fwd.Bootstrap);
		//iniWrite.SetInt("GAMESETTINGS", "WIDESCREEN", fwd.Widescreen); // Widescreen TODO.
		//iniWrite.SetInt("GAMESETTINGS", "EXTENDED_MEMORY", fwd.ExtendedMemory); // Extended Memory mode?
		iniWrite.SaveIniFile("sdmc:/_nds/TWiLightMenu/gamesettings/" + fwd.ROMName + ".ini");
	}
}