#include "FastDeath.h"

// The original Fast Death tool was made by jellyfishswimmer.
// all the death positions were stolen from jelly's tool.
// Thanks for your incredible awesome epic work turtlechuck...

FastDeath::FastDeath() {
	// initialize death position table
	this->death_positions = {
	{LevelIDs_GreenForest, {0.0f, -990.0f, 0.0f}},    // green forest
	{LevelIDs_WhiteJungle, {0.0f, -2305.0f, 0.0f}},   // white jungle
	{LevelIDs_PumpkinHill, {0.0f, -1550.0f, 0.0f}},   // pumpkin hill
	{LevelIDs_SkyRail, {0.0f, -1276.0f, 1320.0f}}, // sky rail
	{LevelIDs_AquaticMine, {-439.0f, -80.0f, -30.0f}}, // aquatic mine - special case, no death planes exist so must put under weight
	{LevelIDs_SecurityHall, {0.0f, -1000.0f, 0.0f}},   // security hall
	{LevelIDs_PrisonLane, {-700.0f, -113.0f, 0.0f}}, // prison lane
	{LevelIDs_MetalHarbor, {0.0f, -332.0f, 0.0f}},   // metal harbor
	{LevelIDs_IronGate, {3080.0f, -1013.0f, 0.0f}}, // iron gate
	{LevelIDs_WeaponsBed, {0.0f, -332.0f, 0.0f}},   // weapons bed
	{LevelIDs_CityEscape, {344.0f, -6640.0f, 9828.0f}}, // city escape
	{LevelIDs_RadicalHighway, {0.0f, -700.0f, 0.0f}},   // radical highway
	{LevelIDs_WeaponsBed2P, {0.0f, -332.0f, 0.0f}},   // weapons bed 2P
	{LevelIDs_WildCanyon, {0.0f, -50.0f, 0.0f}},    // wild canyon
	{LevelIDs_MissionStreet, {0.0f, -50.0f, -3094.0f}}, // mission street
	{LevelIDs_DryLagoon, {0.0f, -200.0f, 0.0f}},   // dry lagoon
	{LevelIDs_SonicVsShadow1, {0.0f, -60.0f, 0.0f}},    // sonic vs shadow 1
	{LevelIDs_TailsVsEggman1, {0.0f, -332.0f, 0.0f}},   // tails vs eggman 1
	{LevelIDs_SandOcean, {0.0f, 0.0f, 0.0f}},      // sand ocean
	{LevelIDs_CrazyGadget, {-1053.0f, -245.0f, -396.0f}}, // crazy gadget
	{LevelIDs_HiddenBase, {0.0f, 0.0f, 0.0f}},      // hidden base
	{LevelIDs_EternalEngine, {0.0f, -1986.0f, -9129.0f}}, // eternal engine
	{LevelIDs_DeathChamber, {0.0f, -400.0f, 0.0f}},   // death chamber
	{LevelIDs_EggQuarters, {0.0f, -100.0f, 0.0f}},   // egg quarters
	{LevelIDs_LostColony, {0.0f, -492.0f, 0.0f}},   // lost colony
	{LevelIDs_PyramidCave, {0.0f, -820.0f, -1239.0f}}, // pyramid cave
	{LevelIDs_FinalRush, {0.0f, -150.0f, 0.0f}},   // final rush !!note 30!!
	{LevelIDs_GreenHill, {0.0f, -100.0f, 0.0f}},   // green hill
	{LevelIDs_MeteorHerd, {0.0f, -1670.0f, 0.0f}},  // meteor herd
	{LevelIDs_CannonsCoreS, {0.0f, -600.0f, 0.0f}},   // cannon's core (sonic)
	{LevelIDs_CannonsCoreE, {0.0f, -3040.0f, 0.0f}},  // cc eggman
	{LevelIDs_CannonsCoreT, {0.0f, -2681.0f, 0.0f}},  // cc tails
	{LevelIDs_CannonsCoreR, {0.0f, -200.0f, 0.0f}},   // cc rouge
	{LevelIDs_CannonsCoreK, {0.0f, -200.0f, 0.0f}},   // cc knuckles
	{LevelIDs_MissionStreet2P, {0.0f, -200.0f, 0.0f}},   // mission street 2p
	{LevelIDs_FinalChase, {0.0f, -4979.0f, 3499.0f}}, // final chase
	{LevelIDs_WildCanyon2P, {0.0f, -50.0f, 0.0f}},    // wild canyon 2p
	{LevelIDs_CosmicWall, {0.0f, -96.0f, 0.0f}},    // cosmic wall
	{LevelIDs_MadSpace, {0.0f, 160.0f, 0.0f}},    // mad space
	{LevelIDs_SandOcean2P, {0.0f, 0.0f, 0.0f}},      // sand ocean 2p
	{LevelIDs_DryLagoon2P, {0.0f, -200.0f, 0.0f}},   // dry lagoon 2p
	{LevelIDs_HiddenBase2P, {0.0f, -15.0f, 0.0f}},    // hidden base 2p
	{LevelIDs_PlanetQuest, {0.0f, -160.0f, 0.0f}},   // planet quest
	{LevelIDs_DeckRace, {0.0f, -332.0f, 0.0f}},   // deck race
	{LevelIDs_CosmicWall2P, {0.0f, -500.0f, 0.0f}},   // cosmic wall 2p
	{LevelIDs_GrindRace, {0.0f, -3620.0f, 0.0f}},  // grind race
	{LevelIDs_MetalHarbor2P, {0.0f, -332.0f, 0.0f}},   // metal harbor 2p
	{LevelIDs_IronGate2P, {3195.0f, -1013.0f, 0.0f}}, // iron gate 2p
	{LevelIDs_DeathChamber2P, {0.0f, -400.0f, 0.0f}}    // death chamber 2p
	};
}

// given level key, set player position to the relevant vector
void FastDeath::KillPlayer(int levelID) {
	if (this->death_positions.count(levelID) > 0) {
		MainCharObj1[0]->Position = death_positions[levelID];
	}
}

// ask tenzit to see if i can call the function that causes the death fadeout.
void FastDeath::OnInput() {
	if (GameState == GameStates_Ingame && fastDeathToggle) {
		// keep track of previous button presses by negating prevButtons and ANDing that with the current buttons pressed
		int buttons = ControllerPointers[0]->on;
		int buttonsPressed = ~this->prevButtons & buttons;
		this->prevButtons = buttons; // if bit = 1, prevButtons will be negated to prevent double presses.

		if (buttonsPressed & Buttons_Right) {
			this->KillPlayer(CurrentLevel);
		}
	}
}

// TODO: disable if savestates are enabled
void FastDeath::RenderTab() {
	if (ImGui::CollapsingHeader("Fast Death")) {
		ImGui::Checkbox("Enable Fast Death", &fastDeathToggle);
		ImGui::SetItemTooltip("If checked, pressing right on the D-Pad will place you inside a killplane, killing you instantly. Disabled if using savestates.");
	}
}