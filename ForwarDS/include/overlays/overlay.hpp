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

#ifndef _FORWARDS_OVERLAY_HPP
#define _FORWARDS_OVERLAY_HPP

#include "cheatManager.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

namespace Overlays {
	/* Game Settings related things. */
	void GameSettings(Forwarder &fwd, CheatManager &cheatManager);
	void selectCheats(const std::string &filename, CheatManager &cheatManager);

	/* File Select related things. */
	std::string SelectFile(const std::string &startPath, const std::vector<std::string> &ext, const std::string &text);
	std::string SelectFileNoBack(const std::string &startPath, const std::vector<std::string> &ext, const std::string &text);

	/* Utility related things. */
	void AppSettings(CheatManager &cheatManager);
	void ForwarderCreator();
	void SelectLanguage();
};

#endif