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

#include "settings.hpp"

CIniFile Settings::config;
bool Settings::DirectBoot = false; // This option would directly boot into TWiLight Menu++ Last Run ROM instead of showing the ForwarDS menu.
std::string Settings::LastForwarder = ""; // The last used forwarder. Also used for CIA args by reading from it, if not 2 ARGS provided.
int Settings::Language = 0, Settings::FirstStart = 0;

void Settings::Load() {
	Settings::config = CIniFile("sdmc:/3ds/ForwarDS/config.ini");

	Settings::DirectBoot = Settings::config.GetInt("APP", "DIRECT_BOOT", Settings::DirectBoot);
	Settings::LastForwarder = Settings::config.GetString("APP", "LAST_FORWARDER", Settings::LastForwarder);
	Settings::Language = Settings::config.GetInt("APP", "LANGUAGE", Settings::Language);
	Settings::FirstStart = Settings::config.GetInt("APP", "FIRST_START", Settings::FirstStart);
}

void Settings::Save() {
	Settings::config.SetInt("APP", "DIRECT_BOOT", Settings::DirectBoot);
	Settings::config.SetString("APP", "LAST_FORWARDER", Settings::LastForwarder);
	Settings::config.SetInt("APP", "LANGUAGE", Settings::Language);
	Settings::config.SetInt("APP", "FIRST_START", Settings::FirstStart);

	Settings::config.SaveIniFile("sdmc:/3ds/ForwarDS/config.ini");
}