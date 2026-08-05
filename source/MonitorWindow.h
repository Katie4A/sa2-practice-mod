#pragma once
#include "pch.h"
#include <imgui.h>
#include "utils.h"
#include <vector>
#include <bitset>

// RNG state pointer
// no idea if this changes when pracmod is on
// magic number offset is 0x5D4 bytes
DataPointer(uint32_t*, RNGState_ptr, 0x01751FF4);

struct checkpointTime {
	char minutes;
	char seconds;
	char centiseconds;
};

class MonitorWindow {
public:
	MonitorWindow();
	void ResetRNGIterCount();
	void RenderTab();
	void drawMonitorWindow();
	void drawCheckpointTime();
	void drawPos();
	void drawPathDist();
	void drawVelocity();
	void drawStateID();
	void drawStatusBitfield();
	void drawInputs();
	void drawFrameCounter();
	void drawRNGValue();
	void setEmeraldFrameCount(uint32_t f);
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
	bool OSD_displayPathDist = false;
	std::vector<checkpointTime> checkpointTimes;
	uint32_t setFrameCount = 0;
};