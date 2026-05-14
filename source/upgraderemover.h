#pragma once
#include "pch.h"
#include <imgui.h>
#include "utils.h"

// upgrade booleans stored on file (there are 29 bytes to update)
// thank you Emerua for making UpgradeRemover & finding this address
DataArray(bool, UpgradesOnFile, 0x1DEB300, 29);

class UpgradeRemover {
	public:
		UpgradeRemover() {};
		void InitUpgradeObjectHooks();
		void OnControl();
		void OnPlayerInit(CharObj2Base* player);
		bool ApplyCurrentUpgradeMask(CharObj2Base* player);
		bool ApplyStoryUpgrades(short currentLevel, CharObj2Base* player);
		void ApplyPendingRestartUpgradeReset(CharObj2Base* player);
		void RenderTab();
		bool SonicTab();
		bool TailsTab();
		bool KnucklesTab();
		bool ShadowTab();
		bool EggmanTab();
		bool RougeTab();
		bool storyUpgradesToggleStatus() { return storyUpgrades; };
		bool realtimeUpgradesToggleStatus() { return realTime; };
	private:
		bool QueueStoryRestartReset();
		bool storyUpgrades = 0;
		bool realTime = 0;
		bool storyRestartResetQueued = false;
};
