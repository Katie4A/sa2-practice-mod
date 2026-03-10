#pragma once
#include "pch.h"
#include <imgui.h>
#include <map>
#include <vector>

enum SonicUpgrades : int {
	LightShoes,
	Sonic_AncientLight,
	MagicGloves,
	Sonic_FlameRing,
	BounceBracelet,
	Sonic_MysticMelody
};

enum TailsUpgrades : int {
	Booster,
	Bazooka,
	Tails_LaserBlaster,
	Tails_MysticMelody
};

enum KnuxUpgrades : int {
	ShovelClaws,
	Sunglasses,
	HammerGloves,
	AirNecklace,
	Knux_MysticMelody
};

enum ShadowUpgrades : int {
	AirShoes,
	Shadow_AncientLight,
	Shadow_FlameRing,
	Shadow_MysticMelody
};

enum EggmanUpgrades : int {
	JetEngine,
	LargeCannon,
	Eggman_LaserBlaster,
	ProtectiveArmor,
	Eggman_MysticMelody
};

enum RougeUpgrades : int {
	PickNails,
	TreasureScope,
	IronBoots,
	Rouge_MysticMelody
};

// upgrade booleans stored on file (there are 29 bytes to update)
// thank you Emerua for making UpgradeRemover & finding this address
DataArray(bool, UpgradesOnFile, 0x1DEB300, 29);

class UpgradeRemover {
	public:
		UpgradeRemover();
		void UpdateRealTime(CharObj2Base* player);
		void SetStoryUpgrades(short currentLevel, CharObj2Base* player);
		void SetSonicUpgrades(std::vector<bool> upgrades);
		void SetTailsUpgrades(std::vector<bool> upgrades);
		void SetKnuxUpgrades(std::vector<bool> upgrades);
		void SetShadowUpgrades(std::vector<bool> upgrades);
		void SetEggmanUpgrades(std::vector<bool> upgrades);
		void SetRougeUpgrades(std::vector<bool> upgrades);
		void RenderTab();
		void SonicTab();
		void TailsTab();
		void KnucklesTab();
		void ShadowTab();
		void EggmanTab();
		void RougeTab();
	private:
		std::map<int, std::vector<bool>> StoryUpgradesTable;
};

