#include "upgraderemover.h"

static bool realTime = 0;
static bool storyUpgrades = 0;

UpgradeRemover::UpgradeRemover() {
	// initialize story style table
	// TODO: make these not magic numbers?
	this->StoryUpgradesTable = {
		{LevelIDs_CityEscape, {0, 0, 0, 0, 0, 0}}, // check header for what each bool corresponds to
		{LevelIDs_MetalHarbor, {0, 0, 0, 0, 0, 0}},
		{LevelIDs_GreenForest, {1, 0, 0, 0, 0, 0}},
		{LevelIDs_PyramidCave, {1, 0, 0, 0, 0, 0}},
		{LevelIDs_CrazyGadget, {1, 0, 0, 0, 1, 0}},
		{LevelIDs_FinalRush, {1, 0, 0, 0, 1, 0}}, // most runners skip the flame ring via a crazy gadget skip of some kind
		{LevelIDs_CannonsCoreS, {1, 0, 0, 0, 1, 0}},
		// think about adding boss stages later if you care

		{LevelIDs_PrisonLane, {0, 0, 0, 0}},
		{LevelIDs_MissionStreet, {0, 0, 0, 0}},
		{LevelIDs_HiddenBase, {1, 0, 0, 0}},
		{LevelIDs_EternalEngine, {1, 0, 0, 0}},
		{LevelIDs_CannonsCoreT, {1, 0, 0, 0}},
		
		{LevelIDs_WildCanyon, {0, 0, 0, 0, 0}},
		{LevelIDs_PumpkinHill, {0, 0, 0, 0, 0}},
		{LevelIDs_AquaticMine, {1, 0, 0, 0, 0}},
		{LevelIDs_DeathChamber, {1, 0, 0, 0, 0}},
		{LevelIDs_MeteorHerd, {1, 0, 1, 0, 0}},
		{LevelIDs_CannonsCoreK, {1, 0, 1, 0, 0}},
		
		// shadow (does not collect upgrades in a story run)
		{LevelIDs_RadicalHighway, {0, 0, 0, 0}},
		{LevelIDs_WhiteJungle, {0, 0, 0, 0}},
		{LevelIDs_SkyRail, {0, 0, 0, 0}},
		{LevelIDs_FinalChase, {0, 0, 0, 0}},

		// eggman
		{LevelIDs_IronGate, {0, 0, 0, 0, 0}},
		{LevelIDs_SandOcean, {0, 0, 0, 0, 0}},
		{LevelIDs_LostColony, {0, 0, 0, 0, 1}},
		{LevelIDs_WeaponsBed, {1, 0, 0, 0, 1}}, // mystic melody gets obtained if you do sandwalking.
		{LevelIDs_CosmicWall, {1, 0, 0, 0, 1}},
		{LevelIDs_CannonsCoreE, {1, 0, 0, 0, 1}},

		// rouge
		{LevelIDs_DryLagoon, {0, 0, 0, 0}},
		{LevelIDs_EggQuarters, {0, 0, 0, 0}},
		{LevelIDs_SecurityHall, {1, 0, 0, 0}},
		{LevelIDs_MadSpace, {1, 0, 0, 0}},
		{LevelIDs_CannonsCoreR, {1, 0, 1, 0}}, // its common for players to skip the iron boots in mad space, but let's assume they grab them anyway.
	};
}

// in sa2, characters can share the same upgrade.
// if an upgrade is enabled for both characters, the upgrade will remain until both are turned off,
// regardless of whether or not your character has that upgrade.
// the game only sets the bitfields for the character you're currently playing, so just set those instead.
void UpgradeRemover::UpdateRealTime(CharObj2Base* player) {
	if (realTime) {
		int characterBitStart = 0;
		int characterBitEnd = 0;
		// grab character ID
		char charID = player->CharID;
		// set loop start & end per character
		if (charID == Characters_Sonic) { characterBitStart = 0, characterBitEnd = 6; }
		if (charID == Characters_MechTails) { characterBitStart = 6, characterBitEnd = 10; }
		if (charID == Characters_Knuckles) { characterBitStart = 10; characterBitEnd = 15; }
		if (charID == Characters_Shadow) { characterBitStart = 16; characterBitEnd = 20; }
		if (charID == Characters_MechEggman) { characterBitStart = 20; characterBitEnd = 25; }
		if (charID == Characters_Rouge) { characterBitStart = 25; characterBitEnd = 29; }

		int upgrades = 0;
		// UpgradesOnFile is an array of 29 booleans, set all the upgrade bits for the character you're playing
		// i figured this would be more efficient then writing out every upgrade bitfield manually
		for (int i = characterBitStart; i < characterBitEnd; i++) {
			upgrades = upgrades | (UpgradesOnFile[i] << i);
		}
		player->Upgrades = upgrades;
	}
}

void UpgradeRemover::SetStoryUpgrades(short currentLevel, CharObj2Base* player) {
	if (storyUpgrades) {
		if (this->StoryUpgradesTable.count(currentLevel) > 0) {
			char charID = player->CharID;
			std::vector<bool> upgrades;
			// grab upgrades from table
			upgrades = this->StoryUpgradesTable[currentLevel];

			if (charID == Characters_Sonic) { SetSonicUpgrades(upgrades); }
			else if (charID == Characters_MechTails) { SetTailsUpgrades(upgrades); }
			else if (charID == Characters_Knuckles) { SetKnuxUpgrades(upgrades); }
			else if (charID == Characters_Shadow) { SetShadowUpgrades(upgrades); }
			else if (charID == Characters_MechEggman) { SetEggmanUpgrades(upgrades); }
			else if (charID == Characters_Rouge) { SetRougeUpgrades(upgrades); }

			// TODO: update real time somewhere else
			this->UpdateRealTime(player);
		}
	}
}

void UpgradeRemover::SetSonicUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		SonicLightShoesGot = upgrades[LightShoes];
		SonicAncientLightGot = upgrades[Sonic_AncientLight];
		SonicMagicGlovesGot = upgrades[MagicGloves];
		SonicFlameRingGot = upgrades[Sonic_FlameRing];
		SonicBounceBraceletGot = upgrades[BounceBracelet];
		SonicMysticMelodyGot = upgrades[Sonic_MysticMelody];
	}
}

void UpgradeRemover::SetTailsUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		TailsBoosterGot = upgrades[Booster];
		TailsBazookaGot = upgrades[Bazooka];
		TailsLaserBlasterGot = upgrades[Tails_LaserBlaster];
		TailsMysticMelodyGot = upgrades[Tails_MysticMelody];
	}
}

void UpgradeRemover::SetKnuxUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		KnucklesShovelClawGot = upgrades[ShovelClaws];
		KnucklesSunglassesGot = upgrades[Sunglasses];
		KnucklesHammerGlovesGot = upgrades[HammerGloves];
		KnucklesAirNecklaceGot = upgrades[AirNecklace];
		KnucklesMysticMelodyGot = upgrades[Knux_MysticMelody];
	}
}

void UpgradeRemover::SetShadowUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		ShadowAirShoesGot = upgrades[AirShoes];
		ShadowAncientLightGot= upgrades[Shadow_AncientLight];
		ShadowFlameRingGot = upgrades[Shadow_FlameRing];
		ShadowMysticMelodyGot = upgrades[Shadow_MysticMelody];
	}
}

void UpgradeRemover::SetEggmanUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		EggmanJetEngineGot = upgrades[JetEngine];
		EggmanLargeCannonGot = upgrades[LargeCannon];
		EggmanLaserBlasterGot = upgrades[Eggman_LaserBlaster];
		EggmanProtectiveArmorGot = upgrades[ProtectiveArmor];
		EggmanMysticMelodyGot = upgrades[Eggman_MysticMelody];
	}
}

void UpgradeRemover::SetRougeUpgrades(std::vector<bool> upgrades) {
	if (!upgrades.empty()) {
		RougePickNailsGot = upgrades[PickNails];
		RougeTreasureScopeGot = upgrades[TreasureScope];
		RougeIronBootsGot = upgrades[IronBoots];
		RougeMysticMelodyGot = upgrades[Rouge_MysticMelody];
	}

}

void UpgradeRemover::RenderTab() {
	if (ImGui::CollapsingHeader("Upgrades")) {
		if (ImGui::BeginTable("", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Real-time Updates", &realTime);
			ImGui::SetItemTooltip("If checked, upgrade changes will immediately be reflected in-game.");
			ImGui::TableNextColumn(); ImGui::Checkbox("Story Upgrades", &storyUpgrades);
			ImGui::SetItemTooltip("If checked, upon entering a stage or restarting while holding Y, your upgrades will be set to match story conditions.");
			ImGui::EndTable();
		}
		SonicTab();
		TailsTab();
		KnucklesTab();
		ShadowTab();
		EggmanTab();
		RougeTab();
	}
}

void UpgradeRemover::SonicTab() {
	if (ImGui::TreeNode("Sonic")) {
		if (ImGui::BeginTable("Sonic", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Light Shoes", &SonicLightShoesGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Ancient Light", &SonicAncientLightGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Magic Gloves", &SonicMagicGlovesGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flame Ring", &SonicFlameRingGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Bounce Bracelet", &SonicBounceBraceletGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &SonicMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::TailsTab() {
	if (ImGui::TreeNode("Tails")) {
		if (ImGui::BeginTable("Tails", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Booster", &TailsBoosterGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Bazooka", &TailsBazookaGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Laser Blaster", &TailsLaserBlasterGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &TailsMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::KnucklesTab() {
	if (ImGui::TreeNode("Knuckles")) {
		if (ImGui::BeginTable("Knuckles", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Shovel Claw", &KnucklesShovelClawGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Sunglasses", &KnucklesSunglassesGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Hammer Gloves", &KnucklesHammerGlovesGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Air Necklace", &KnucklesAirNecklaceGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &KnucklesMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::ShadowTab() {
	if (ImGui::TreeNode("Shadow")) {
		if (ImGui::BeginTable("Shadow", 2)) {
			// note: shadow skips a byte???
			ImGui::TableNextColumn(); ImGui::Checkbox("Air Shoes", &ShadowAirShoesGot); 
			ImGui::TableNextColumn(); ImGui::Checkbox("Ancient Light", &ShadowAncientLightGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flame Ring", &ShadowFlameRingGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &ShadowMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::EggmanTab() {
	if (ImGui::TreeNode("Eggman")) {
		if (ImGui::BeginTable("Eggman", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Jet Engine", &EggmanJetEngineGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Large Cannon", &EggmanLargeCannonGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Laser Blaster", &EggmanLaserBlasterGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Protective Armor", &EggmanProtectiveArmorGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &EggmanMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::RougeTab() {
	if (ImGui::TreeNode("Rouge")) {
		if (ImGui::BeginTable("Rouge", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Pick Nails", &RougePickNailsGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Treasure Scope", &RougeTreasureScopeGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Iron Boots", &RougeIronBootsGot);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &RougeMysticMelodyGot);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}