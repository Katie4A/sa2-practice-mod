#pragma once
#include "pch.h"
#include <imgui.h>
#include <map>
#include <vector>

class FastDeath {
public:
	FastDeath();
	void OnInput();
	void KillPlayer(int levelID);
	void RenderTab();
private:
	std::map<int, NJS_VECTOR> death_positions;
	bool fastDeathToggle = false;
	int prevButtons = 0;
};