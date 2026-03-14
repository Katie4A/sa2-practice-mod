#pragma once
#include "pch.h"
#include "utils.h"
#include <imgui.h>

// ESG
DataPointer(int, esgFlag, 0x174B08C);
// pp Skip (thank you tenzit)
DataPointer(int, ppSkipTimer, 0x1AEDEC8);

class Settings {
public:
	Settings() {};
	void setESG();
	void RenderTab();
	bool ppSkipToggleStatus();
	void OnFrame();
};