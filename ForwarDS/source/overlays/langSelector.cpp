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
#include <vector>

const std::vector<std::string> Languages = { "English", "Deutsch" };
#define LANGS_PER_SCREEN 7

static void Draw(int curPos, int screenPos) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("AS_LANG_INSTRUCTION"), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);
	Gui::DrawStringCentered(0, 33, 0.5f, BLACK, Lang::Get("AS_SELECT_LANGUAGE"), 275);

	for(uint i = 0; i < LANGS_PER_SCREEN && i < Languages.size(); i++) {
		if (screenPos + (int)i == curPos) Gui::DrawString(18, 53 + (i * 20), 0.48f, BLACK, ">");

		Gui::DrawString(28, 53 + (i * 20), 0.48f, BLACK, Languages[screenPos + i], 275);
	}

	C3D_FrameEnd(0);
}

void Overlays::SelectLanguage() {
	int selectedLang = Settings::Language, screenPos = 0;

	while (1) {
		/* Screen draw part. */
		Draw(selectedLang, screenPos);

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		if (hDown & KEY_A) {
			Settings::Language = selectedLang;
			Lang::LoadLang(Settings::Language);
			break;
		}

		if (hRepeat & KEY_UP) {
			if (selectedLang > 0) selectedLang--;
		}
		if (hRepeat & KEY_DOWN) {
			if ((uint)selectedLang < Languages.size() - 1) selectedLang++;
		}
		if (hRepeat & KEY_LEFT) {
			if (selectedLang - LANGS_PER_SCREEN >= 0) selectedLang -= LANGS_PER_SCREEN;
			else selectedLang = 0;
		}
		if (hRepeat & KEY_RIGHT) {
			if (selectedLang + LANGS_PER_SCREEN < (int)Languages.size() - 1) selectedLang += LANGS_PER_SCREEN;
			else selectedLang = (int)Languages.size() - 1;
		}

		if (hDown & KEY_B) break;

		/* Scroll screen if needed. */
		if (selectedLang < screenPos) {
			screenPos = selectedLang;

		} else if (selectedLang > screenPos + LANGS_PER_SCREEN - 1) {
			screenPos = selectedLang - LANGS_PER_SCREEN + 1;
		}
	}
}