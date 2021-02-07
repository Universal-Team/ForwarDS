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

void Msg::waitMsg(const std::string &msg) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("MSG_CONTINUE"), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);
	Gui::DrawStringCentered(0, 33, 0.50f, BLACK, msg, 280, 0, nullptr, C2D_WordWrap);

	C3D_FrameEnd(0);
	bool doOut = false;

	while (!doOut) {
		hidScanInput();
		uint32_t hDown = hidKeysDown();

		if (hDown & KEY_A) doOut = true;
	};
}

void Msg::displayMsg(const std::string &msg) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, msg, 360, 115, nullptr, C2D_WordWrap);

	C3D_FrameEnd(0);
}


bool Msg::prompt(const std::string &msg) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, Lang::Get("MSG_CONFIRM_CANCEL"), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);
	Gui::DrawStringCentered(0, 33, 0.50f, BLACK, msg, 280, 0, nullptr, C2D_WordWrap);

	C3D_FrameEnd(0);

	while (1) {
		hidScanInput();
		uint32_t hDown = hidKeysDown();

		if (hDown & KEY_A) return true;
		if (hDown & KEY_B) return false;
	};
}