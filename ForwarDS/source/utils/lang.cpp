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

#include "json.hpp"
#include "lang.hpp"
#include "settings.hpp"

static nlohmann::json appJson;
const std::vector<std::string> Languages = { "en", "de" };

/* Load the Language strings. */
void Lang::LoadLang(const uint8_t L) {
	/* Ensure, the language index is not outside of the vector. */
	if (L <= Languages.size() - 1) {
		FILE *values = fopen(("romfs:/lang/" + Languages[L] + "/app.json").c_str(), "rt");
		appJson = nlohmann::json::parse(values, nullptr, false);
		fclose(values);

	} else {
		Settings::Language = 0; // Reset Language to 0 (English), cause invalid.
		FILE *values = fopen("romfs:/lang/en/app.json", "rt");
		appJson = nlohmann::json::parse(values, nullptr, false);
		fclose(values);
	}
}

/* Get a string. */
std::string Lang::Get(const std::string &key) {
	if (!appJson.contains(key)) return key;

	return appJson.at(key).get_ref<const std::string &>();
}