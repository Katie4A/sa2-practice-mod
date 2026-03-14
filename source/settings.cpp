#include "settings.h"
#include "utils.h"
static bool ppSkipToggle = 0;
static bool esgToggle = 0;
static bool egRestartLockToggle = 0;
static int restartDeathCounter = 0;
// hooks
// restart function address stolen from https://github.com/StarlitLuna/sa2-story-style-upgrades
FunctionHook<void> hRestartLevel((intptr_t)0x43C370);
// kart_delete function (thank you tenzit)
FunctionHook<void> hkart_delete((intptr_t)0x61A910);


void Settings::init() {
	hRestartLevel.Hook(restartLevel_impl);
	hkart_delete.Hook(kart_delete_impl);
}

void Settings::setESG() {
	if (esgToggle) { esgFlag = 1; }
}


void Settings::RenderTab() {
	if (ImGui::CollapsingHeader("Settings")) {
		ImGui::Checkbox("Emblem Skip Glitch", &esgToggle);
		ImGui::Checkbox("PP Skip Reset", &ppSkipToggle);
		Utils::HelpMarker("If checked, reset the variable that pushes the kart forward for 120 frames on every load/restart. Only relevant for Route 101/280.");
		ImGui::Checkbox("Lock Egg Golem Death/Restart Counter", &egRestartLockToggle);
		Utils::HelpMarker(
			"Adjusts the number of times the player has died or restarted.\n"
			"This is relevant for Egg Golem, where the boss delays itself\n"
			"depending on how many times you have died/restarted."
		);
		ImGui::SameLine();
		if (!egRestartLockToggle) {
			ImGui::BeginDisabled();
		}
		//variable's a short so this is type-safe
		ImGui::InputScalar("Deaths/Restarts", ImGuiDataType_S16, &restartDeathCounter,NULL, NULL, "%d");
		if (!egRestartLockToggle) {
			ImGui::EndDisabled();
		}
		
	}
}

void Settings::OnFrame() {
	if (egRestartLockToggle) {
		if (restartDeathCounter > 99) {
			restartDeathCounter = 99;
		} if (CurrentLevel == LevelIDs_EggGolemE || CurrentLevel == LevelIDs_EggGolemS) {
			TimesRestartedOrDied = restartDeathCounter;
		}
	}
}

void restartLevel_impl() {
	hRestartLevel.Original();
	if (ppSkipToggle) {
		ppSkipTimer = 0;
	}
}

// called when exiting the level
void kart_delete_impl() {
	hkart_delete.Original();
	if (ppSkipToggle) {
		ppSkipTimer = 0;
	}
}