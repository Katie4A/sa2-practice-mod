#include "settings.h"

static bool ppSkipToggle = 0;
static bool esgToggle = 0;

// hooks
// restart function address stolen from https://github.com/StarlitLuna/sa2-story-style-upgrades
FunctionHook<void> hRestartLevel((intptr_t)0x43C370);
// kart_delete function (thank you tenzit)
FunctionHook<void> hkart_delete((intptr_t)0x61A910);


void Settings::init() {
	hRestartLevel.Hook(RestartLevel);
	hkart_delete.Hook(kart_delete);
}

void Settings::setESG() {
	if (esgToggle) { esgFlag = 1; }
}

void Settings::RenderTab() {
	if (ImGui::CollapsingHeader("Settings")) {
		ImGui::Checkbox("Emblem Skip Glitch", &esgToggle);
		ImGui::Checkbox("PP Skip Reset", &ppSkipToggle);
		ImGui::SetItemTooltip("If checked, reset the variable that pushes the kart forward for 120 frames on every load/restart. Only relevant for Route 101/280.");
		//ImGui::InputInt2("Times Restarted or Died", (int*)TimesRestartedOrDied);
		//ImGui::SameLine(); HelpMarker(
		//	"Adjusts the number of times the player has died or restarted."
		//	"This is relevant for Egg Golem, where the boss delays itself"
		//);
	}
}

void RestartLevel() {
	hRestartLevel.Original();
	if (ppSkipToggle) {
		ppSkipTimer = 0;
	}
}

// called when exiting the level
void kart_delete() {
	hkart_delete.Original();
	if (ppSkipToggle) {
		ppSkipTimer = 0;
	}
}