#include "upgraderemover.h"

namespace {
	struct UpgradeBitRange {
		int start;
		int end;
	};

	struct StoryUpgradeEntry {
		short level;
		int mask;
	};

	constexpr int UpgradeObjectFlagCount = UpgradeBits_RougeMysticMelody + 1;
	static_assert(UpgradeObjectFlagCount == 29, "Unexpected SA2 upgrade flag count.");

	FunctionHook<int, int> levelItemMainHook((intptr_t)LevelItem_Main);
	bool upgradeObjectFlags[UpgradeObjectFlagCount] = {};
	bool upgradeObjectHooksInitialized = false;
	const void* jumpBackToLevelItemLoad = (void*)0x6D8653;
	const void* jumpBackToLevelItemUpgradeLoad = (void*)0x6D865F;

	constexpr StoryUpgradeEntry StoryUpgradeMasks[] = {
		// Sonic story stages. Most runners skip Flame Ring via a Crazy Gadget skip of some kind.
		{ LevelIDs_CityEscape, 0 },
		{ LevelIDs_MetalHarbor, 0 },
		{ LevelIDs_GreenForest, Upgrades_SonicLightShoes },
		{ LevelIDs_PyramidCave, Upgrades_SonicLightShoes },
		{ LevelIDs_CrazyGadget, Upgrades_SonicLightShoes | Upgrades_SonicBounceBracelet },
		{ LevelIDs_FinalRush, Upgrades_SonicLightShoes | Upgrades_SonicBounceBracelet },
		{ LevelIDs_CannonsCoreS, Upgrades_SonicLightShoes | Upgrades_SonicBounceBracelet },
		// Consider boss stages later

		// Tails story stages.
		{ LevelIDs_PrisonLane, 0 },
		{ LevelIDs_MissionStreet, 0 },
		{ LevelIDs_HiddenBase, Upgrades_TailsBooster },
		{ LevelIDs_EternalEngine, Upgrades_TailsBooster },
		{ LevelIDs_CannonsCoreT, Upgrades_TailsBooster },

		// Knuckles story stages.
		{ LevelIDs_WildCanyon, 0 },
		{ LevelIDs_PumpkinHill, 0 },
		{ LevelIDs_AquaticMine, Upgrades_KnucklesShovelClaw },
		{ LevelIDs_DeathChamber, Upgrades_KnucklesShovelClaw },
		{ LevelIDs_MeteorHerd, Upgrades_KnucklesShovelClaw | Upgrades_KnucklesHammerGloves },
		{ LevelIDs_CannonsCoreK, Upgrades_KnucklesShovelClaw | Upgrades_KnucklesHammerGloves },

		// Shadow does not collect upgrades in a story run.
		{ LevelIDs_RadicalHighway, 0 },
		{ LevelIDs_WhiteJungle, 0 },
		{ LevelIDs_SkyRail, 0 },
		{ LevelIDs_FinalChase, 0 },

		// Eggman story stages. Mystic Melody is obtained if you do sandwalking.
		{ LevelIDs_IronGate, 0 },
		{ LevelIDs_SandOcean, 0 },
		{ LevelIDs_LostColony, Upgrades_EggmanMysticMelody },
		{ LevelIDs_WeaponsBed, Upgrades_EggmanJetEngine | Upgrades_EggmanMysticMelody },
		{ LevelIDs_CosmicWall, Upgrades_EggmanJetEngine | Upgrades_EggmanMysticMelody },
		{ LevelIDs_CannonsCoreE, Upgrades_EggmanJetEngine | Upgrades_EggmanMysticMelody },

		// Rouge story stages. Players often skip Iron Boots in Mad Space; assume they grab them anyway.
		{ LevelIDs_DryLagoon, 0 },
		{ LevelIDs_EggQuarters, 0 },
		{ LevelIDs_SecurityHall, Upgrades_RougePickNails },
		{ LevelIDs_MadSpace, Upgrades_RougePickNails },
		{ LevelIDs_CannonsCoreR, Upgrades_RougePickNails | Upgrades_RougeIronBoots },
	};

	bool TryGetUpgradeBitRange(const char charID, UpgradeBitRange& range) {
		switch (charID) {
			case Characters_Sonic:
				range = { UpgradeBits_SonicLightShoes, UpgradeBits_SonicMysticMelody + 1 };
				return true;
			case Characters_MechTails:
				range = { UpgradeBits_TailsBooster, UpgradeBits_TailsMysticMelody + 1 };
				return true;
			case Characters_Knuckles:
				range = { UpgradeBits_KnucklesShovelClaw, UpgradeBits_KnucklesMysticMelody + 1 };
				return true;
			case Characters_Shadow:
				range = { UpgradeBits_ShadowAirShoes, UpgradeBits_ShadowMysticMelody + 1 };
				return true;
			case Characters_MechEggman:
				range = { UpgradeBits_EggmanJetEngine, UpgradeBits_EggmanMysticMelody + 1 };
				return true;
			case Characters_Rouge:
				range = { UpgradeBits_RougePickNails, UpgradeBits_RougeMysticMelody + 1 };
				return true;
			default:
				return false;
		}
	}

	void SyncAllUpgradeObjectFlagsFromSaveFlags() {
		for (int i = 0; i < UpgradeObjectFlagCount; ++i) {
			upgradeObjectFlags[i] = UpgradesOnFile[i];
		}
	}

	void SyncUpgradeObjectFlagRange(const UpgradeBitRange& range) {
		for (int i = range.start; i < range.end && i < UpgradeObjectFlagCount; ++i) {
			upgradeObjectFlags[i] = UpgradesOnFile[i];
		}
	}

	void SyncUpgradeObjectFlagsForCharacter(const char charID) {
		UpgradeBitRange range = {};
		if (TryGetUpgradeBitRange(charID, range)) {
			SyncUpgradeObjectFlagRange(range);
		}
	}

	void MarkCollectedUpgradeObjectsFromMask(const int mask) {
		for (int i = 0; i < UpgradeObjectFlagCount; ++i) {
			if ((mask & (1 << i)) != 0) {
				UpgradesOnFile[i] = true;
				upgradeObjectFlags[i] = true;
			}
		}
	}

	bool CharacterHasUpgrade(const unsigned int upgrade) {
		if (upgrade >= UpgradeObjectFlagCount) {
			return false;
		}

		return upgradeObjectFlags[upgrade];
	}

	__declspec(naked) void UpgradeItemComparison() {
		__asm {
			push	eax
			call	CharacterHasUpgrade
			cmp		eax, 0
			pop		eax
			pop		edi
			jz		UPGRADE_RETURN
			jmp		jumpBackToLevelItemLoad

UPGRADE_RETURN:
			jmp		jumpBackToLevelItemUpgradeLoad
		}
	}

	int LevelItemMain_Hook(int object) {
		CharObj2Base* player = MainCharObj2[0];
		const bool hadPlayer = player != nullptr;
		const int previousUpgrades = player != nullptr ? player->Upgrades : 0;
		const int result = levelItemMainHook.Original(object);

		SyncAllUpgradeObjectFlagsFromSaveFlags();

		player = MainCharObj2[0];
		if (hadPlayer && player != nullptr) {
			MarkCollectedUpgradeObjectsFromMask(player->Upgrades & ~previousUpgrades);
		}

		return result;
	}

	// In SA2, some upgrades are shared by multiple characters in save-file state.
	// If an upgrade is enabled for both characters, it remains on until both are off,
	// regardless of whether the currently loaded character can use that upgrade.
	// The game only sets bitfields for the character you're playing, so do the same.
	int BuildCurrentUpgradeMask(const char charID) {
		UpgradeBitRange range = {};
		if (!TryGetUpgradeBitRange(charID, range)) {
			return 0;
		}

		// UpgradesOnFile is an array of 29 booleans. Convert only the current
		// character's save-file range into the player object's upgrade bitfield.
		int mask = 0;
		for (int i = range.start; i < range.end; ++i) {
			if (UpgradesOnFile[i]) {
				mask |= 1 << i;
			}
		}

		return mask;
	}

	void WriteUpgradeFileFlagsForMask(const char charID, const int mask) {
		UpgradeBitRange range = {};
		if (!TryGetUpgradeBitRange(charID, range)) {
			return;
		}

		for (int i = range.start; i < range.end; ++i) {
			const bool hasUpgrade = (mask & (1 << i)) != 0;
			UpgradesOnFile[i] = hasUpgrade;
			upgradeObjectFlags[i] = hasUpgrade;
		}
	}

	bool TryGetStoryMask(const short level, int& mask) {
		for (const StoryUpgradeEntry& entry : StoryUpgradeMasks) {
			if (entry.level == level) {
				mask = entry.mask;
				return true;
			}
		}

		return false;
	}
}

void UpgradeRemover::InitUpgradeObjectHooks() {
	if (upgradeObjectHooksInitialized) {
		return;
	}

	SyncAllUpgradeObjectFlagsFromSaveFlags();
	WriteJump((void*)0x6D8649, &UpgradeItemComparison);
	for (unsigned short i = 0; i < 5; ++i) {
		WriteData<1>((void*)(0x6D864E + i), 0x90u);
	}

	levelItemMainHook.Hook(LevelItemMain_Hook);
	upgradeObjectHooksInitialized = true;
}

void UpgradeRemover::QueueStoryRestartReset() {
	if (!storyUpgrades) {
		return;
	}

	storyRestartResetQueued = true;
}

void UpgradeRemover::OnControl() {
	// Inputs during restart are awkward: assume holding Y at any point during the restart
	// state means the player wants story upgrades restored once the character is initialized.
	if (GameState == GameStates_NormalRestart && ControllerPointers[0] != nullptr && (ControllerPointers[0]->on & Buttons_Y)) {
		QueueStoryRestartReset();
	}
}

void UpgradeRemover::OnPlayerInit(CharObj2Base* player) {
	if (player == nullptr) {
		return;
	}

	SyncUpgradeObjectFlagsForCharacter(player->CharID);

	const bool appliedStoryUpgrades = ApplyStoryUpgrades(CurrentLevel, player);
	if (appliedStoryUpgrades) {
		storyRestartResetQueued = false;
		return;
	}

	if (realTime) {
		ApplyCurrentUpgradeMask(player);
	}
}

bool UpgradeRemover::ApplyCurrentUpgradeMask(CharObj2Base* player) {
	if (player == nullptr) {
		return false;
	}

	UpgradeBitRange range = {};
	if (!TryGetUpgradeBitRange(player->CharID, range)) {
		return false;
	}

	SyncUpgradeObjectFlagRange(range);

	const int upgrades = BuildCurrentUpgradeMask(player->CharID);
	if (player->Upgrades != upgrades) {
		player->Upgrades = upgrades;
	}

	return true;
}

bool UpgradeRemover::ApplyStoryUpgrades(const short currentLevel, CharObj2Base* player) {
	if (!storyUpgrades || player == nullptr) {
		return false;
	}

	int upgrades = 0;
	if (!TryGetStoryMask(currentLevel, upgrades)) {
		return false;
	}

	WriteUpgradeFileFlagsForMask(player->CharID, upgrades);
	return ApplyCurrentUpgradeMask(player);
}

void UpgradeRemover::ApplyPendingRestartUpgradeReset(CharObj2Base* player) {
	if (!storyRestartResetQueued) {
		return;
	}

	storyRestartResetQueued = false;
	ApplyStoryUpgrades(CurrentLevel, player);
}

void UpgradeRemover::RenderTab() {
	if (ImGui::CollapsingHeader("Upgrades")) {
		bool applyRealTimeUpgrades = false;
		bool upgradeStateChanged = false;

		if (ImGui::BeginTable("", 2)) {
			ImGui::TableNextColumn();
			if (ImGui::Checkbox("Real-time Updates", &realTime) && realTime) {
				applyRealTimeUpgrades = true;
			}

			ImGui::SameLine();
			Utils::HelpMarker("If checked, upgrade changes will immediately be reflected in-game.");
			ImGui::TableNextColumn();
			ImGui::Checkbox("Story Upgrades", &storyUpgrades);
			ImGui::SameLine();
			Utils::HelpMarker("If checked, upon entering a stage or restarting while holding Y, your upgrades will be set to match story conditions.");
			ImGui::EndTable();
		}

		const ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
			if (ImGui::BeginTabItem("Sonic")) {
				const bool changed = SonicTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Tails")) {
				const bool changed = TailsTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Knuckles")) {
				const bool changed = KnucklesTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Shadow")) {
				const bool changed = ShadowTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Eggman")) {
				const bool changed = EggmanTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Rouge")) {
				const bool changed = RougeTab();
				upgradeStateChanged |= changed;
				applyRealTimeUpgrades |= changed;
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (upgradeStateChanged) {
			SyncAllUpgradeObjectFlagsFromSaveFlags();
		}

		if (applyRealTimeUpgrades && realTime) {
			ApplyCurrentUpgradeMask(MainCharObj2[0]);
		}
	}
}

bool UpgradeRemover::SonicTab() {
	bool changed = false;

	if (ImGui::BeginTable("Sonic", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Light Shoes", &SonicLightShoesGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Ancient Light", &SonicAncientLightGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Magic Gloves", &SonicMagicGlovesGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Flame Ring", &SonicFlameRingGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Bounce Bracelet", &SonicBounceBraceletGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &SonicMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}

bool UpgradeRemover::TailsTab() {
	bool changed = false;

	if (ImGui::BeginTable("Tails", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Booster", &TailsBoosterGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Bazooka", &TailsBazookaGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Laser Blaster", &TailsLaserBlasterGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &TailsMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}

bool UpgradeRemover::KnucklesTab() {
	bool changed = false;

	if (ImGui::BeginTable("Knuckles", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Shovel Claw", &KnucklesShovelClawGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Sunglasses", &KnucklesSunglassesGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Hammer Gloves", &KnucklesHammerGlovesGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Air Necklace", &KnucklesAirNecklaceGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &KnucklesMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}

bool UpgradeRemover::ShadowTab() {
	bool changed = false;

	if (ImGui::BeginTable("Shadow", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Air Shoes", &ShadowAirShoesGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Ancient Light", &ShadowAncientLightGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Flame Ring", &ShadowFlameRingGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &ShadowMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}

bool UpgradeRemover::EggmanTab() {
	bool changed = false;

	if (ImGui::BeginTable("Eggman", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Jet Engine", &EggmanJetEngineGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Large Cannon", &EggmanLargeCannonGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Laser Blaster", &EggmanLaserBlasterGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Protective Armor", &EggmanProtectiveArmorGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &EggmanMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}

bool UpgradeRemover::RougeTab() {
	bool changed = false;

	if (ImGui::BeginTable("Rouge", 2)) {
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Pick Nails", &RougePickNailsGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Treasure Scope", &RougeTreasureScopeGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Iron Boots", &RougeIronBootsGot);
		ImGui::TableNextColumn(); changed |= ImGui::Checkbox("Mystic Melody", &RougeMysticMelodyGot);
		ImGui::EndTable();
	}

	return changed;
}
