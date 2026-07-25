#pragma once
#include "pch.h"
#include <imgui.h>
#include "utils.h"
#include <vector>
#include <bitset>

struct checkpointTime {
	char minutes;
	char seconds;
	char centiseconds;
};

class MonitorWindow {
public:
	MonitorWindow() { };
	void OnFrame();
	void RenderTab();
	void drawMonitorWindow();
	void drawTime();
	void drawCheckpointTime();
	void drawPos();
	void drawPathDist();
	void drawVelocity();
	void drawStateID();
	void drawStatusBitfield();
	void drawInputs();
	void drawFrameCounter();
	void drawRNGValue();
private:
	void clearCheckTimes();

	bool OSD_show = false;
	bool OSD_displayTime = false;
	bool OSD_displayCheckpointTime = false;
	bool OSD_displayPos = false;
	bool OSD_displayVel = false;
	bool OSD_displayState = false;
	bool OSD_displayInputs = false;
	bool OSD_displayRNG = false;
	bool OSD_displayFrameCount = false;
	bool OSD_displayStatusBitfield = false;
	std::vector<checkpointTime> checkpointTimes;
};