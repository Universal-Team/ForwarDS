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
#include <fstream>
#include <vector>

extern bool touching(touchPosition t, Structs::ButtonPos button);
const std::vector<std::string> Page1Options = { "FC_ROM_PATH_TEXT", "FC_GAME_SETTINGS_TEXT", "FC_CANCEL_TEXT", "FC_CREATE_TEXT" };
const std::vector<std::string> Page1Desc = { "FC_ROM_PATH_DESC", "FC_GAME_SETTINGS_DESC", "FC_CANCEL_DESC", "FC_CREATE_DESC" };

/* Game Settings related stuff. */
const std::vector<std::string> GameSettingsDescriptions = {
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

const std::vector<Structs::ButtonPos> buttons = {
	{ 42, 45, 236, 32 },
	{ 42, 95, 236, 32 },
	{ 42, 145, 236, 32 },
	{ 42, 195, 236, 32 }
};

static void Draw(const uint8_t selection, const uint8_t page, Forwarder fwd) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);

	/* Page 1: General. */
	if (page == 1) {
		Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get(Page1Desc[selection]), 360, 115, nullptr, C2D_WordWrap);

		Gui::ScreenDraw(Bottom);
		Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);

		Gui::DrawString(6, 5, 0.55f, WHITE, "ROM: " + fwd.ROMPath, 310);

		for (uint8_t i = 0; i < Page1Options.size(); i++) {
			if (selection == i) Gui::DrawSprite(sprites, sprites_button2_idx, buttons[i].x, buttons[i].y);
			else GFX::DrawImageTint(sprites_button2_idx, buttons[i].x, buttons[i].y, GRAY);

			Gui::DrawStringCentered(0, buttons[i].y + 6, 0.6f, BLACK, Lang::Get(Page1Options[i]), 230);
		}

		/* Game settings. */
	} else if (page == 2) {
		Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get(GameSettingsDescriptions[selection]), 360, 115, nullptr, C2D_WordWrap);

		Gui::ScreenDraw(Bottom);
		Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
		Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);

		Gui::DrawStringCentered(0, 33, 0.5f, BLACK, Lang::Get("GS_GAME_SETTINGS"), 275);
		Gui::DrawString(18, 53 + (selection * 20), 0.5f, BLACK, "> ");

		/* Language being a special case, cause handling -1 and -2. */
		if (fwd.Language == -2) {
			Gui::DrawString(28, 53, 0.5f, BLACK, Lang::Get("GS_LANG_TEXT") + Lang::Get("GS_DEFAULT"), 275);
		} else {
			Gui::DrawString(28, 53, 0.5f, BLACK, Lang::Get("GS_LANG_TEXT") + (fwd.Language == -1 ? Lang::Get("GS_LANG_SYS") : Lang::Get(Languages[fwd.Language])), 275);
		}

		Gui::DrawString(28, 73, 0.5f, BLACK, Lang::Get("GS_SAV_NUMBER_TEXT") + std::to_string(fwd.SaveNumber), 275);

		Gui::DrawString(28, 93, 0.5f, BLACK, Lang::Get("GS_RUN_IN_TEXT") + (fwd.DSiMode == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(RunModes[fwd.DSiMode])), 275);
		Gui::DrawString(28, 113, 0.5f, BLACK, Lang::Get("GS_ARM9_CPU_SPEED_TEXT") + (fwd.BoostCPU == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(ARM9CPUSpeed[fwd.BoostCPU])), 275);
		Gui::DrawString(28, 133, 0.5f, BLACK, Lang::Get("GS_VRAM_BOOST_TEXT") + (fwd.BoostVRAM == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(VRAMBoost[fwd.BoostVRAM])), 275);

		/* Heap shrink being a special case too cause of handling -1 & -2. */
		if (forwarder.HeapShrink == -2) {
			Gui::DrawString(28, 153, 0.5f, BLACK, Lang::Get("GS_HEAP_SHRINK_TEXT") + Lang::Get("GS_DEFAULT"), 275);
		} else {
			Gui::DrawString(28, 153, 0.5f, BLACK, Lang::Get("GS_HEAP_SHRINK_TEXT") + (fwd.HeapShrink == -1 ? Lang::Get("GS_HEAP_SHRINK_AUTO") : Lang::Get(HeapShrink[fwd.HeapShrink])), 275);
		}

		Gui::DrawString(28, 173, 0.5f, BLACK, Lang::Get("GS_BOOTSTRAP_TEXT") + (fwd.Bootstrap == -1 ? Lang::Get("GS_DEFAULT") : Lang::Get(Bootstrap[fwd.Bootstrap])), 275);
	}

	C3D_FrameEnd(0);
}

/*
	We create the XML shortcut here for our forwarder.
*/
static void CreateXML(const std::string &fN, const Forwarder &fwd) {
	std::ofstream out("sdmc:/3ds/ForwarDS/" + fN + ".xml", std::ios::binary);

	out << "<shortcut>" << std::endl;

	/* Executable. */
	out << "	<executable>" << "/3ds/ForwarDS.3dsx" << "</executable>" << std::endl;

	/* Arguments. */
	out << "	<arg>\"" << "sdmc:/3ds/ForwarDS/forwarders/" << fN << ".ini" << "\"" << "</arg>" << std::endl;

	/* Title. */
	out << "	<name>" << fwd.Title[0] << "</name>" << std::endl;

	/* Description being the ROM Name. */
	out << "	<description>" << fwd.ROMName << "</description>" << std::endl;

	/* Author being Universal-Team, cause developed by us. */
	out << "	<author>" << fwd.Author << "</author>" << std::endl;

	out << "</shortcut>" << std::endl;
	out.close();

	Msg::waitMsg(Lang::Get("AP_CREATION_SUCCESSFULL"));
}


void Overlays::ForwarderCreator() {
	Forwarder fwd = { };
	size_t dist = 0;
	uint8_t selection = 0, page = 1;
	bool finished = false, cancel = false;

	while(!finished) {
		Draw(selection, page, fwd);
		touchPosition t;
		hidScanInput();
		uint32_t hDown = hidKeysDown();
		hidTouchRead(&t);

		if (page == 1) {
			if (hDown & KEY_DOWN) {
				if (selection < (int)Page1Options.size() - 1) selection++;
			}

			if (hDown & KEY_UP) {
				if (selection > 0) selection--;
			}

			if (hDown & KEY_A) {
				switch(selection) {
					case 0:
						/* ROM path. */
						fwd.ROMPath = Overlays::SelectFile("/", { "nds" }, Lang::Get("AP_SELECT_NDS_ROM"));

						if (fwd.ROMPath != "") {
							size_t found = fwd.ROMPath.find("sdmc:/");
							if (found != std::string::npos) fwd.ROMPath.erase(0, 5); // Remove the "sdmc:/" if found.

							fwd.ROMName = fwd.ROMPath;

							dist = fwd.ROMName.find_last_of("/");
							if (std::string::npos != dist) fwd.ROMName.erase(0, dist + 1);
						}

						break;

					case 1:
						/* Game Settings. */
						selection = 0;
						page = 2;
						break;

					case 2:
						/* Cancel forwarder creation. */
						cancel = true;
						finished = true;
						break;

					case 3:
						/* Creating forwarder. */
						finished = true;
						break;
				}
			}

			if (hDown & KEY_B) {
				cancel = true;
				finished = true;
			}


			if (hDown & KEY_TOUCH) {
				if (touching(t, buttons[0])) {
					selection = 0;

					/* ROM path. */
					fwd.ROMPath = Overlays::SelectFile("/", { "nds" }, Lang::Get("AP_SELECT_NDS_ROM"));

					if (fwd.ROMPath != "") {
						size_t found = fwd.ROMPath.find("sdmc:/");
						if (found != std::string::npos) fwd.ROMPath.erase(0, 5); // Remove the "sdmc:/" if found.

						fwd.ROMName = fwd.ROMPath;

						dist = fwd.ROMName.find_last_of("/");
						if (std::string::npos != dist) fwd.ROMName.erase(0, dist + 1);
					}

				} else if (touching(t, buttons[1])) {
					/* Game Settings. */
					selection = 0;
					page = 2;

				} else if (touching(t, buttons[2])) {
					/* Cancel forwarder creation. */
					cancel = true;
					finished = true;

				} else if (touching(t, buttons[3])) {
					/* Creating forwarder. */
					finished = true;
				}
			}

			/* Game Settings. */
		} else if (page == 2) {
			if (hDown & KEY_DOWN) {
				if (selection < 6) selection++;
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
				}
			}

			if (hDown & KEY_B) {
				selection = 0;
				page = 1;
			}
		}
	}

	if (!cancel) {
		/* Writing. */
		std::unique_ptr<CIniFile> ini = std::make_unique<CIniFile>();

		/* Game Setting things. */
		ini->SetInt("GAMESETTINGS", "LANGUAGE", fwd.Language);
		ini->SetInt("GAMESETTINGS", "DSI_MODE", fwd.DSiMode);
		ini->SetInt("GAMESETTINGS", "SAVE_NUMBER", fwd.SaveNumber);
		ini->SetInt("GAMESETTINGS", "BOOST_CPU", fwd.BoostCPU);
		ini->SetInt("GAMESETTINGS", "BOOST_VRAM", fwd.BoostVRAM);
		ini->SetInt("GAMESETTINGS", "HEAP_SHRINK", fwd.HeapShrink);
		ini->SetInt("GAMESETTINGS", "BOOTSTRAP_FILE", fwd.Bootstrap);

		/* ROM things. */
		ini->SetString("ROM", "PATH", fwd.ROMPath);

		ini->SaveIniFile("sdmc:/3ds/ForwarDS/forwarders/" + fwd.ROMName + ".ini");
		CreateXML(fwd.ROMName, fwd);
	}
}