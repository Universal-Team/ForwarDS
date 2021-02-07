# ForwarDS

> Forwarders for playing DS games from your home menu or the Homebrew Launcher that you can make right on your 3DS.

## Note
You will need TWiLight Menu++ along with nds-bootstrap for ForwarDS to work.

## Features

* Manage Game Settings and cheats for your DS Games.
* Create Forwarders for use with ForwarDS directly on your console.
* Multi Language support.
* Custom background music support (You need to include a `music.wav` inside `sdmc:/3ds/ForwarDS/` and the format must be mono and under 10 MB).

## Compilation
### Setting up your enviromment

To build ForwarDS from source, you will need to setup devkitARM with libctru, Citro2d and Citro3d. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run `(sudo dkp-)pacman -S 3ds-dev`. You will also need [bannertool](https://github.com/Steveice10/bannertool/releases/latest) and [makerom](https://github.com/profi200/Project_CTR/releases/latest) in your PATH. Once you clone the repo (and its submodules), simply run `make` in the source code directory.

## Screenshots
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/appsettings.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/cheatmanager.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/credits.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/forwardercreator.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/forwarderselector.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/gamesettings.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/languageselector.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/mainscreen.png)
![](https://github.com/Universal-Team/ForwarDS/blob/main/res/screenshots/romselection.png)

## Credits
### Main Developers
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): App idea and main developer.

### Others
- [devkitPro](https://github.com/devkitPro): For devkitARM, libctru, citro2d and citro3d.
- [Pk11](https://github.com/epicpkmn11): For the name idea of ForwarDS.
- [RocketRobz](https://github.com/RocketRobz): For some old TWiLight Menu++ Launcher graphics.
- [Universal-Team](https://github.com/Universal-Team): For Universal-Core.