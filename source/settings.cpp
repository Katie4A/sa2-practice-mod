#include "settings.h"


static bool ppSkipToggle = 0;
static bool esgToggle = 0;
static bool egRestartLockToggle = 0;
static bool boxGlitchFixToggle = 0;
static bool forceCkgWarpToggle = 0;
static short restartDeathCounter = 0;
static const CkgLevelEntry* selectedCkgLevel = &ckgLevelNameMap[0];

static FunctionHook<void, ObjectMaster*> CrateObjectCollisionHelper(0x6B9D60);

bool Settings::ppSkipToggleStatus() {
	return ppSkipToggle;
}

bool Settings::NeedsFrameTick() const {
	return egRestartLockToggle || forceCkgWarpToggle;
}

void Settings::setESG() {
	if (esgToggle) { esgFlag = 1; }
}

void Settings::Init() {
	CrateObjectCollisionHelper.Hook(Settings::CrateObjectCollisionHelper_Hook);
}

void Settings::RenderTab() {
	if (ImGui::CollapsingHeader("Settings")) {
		ImGui::Checkbox("Emblem Skip Glitch toggle", &esgToggle);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, no emblem cutscenes will be played. Note that you need certain emblems to unlock certain modes \n(i.e., to unlock last story you need both the Hero and Dark credits emblems.)");

		ImGui::Checkbox("PP Skip Reset", &ppSkipToggle);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, the variable that pushes the kart forward for 122 on first load will be reset on restart/subsequent load.");
		ImGui::Checkbox("Fix Box Glitch", &boxGlitchFixToggle);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, box glitch will be fixed even if it's currently affecting your game.");

		ImGui::Checkbox("Lock Egg Golem Death/Restart Counter", &egRestartLockToggle);
		ImGui::SameLine();
		Utils::HelpMarker(
			"Adjusts the number of times the player has died or restarted.\n"
			"This is relevant for Egg Golem, where the boss delays itself depending on how many times you've died/restarted."
		);
		if (!egRestartLockToggle) {
			ImGui::BeginDisabled();
		}
		//variable's a short so this is type-safe
		if (ImGui::InputScalar("Deaths/Restarts", ImGuiDataType_S16, &restartDeathCounter, NULL, NULL, "%d")) {
			if (restartDeathCounter < 0) {
				restartDeathCounter = 0;
			} else if (restartDeathCounter > 99) {
				restartDeathCounter = 99;
			}
		}

		if (!egRestartLockToggle) {
			ImGui::EndDisabled();
		}

		ImGui::Checkbox("Force CKG Warp Destination", &forceCkgWarpToggle);
		ImGui::SameLine();
		Utils::HelpMarker(
			"Forces the next CKG to warp you to the selected stage."
		);
		if (!forceCkgWarpToggle) {
			ImGui::BeginDisabled();
		}

		if (ImGui::BeginCombo("CKG Stage", selectedCkgLevel->name)) {
			for (const CkgLevelEntry& entry : ckgLevelNameMap) {
				const bool isSelected = (selectedCkgLevel == &entry);
				if (ImGui::Selectable(entry.name, isSelected))
					selectedCkgLevel = &entry;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (!forceCkgWarpToggle) {
			ImGui::EndDisabled();
		}
	}
}

void Settings::OnFrame() {
	if (egRestartLockToggle) {
		if ((CurrentLevel == LevelIDs_EggGolemE || CurrentLevel == LevelIDs_EggGolemS) && TimesRestartedOrDied != restartDeathCounter) {
			TimesRestartedOrDied = restartDeathCounter;
		}
	}

	if (forceCkgWarpToggle) {
		NextLevel = static_cast<short>(selectedCkgLevel->level);
	}
}

/**
 * If colliding with stacked crates, apply no contact flag to child crate object.
 * This prevents the child "crate falling" invisible object from being able to be registered with
 * the character as an object that forces a turn without messing with normal forced-turn physics.
 * 
 * @author Luna
 * @see https://github.com/StarlitLuna/sa2-box-glitch-fix
 * @copyright GPL-3.0-or-later
 */
void Settings::CrateObjectCollisionHelper_Hook(ObjectMaster* obj) {
	if (boxGlitchFixToggle && obj && obj->Data1.Entity) {
		EntityData1 *data1 = obj->Data1.Entity;
		CollisionInfo *collisionInfo = data1->Collision;
		if (!collisionInfo || !collisionInfo->CollisionArray) {
			return CrateObjectCollisionHelper.Original(obj);
		}

		for (size_t i = 0; i < collisionInfo->Count; i++) {
			CollisionData *shape = &collisionInfo->CollisionArray[i];
			if ((shape->attr & Settings::NoContactChildFlag) != 0) {
				continue;
			}

			shape->attr |= Settings::NoContactChildFlag;
		}
	}

	CrateObjectCollisionHelper.Original(obj);
}