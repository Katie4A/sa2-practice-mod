#pragma once
#include "pch.h"
#include <imgui.h>
#include <map>
#include <vector>

class FastDeath {
public:
	FastDeath();
	void OnInput();
	//void KillPlayerOld(int levelID);
	void KillPlayer();
	void RenderTab();
private:
	std::map<int, NJS_VECTOR> death_positions;
	bool fastDeathToggle = false;
	int prevButtons = 0;
};