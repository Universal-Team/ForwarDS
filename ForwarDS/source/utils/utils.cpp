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
#include "lang.hpp"
#include "msg.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include <3ds.h>
#include <cstring>
#include <unistd.h>

/* Read TWiLight Menu's default settings. */
void Utils::ReadTWLMenuDefaults(DefaultGameSettings &defaults) {
	std::unique_ptr<CIniFile> ini = std::make_unique<CIniFile>("sdmc:/_nds/TWiLightMenu/settings.ini");

	if (ini) {
		defaults.Language = ini->GetInt("NDS-BOOTSTRAP", "LANGUAGE", -1);
		defaults.DSiMode = ini->GetInt("NDS-BOOTSTRAP", "DSI_MODE", 0);
		defaults.BoostCPU = ini->GetInt("NDS-BOOTSTRAP", "BOOST_CPU", 0);
		defaults.BoostVRAM = ini->GetInt("NDS-BOOTSTRAP", "BOOST_VRAM", 0);
		defaults.ExtendedMemory = ini->GetInt("NDS-BOOTSTRAP", "EXTENDED_MEMORY", 0);
		defaults.Bootstrap = ini->GetInt("SRLOADER", "BOOTSTRAP_FILE", 0);
		defaults.Widescreen = ini->GetInt("SRLOADER", "WIDESCREEN", 0);
	}

	defaults.HeapShrink = -1; // Seems to not be handled inside that.
}

/* Get the per game settings. */
void Utils::GetPergameSettings(const std::unique_ptr<CIniFile> &ini, Forwarder &fwd) {
	if (ini) {
		fwd.Language = ini->GetInt("GAMESETTINGS", "LANGUAGE", -1);
		fwd.DSiMode = ini->GetInt("GAMESETTINGS", "DSI_MODE", 0);
		fwd.SaveNumber = ini->GetInt("GAMESETTINGS", "SAVE_NUMBER", 0);
		fwd.BoostCPU = ini->GetInt("GAMESETTINGS", "BOOST_CPU", 0);
		fwd.BoostVRAM = ini->GetInt("GAMESETTINGS", "BOOST_VRAM", 0);
		fwd.HeapShrink = ini->GetInt("GAMESETTINGS", "HEAP_SHRINK", -1);
		fwd.Bootstrap = ini->GetInt("GAMESETTINGS", "BOOTSTRAP_FILE", 0);
	}
}

/* Get ROM filepath and name. */
void Utils::GetROM(const std::unique_ptr<CIniFile> &ini, Forwarder &fwd) {
	if (ini) {
		fwd.ROMPath = ini->GetString("ROM", "PATH", "");
		fwd.ROMName = fwd.ROMPath;

		if (fwd.ROMName != "") {
			const size_t last_slash_idx = fwd.ROMName.find_last_of("/");
			if (std::string::npos != last_slash_idx) fwd.ROMName.erase(0, last_slash_idx + 1);
		}
	}
}

/* Initialize a forwarder. */
bool Utils::InitForwarder(Forwarder &fwd, const std::string &path, bool shouldBoot, CheatManager &CM) {
	fwd = Forwarder(); // Init the forwarder from new.
	fwd.State = ForwarderState::NoForwarder;

	/* Forwarder does not exist. */
	if (access(path.c_str(), F_OK) != 0) {
		fwd.State = ForwarderState::NoIniFound;
		return false;
	}

	std::unique_ptr<CIniFile> ini = std::make_unique<CIniFile>(path);

	/* Load Forwarder stuff. */
	Utils::GetPergameSettings(ini, fwd);
	Utils::GetROM(ini, fwd);

	if (fwd.ROMName != "") {
		const std::string gameSettings = "sdmc:/_nds/TWiLightMenu/gamesettings/" + fwd.ROMName + ".ini";

		/* Make sure to also check the TWiLight Menu++ per game settings and overwrite from that as well. */
		if (access(gameSettings.c_str(), F_OK) == 0) {
			std::unique_ptr<CIniFile> twlIni = std::make_unique<CIniFile>(gameSettings);
			Utils::GetPergameSettings(twlIni, fwd);
		}

	} else {
		fwd.State = ForwarderState::NoForwarder;
		return false; // ROM Name is "", which is definitely NOT a file.
	}

	fwd.State = ForwarderState::OK;

	Settings::LastForwarder = path; // Set our current forwarder as last forwarder.
	if (shouldBoot) return Utils::BootROM(fwd, CM); // Launch!
	else return true; // Because we reached until here, we're good.
}

/* TODO: Get the ROM Titles + Author of the ROM. */
void Utils::GetROMInfo(const std::string &ROM, Forwarder &fwd) { };

#define TWLMENU_LRR 0x00048015534C524EULL // TWiLight Menu++'s Last Run ROM TitleID.

/* Boot the ROM. */
bool Utils::BootROM(const Forwarder &fwd, CheatManager &CM) {
	/* Check for ROM existence. */
	if (access((std::string("sdmc:") + fwd.ROMPath).c_str(), F_OK) != 0) {
		Msg::waitMsg(Lang::Get("MA_GAME_NOT_FOUND"));
		return false;
	}

	/* Set stuff into TWiLight Menu's ini. */
	CIniFile twl("sdmc:/_nds/TWiLightMenu/settings.ini");
	twl.SetString("SRLOADER", "ROM_PATH", "sd:" + fwd.ROMPath);
	twl.SetInt("SRLOADER", "LAUNCH_TYPE", 1);
	twl.SaveIniFile("sdmc:/_nds/TWiLightMenu/settings.ini");

	/* Cheat handling. */
	CM.HandleCheats("sdmc:" + fwd.ROMPath);

	Settings::Save(); // Just to make sure.

	/* Prepare for booting TWLMenu LRR. */
	uint8_t param[0x300];
	uint8_t hmac[0x20];
	memset(param, 0, sizeof(param));
	memset(hmac, 0, sizeof(hmac));
	APT_PrepareToDoApplicationJump(0, TWLMENU_LRR, MEDIATYPE_NAND);

	/* Boot. */
	APT_DoApplicationJump(param, sizeof(param), hmac);
	return true; // We're good.
}