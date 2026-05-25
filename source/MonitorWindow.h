#pragma once
#include "pch.h"
#include <imgui.h>
#include "utils.h"


class MonitorWindow {
public:
	MonitorWindow() { };
	void RenderTab();
	void drawMonitorWindow();
	void drawTime();
	void drawCheckpointTime();
	void drawPos();
	void drawVelocity();
	void drawStateID();
	void drawInputs();
	void drawRNGValue();
private:
	bool OSD_show = false;
	bool OSD_displayTime = false;
	bool OSD_displayCheckpointTime = false;
	bool OSD_displayPos = false;
	bool OSD_displayVel = false;
	bool OSD_displayState = false;
	bool OSD_displayInputs = false;
	bool OSD_displayRNG = false;
};