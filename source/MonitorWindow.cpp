#include "MonitorWindow.h"

// This monitor window was largely inspired by Prime's Practice Mod
// https://github.com/MetroidPrimeModding/prime-practice-native/blob/main/src/UI/MonitorWindow.cpp

// checkpoint function hooks
FunctionHook<void> Checkpoint_Save_Current_Player((intptr_t)0x43E520);

void CheckpointSave_Hook() {
	
}

void MonitorWindow::OnFrame() {
	if (GameState == GameStates_Ingame) {
		// has the player hit a checkpoint?

	}
}

void MonitorWindow::drawMonitorWindow() {
	if (!OSD_show) {
		return;
	}
	
	// i have no idea where this is gonna draw
	ImGui::SetNextWindowPos(ImVec2(ceil(HorizontalResolution - (HorizontalResolution / (4))), 10), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Monitor", nullptr,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration);
	if (OSD_displayFrameCount) {
		drawFrameCounter();
	}
	if (OSD_displayCheckpointTime) {
		drawCheckpointTime();
	}
	if (OSD_displayPos) {
		drawPos();
	}
	if (OSD_displayVel) {
		drawVelocity();
	}
	if (OSD_displayState) {
		drawStateID();
	}
	if (OSD_displayStatusBitfield) {
		drawStatusBitfield();
	}
	if (OSD_displayInputs) {
		drawInputs();
	}
	if (OSD_displayRNG) {
		drawRNGValue();
	}

	ImGui::End();
}


/*
	Displays the player's checkpoint times through the stage
	TODO: potentially add a checkpoint time comparison option?
		- would require the ability to save settings and/or data
*/
void MonitorWindow::drawCheckpointTime() {
	ImGui::Text("Checkpoint Times:");
	if (!checkpointTimes.empty()) {
		for (int i = 0; i < checkpointTimes.size(); ++i) {
			checkpointTime check = checkpointTimes[i];
			ImGui::Text("Time: %02d:%02d:%02d", check.minutes, check.seconds, check.centiseconds);
		}
	}
}

void MonitorWindow::drawPos() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause || GameState == GameStates_LoadFinished) {
		NJS_VECTOR position = MainCharObj1[0]->Position;
		ImGui::Text("Pos: %5.3fx %5.3fy %5.3fz", position.x, position.y, position.z);
	}
}

// the fuck is this?
void MonitorWindow::drawPathDist() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause || GameState == GameStates_LoadFinished) {
		float path_distance = MainCharObj2[0]->PathDist;
		ImGui::Text("Path Distance: %0.2f", path_distance);
	}
}

// horizontal, vertical speed
// add stored speed if speed character is active
void MonitorWindow::drawVelocity() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause || GameState == GameStates_LoadFinished) {
		NJS_VECTOR speed = MainCharObj2[0]->Speed;
		ImGui::Text("Speed: %5.4fx %5.4fy %5.4fz", speed.x, speed.y, speed.z);
		
		if (MainCharObj2[0]->CharID == Characters_Sonic || MainCharObj2[0]->CharID == Characters_Shadow || MainCharObj2[0]->CharID == Characters_Amy || MainCharObj2[0]->CharID == Characters_MetalSonic) {
			float storedSpeed = MainCharObj2[0]->storedSpeed;
			ImGui::Text("Stored Speed: %5.4f", storedSpeed);
		}
	}
}

// Action & NextAction are a part of EntityData1
void MonitorWindow::drawStateID() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause || GameState == GameStates_LoadFinished) {
		char action = MainCharObj1[0]->Action;
		char sAction = MainCharObj1[0]->NextAction;

		ImGui::Text("Action ID: %d Next Action ID: %d", action, sAction);
	}
}

// Status is a part of EntityData1 as well
// TODO: verbalize status flags??
void MonitorWindow::drawStatusBitfield() {
	if (GameState == GameStates_Ingame || GameState == GameStates_Pause || GameState == GameStates_LoadFinished) {
		short status = static_cast<short>(MainCharObj1[0]->Status);
		std::bitset<16> b1(status);
		std::string output = b1.to_string();
		
		ImGui::Text("Status Bitfield: %s", output.c_str());
	}
}

// TODO: build a fucking input viewer?????
void MonitorWindow::drawInputs() {

}

void MonitorWindow::drawFrameCounter() {
	ImGui::Text("Frame Counter: %0d", FrameCount);
}

void MonitorWindow::drawRNGValue() {

}

void MonitorWindow::clearCheckTimes()
{
	checkpointTimes.clear();
}

void MonitorWindow::RenderTab() {
	if (ImGui::CollapsingHeader("On-screen Display")) {
		// TODO: add font scale option for big resolutions?
		ImGui::Checkbox("Enable", &OSD_show);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, turns on the on-screen display. The on-screen display can show various details as shown below.");

		if (!OSD_show) {
			ImGui::BeginDisabled();
		}

		ImGui::Checkbox("Frame Counter", &OSD_displayFrameCount);
		ImGui::Checkbox("Checkpoint Time", &OSD_displayCheckpointTime);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, displays what time the player hit the checkpoint at with the individual segment time shown beside it.");
		ImGui::Checkbox("Position", &OSD_displayPos);
		ImGui::Checkbox("Velocity", &OSD_displayVel);
		ImGui::Checkbox("State ID", &OSD_displayState);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, displays the character's current state ID.");
		ImGui::Checkbox("Status Bitfield", &OSD_displayStatusBitfield);
		ImGui::Checkbox("Inputs", &OSD_displayInputs);

		ImGui::Checkbox("RNG Value", &OSD_displayRNG);

		if (!OSD_show) {
			ImGui::EndDisabled();
		}
	}
}