#include "MonitorWindow.h"


// This monitor window was largely inspired by Prime's Practice Mod
// https://github.com/MetroidPrimeModding/prime-practice-native/blob/main/src/UI/MonitorWindow.cpp

static bool isInGame = false;

void MonitorWindow::drawMonitorWindow() {
	if (!OSD_show) {
		return;
	}
	
	// i have no idea where this is gonna draw
	ImGui::SetNextWindowPos(ImVec2(ceil(HorizontalResolution - (HorizontalResolution / (4))), 10), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Monitor", nullptr,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration);
	
	if (OSD_displayTime) {
		drawTime();
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
	if (OSD_displayInputs) {
		drawInputs();
	}
	if (OSD_displayRNG) {
		drawRNGValue();
	}

	ImGui::End();
}

void MonitorWindow::drawTime() {
	ImGui::Text("Super fucking boner!!!!!!!!!!! LOL!!!!!!!!!!!!!!!!! BONER TIME\nBONERRRRRRRRRRRR TIME BONERRRR");
}

void MonitorWindow::drawCheckpointTime() {

}

void MonitorWindow::drawPos() {

}

void MonitorWindow::drawVelocity() {

}

void MonitorWindow::drawStateID() {

}

// TODO: build a fucking input viewer?????
void MonitorWindow::drawInputs() {

}

void MonitorWindow::drawRNGValue() {

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

		ImGui::Checkbox("Time", &OSD_displayTime);
		ImGui::Checkbox("Checkpoint Time", &OSD_displayCheckpointTime);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, displays what time the player hit the checkpoint at with the individual segment time shown beside it.");
		ImGui::Checkbox("Position", &OSD_displayPos);
		ImGui::Checkbox("Velocity", &OSD_displayVel);
		ImGui::Checkbox("State ID", &OSD_displayState);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, displays the character's current state ID.");
		ImGui::Checkbox("Inputs", &OSD_displayInputs);
		ImGui::Checkbox("RNG Value", &OSD_displayRNG);

		if (!OSD_show) {
			ImGui::EndDisabled();
		}
	}
}