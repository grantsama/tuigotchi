# TUIGotchi [WIP]
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
- [ ] Make improvements?

# Installation
1. Clone this repo
2. `cd` into "tuigotchi"
3. Run `make install` (You may be prompted to enter your passcode)
4. Done!

## Dependencies
- gcc
- make
- ncurses

# Uninstall
1. `cd` into "tuigotchi"
2. Run `make uninstall` (You may be prompted to enter your passcode)
3. Done!
