#include "SaveState.h"
int SAVESLOT_LIMIT = 10;

// this is heavily, HEAVILY based off of OnVars-Tool by OnVar.
// thank you OnVar for making an incredible savestating tool.
// https://github.com/Isaac-Lozano/OnVars-Tool
// as a consequence, a lot of the savestate code is similar to Sora's Debug Mode mod
// https://github.com/Sora-yx/SA2-Debug-Mode

// NOTE: savestates are invalidated when you go to the main menu

// NOTE: savestates are loaded twice, once initially and again for collision stuff
// this is a workaround for collision issues that occur when editing the player's position
// in a perfect world, we'd save collision data, but thats simply not going to happen.

SaveStates::SaveStates() {

}

void SaveStates::SaveState(int saveSlot) {

}

void SaveStates::LoadState(int saveSlot) {

}

void SaveStates::OnFrame() {
	if (GameState == GameStates_Inactive) {

	}
}

void SaveStates::OnInput() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause) {
		int buttons = ControllerPointers[0]->on;
		int buttonsPressed = ~this->prevButtons & buttons;
		this->prevButtons = buttons; // if bit = 1, prevButtons will be negated to prevent double presses.

		if (buttons & Buttons_Left) {
			this->SaveState(this->saveSlot);
		}
		else if (buttons & Buttons_Right) {
			this->LoadState(this->saveSlot);
		}
		else if (buttons & Buttons_Up) {
			this->saveSlot = (this->saveSlot + 1) % SAVESLOT_LIMIT;
		}
		else if (buttons & Buttons_Down) {
			this->saveSlot = (this->saveSlot - 1) % SAVESLOT_LIMIT;
		}
	}
}
