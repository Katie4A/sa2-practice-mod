#include "settings.h"

static bool esgToggle = 0;
static bool ppSkipToggle = 0;
static bool egRestartLockToggle = 0;
static int restartDeathCounter = 0;

bool Settings::ppSkipToggleStatus() {
	return ppSkipToggle;
}

void Settings::setESG() {
	if (esgToggle) { esgFlag = 1; }
}

void Settings::RenderTab() {
	if (ImGui::CollapsingHeader("Settings")) {
		ImGui::Checkbox("Emblem Skip Glitch toggle", &esgToggle);
		ImGui::SetItemTooltip("If checked, no emblem cutscenes will be played. Note that you need certain emblems to unlock certain modes \n(i.e., to get last story you need both the Hero and Dark credits emblems.)");
		ImGui::Checkbox("PP Skip Reset", &ppSkipToggle);
		ImGui::SetItemTooltip("If checked, reset the variable that pushes the kart forward for 120 frames on every load/restart. Only relevant for Route 101/280.");
		ImGui::Checkbox("Lock Egg Golem Death/Restart Counter", &egRestartLockToggle);
		ImGui::SetItemTooltip(
			"Adjusts the number of times the player has died or restarted.\n"
			"This is relevant for Egg Golem, where the boss delays itself \n"
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

