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
#include "fileBrowse.hpp"
#include <dirent.h>
#include <unistd.h>
#include <vector>

#define FILES_PER_SCREEN 7

static void Draw(const std::vector<DirEntry> &dirContents, int curPos, int screenPos, const std::string &text, const std::string &path, bool canBack = true) {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	Gui::ScreenDraw(Top);
	Gui::DrawSprite(sprites, sprites_top_idx, 0, 0);
	Gui::DrawStringCentered(0, 110, 0.7f, WHITE, (canBack ? Lang::Get("FS_INSTRUCTION_2") : Lang::Get("FS_INSTRUCTION_1")), 360, 115, nullptr, C2D_WordWrap);

	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(sprites, sprites_bottom_idx, 0, 0);
	Gui::DrawSprite(sprites, sprites_box_idx, 0, 0);
	Gui::DrawStringCentered(0, 33, 0.5f, BLACK, text, 275);

	for(uint i = 0; i < FILES_PER_SCREEN && i < dirContents.size(); i++) {
		if (screenPos + (int)i == curPos) Gui::DrawString(18, 53 + (i * 20), 0.48f, BLACK, ">");

		Gui::DrawString(28, 53 + (i * 20), 0.48f, BLACK, dirContents[screenPos + i].name, 275);
	}

	Gui::DrawString(18, 53 + (7 * 20), 0.48f, BLACK, Lang::Get("FS_PATH") + path, 285);

	C3D_FrameEnd(0);
}

/* Normal file selection from the SD Card, with folder back and so on ability. */
std::string Overlays::SelectFile(const std::string &startPath, const std::vector<std::string> &ext, const std::string &text) {
	int selectedFile = 0, screenPos = 0;
	bool dirChanged = false;
	std::string currentPath = startPath;
	std::vector<DirEntry> dirContents;

	/* Initial dir change. */
	dirContents.clear();
	chdir(startPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, ext);

	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		Draw(dirContents, selectedFile, screenPos, text, currentPath, true);

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, ext);

			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if (hDown & KEY_A) {
			if (!dirContents.empty()) { // Ensure the directory contents aren't empty to not cause crashes.
				if (dirContents[selectedFile].isDirectory) {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);

					currentPath = path + dirContents[selectedFile].name;
					chdir(dirContents[selectedFile].name.c_str());
					selectedFile = 0;
					dirChanged = true;

				} else {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					return path + dirContents[selectedFile].name;
				}
			}
		}


		if (hRepeat & KEY_UP) {
			if (selectedFile > 0) selectedFile--;
		}

		if (hRepeat & KEY_DOWN) {
			if (selectedFile < (int)dirContents.size() - 1) selectedFile++;
		}

		if (hRepeat & KEY_LEFT) {
			if (selectedFile - FILES_PER_SCREEN >= 0) selectedFile -= FILES_PER_SCREEN;
			else selectedFile = 0;
		}

		if (hRepeat & KEY_RIGHT) {
			if (selectedFile + FILES_PER_SCREEN < (int)dirContents.size() - 1) selectedFile += FILES_PER_SCREEN;
			else selectedFile = (int)dirContents.size() - 1;
		}

		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) return "";
			else {
				chdir("..");
				selectedFile = 0;
				dirChanged = true;

				getcwd(path, PATH_MAX);
				currentPath = path;
			}
		}

		if (hDown & KEY_START) dirChanged = true;

		/* Scroll screen if needed. */
		if (selectedFile < screenPos) {
			screenPos = selectedFile;

		} else if (selectedFile > screenPos + FILES_PER_SCREEN - 1) {
			screenPos = selectedFile - FILES_PER_SCREEN + 1;
		}
	}
}

/* File Selection.. without the ability to go back. */
std::string Overlays::SelectFileNoBack(const std::string &startPath, const std::vector<std::string> &ext, const std::string &text) {
	int selectedFile = 0, screenPos = 0;
	bool dirChanged = false;
	std::string currentPath = startPath;
	std::vector<DirEntry> dirContents;

	/* Initial dir change. */
	dirContents.clear();
	chdir(startPath.c_str());
	std::vector<DirEntry> dirContentsTemp;

	getDirectoryContents(dirContentsTemp, ext);
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		Draw(dirContents, selectedFile, screenPos, text, currentPath, false);

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, ext);

			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if (hDown & KEY_A) {
			if (!dirContents.empty()) {
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				return path + dirContents[selectedFile].name;
			}
		}

		if (hRepeat & KEY_UP) {
			if (selectedFile > 0) selectedFile--;
		}

		if (hRepeat & KEY_DOWN) {
			if (selectedFile < (int)dirContents.size() - 1) selectedFile++;
		}

		if (hRepeat & KEY_LEFT) {
			if (selectedFile - FILES_PER_SCREEN >= 0) selectedFile -= FILES_PER_SCREEN;
			else selectedFile = 0;
		}

		if (hRepeat & KEY_RIGHT) {
			if (selectedFile + FILES_PER_SCREEN < (int)dirContents.size() - 1) selectedFile += FILES_PER_SCREEN;
			else selectedFile = (int)dirContents.size() - 1;
		}

		if (hDown & KEY_B) return "";
		if (hDown & KEY_START) dirChanged = true;

		/* Scroll screen if needed. */
		if (selectedFile < screenPos) {
			screenPos = selectedFile;

		} else if (selectedFile > screenPos + FILES_PER_SCREEN - 1) {
			screenPos = selectedFile - FILES_PER_SCREEN + 1;
		}
	}
}