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

#ifndef _FORWARDS_MAIN_SCREEN_HPP
#define _FORWARDS_MAIN_SCREEN_HPP

#include "screen.hpp"
#include "structs.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

class MainScreen : public Screen {
public:
	~MainScreen() { };
	MainScreen() { };
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	int selection = 0;

	const std::vector<Structs::ButtonPos> buttons = {
		{ 42, 45, 236, 32 },
		{ 42, 95, 236, 32 },
		{ 42, 145, 236, 32 },
		{ 42, 195, 236, 32 }
	};

	const std::vector<std::string> MainScreenText = {
		"MA_GAME_SETTINGS_TEXT", "MA_FORWARDER_CREATOR_TEXT", "MA_APP_SETTINGS_TEXT", "MA_START_GAME_TEXT"
	};
	const std::vector<std::string> MainScreenDesc = {
		"MA_GAME_SETTINGS_DESC", "MA_FORWARDER_CREATOR_DESC", "MA_APP_SETTINGS_DESC", "MA_START_GAME_DESC"
	};
};

#endif