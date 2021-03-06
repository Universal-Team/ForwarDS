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


#ifndef _FORWARDS_GFX_HPP
#define _FORWARDS_GFX_HPP

#include "common.hpp"
#include <string>

#define GRAY C2D_Color32(127, 127, 127, 150)
#define BLACK C2D_Color32(0, 0, 0, 255)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define GREEN C2D_Color32(0, 200, 0, 255)
#define RED C2D_Color32(200, 0, 0, 255)
#define TRANSPARENT C2D_Color32(0, 0, 0, 0)

namespace GFX {
	void DrawImageTint(int idx, int x, int y, uint32_t clrTint);
};

#endif