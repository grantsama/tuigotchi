# TUIGotchi v0.91 [WIP]
Tamagotchi-like terminal UI application, built in C using ncurses.

## Features
- **Persistent Pets**: Save system to remember your pet's state and name.
- **Tamagotchi-style Simulation**: 
  - Real-time offline degradation (pets get hungry and sad when you are away!)
  - Health penalties for neglect (sickness, extreme hunger, and thirst)
- **UI & Graphics**:
  - Animated ASCII art loaded cleanly from external files.
  - Nerd font symbols for dynamic, color-coded stat bars.
  - Keyboard interrupts for feeding, playing, and cleaning.

## TODO
- [ ] More program arguments and/or flags
- [x] Custom save file argument

# Installation
1. Clone this repo:
   `git clone https://github.com/grantsama/tuigotchi.git`
2. cd into "tuigotchi":
   `cd tuigotchi`
3. Run `make install` (You may be prompted to enter your passcode)
4. Done!

## Dependencies
- A "nerd font" installed and selected in your terminal
- git
- gcc
- make
- ncurses

# Uninstall
1. `cd` into "tuigotchi"
2. Run `make uninstall` (You may be prompted to enter your passcode)
3. Done!
