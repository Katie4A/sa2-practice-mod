#pragma once
#include "pch.h"
#include "utils.h"
#include <imgui.h>
#include <vector>

// ESG
DataPointer(int, esgFlag, 0x174B08C);
// pp Skip (thank you tenzit)
DataPointer(int, ppSkipTimer, 0x1AEDEC8);


static std::vector<std::pair<int, const char*>> ckgLevelNameMap =  {
	{0, "Default"},
	{LevelIDs_SecurityHall, "Security Hall"},
	{LevelIDs_MeteorHerd, "Meteor Herd"},
	{LevelIDs_FinalChase, "Final Chase"},
	{LevelIDs_PumpkinHill, "Pumpkin Hill"} //chao%
};

class Settings {
public:
	Settings() {};
	void setESG();
	void RenderTab();
	bool ppSkipToggleStatus();
	void OnFrame();
};