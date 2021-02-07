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

#ifndef _FORWARDS_UTILS_HPP
#define _FORWARDS_UTILS_HPP

#include "inifile.hpp"
#include "gameManager.hpp"
#include <array>
#include <memory>
#include <string>

enum class ForwarderState {
	OK,
	NoIniFound,
	NoForwarder
};

/* Used for TWiLight Menu++'s defaults. */
struct DefaultGameSettings {
	int Language = -1; // -1: System Language, 0: JPN, 1: ENG, 2: FRE, 3: GER, 4: ITA, 5: SPA, 6: CHN, 7: KOR.
	int DSiMode = 0; // 0: DS Mode, 1: DSi Mode, 2: DSi Mode (forced).
	int BoostCPU = 0; // 0: 67MHz, 1: 133MHz.
	int BoostVRAM = 0; // 0: off, 1: On.
	int HeapShrink = -1; // -1: Auto, 0: Off, 1: Lo, 2: Hi.
	int Bootstrap = 0; // 0: Release, 1: Nightly.
	int ExtendedMemory = 0; // Extended memory??
	int Widescreen = 0; // Widescreen stuff.
};

struct Forwarder {
	int Language = -1; // -1: System Language, 0: JPN, 1: ENG, 2: FRE, 3: GER, 4: ITA, 5: SPA, 6: CHN, 7: KOR.
	int SaveNumber = 0; // 0 - 9.
	int DSiMode = 0; // 0: DS Mode, 1: DSi Mode, 2: DSi Mode (forced).
	int BoostCPU = 0; // 0: 67MHz, 1: 133MHz.
	int BoostVRAM = 0; // 0: off, 1: On.
	int HeapShrink = -1; // -1: Auto, 0: Off, 1: Lo, 2: Hi.
	int Bootstrap = 0; // 0: Release, 1: Nightly.
	int ExtendedMemory = 0; // Extended memory??
	int Widescreen = 0; // Widescreen stuff.
	int IsHomebrew = 0; // If is homebrew or not.

	std::string ROMPath = ""; // Path to the ROM.
	std::string ROMName = ""; // ROM name used for display.
	std::string Author = ""; // Author of the game.
	std::string Title[8] = { "" }; // In 8 different Languages.
	ForwarderState State = ForwarderState::NoForwarder; // The Forwarder state.
};

namespace Utils {
	void ReadTWLMenuDefaults(DefaultGameSettings &defaults);
	void GetPergameSettings(const std::unique_ptr<CIniFile> &ini, Forwarder &fwd);
	void GetROM(const std::unique_ptr<CIniFile> &ini, Forwarder &fwd);
	void GetROMInfo(const std::string &ROM, Forwarder &fwd);
	bool BootROM(const Forwarder &fwd, CheatManager &CM);
	bool InitForwarder(Forwarder &fwd, const std::string &path, bool shouldBoot, CheatManager &CM);
};

#endif