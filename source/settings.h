#pragma once
#include "pch.h"
#include "utils.h"
#include <imgui.h>
#include <array>

// ESG
DataPointer(int, esgFlag, 0x174B08C);
// pp Skip (thank you tenzit)
DataPointer(int, ppSkipTimer, 0x1AEDEC8);

struct CkgLevelEntry {
	int level;
	const char* name;
};

static constexpr std::array<CkgLevelEntry, 5> ckgLevelNameMap = {{
	{ 0, "Default" },
	{ LevelIDs_SecurityHall, "Security Hall" },
	{ LevelIDs_MeteorHerd, "Meteor Herd" },
	{ LevelIDs_FinalChase, "Final Chase" },
	{ LevelIDs_PumpkinHill, "Pumpkin Hill" } // chao%
}};

class Settings {
public:
	Settings() {};
	void setESG();
	void RenderTab();
	bool ppSkipToggleStatus();
	bool NeedsFrameTick() const;
	void OnFrame();
};
