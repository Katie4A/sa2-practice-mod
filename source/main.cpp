#include "pch.h"
#include "hooks.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "magic.h"
#include "upgraderemover.h"
#include "settings.h"
#include "FastDeath.h"

extern "C"
{
	static UpgradeRemover* upgradeR = new UpgradeRemover();
	static Settings* settings = new Settings();
	static FastDeath* f_death = new FastDeath();
	static bool displayMenus = true;
	static bool prevF1Press = false;


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// setup imgui - huge thanks to labrys for helping me with this
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(MainWindowHandle);
		ImGui_ImplDX9_Init(g_pRenderDevice->m_pD3DDevice);
		ImGui::StyleColorsDark();
		initHooks(upgradeR, settings);
	}
	__declspec(dllexport) void __cdecl OnRenderSceneStart() {
		if (displayMenus) {
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
			ImGui::Begin("Practice Mod");

			upgradeR->RenderTab();
			f_death->RenderTab();
			settings->RenderTab();
			

			// tests
			ImGui::Text("Holy shit its level id %d", CurrentLevel);
			ImGui::Text("Press F1 to toggle the windows on or off.");
			ImGui::End();
		}
	}

	__declspec(dllexport) void __cdecl OnRenderSceneEnd() {

		if (displayMenus) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{	
		if (GameState == GameStates_Loading && upgradeR->storyUpgradesToggleStatus()) {
			upgradeR->SetStoryUpgrades(CurrentLevel, MainCharObj2[0]);
		}

		if ((GameState == GameStates_LoadFinished or GameState == GameStates_Ingame or GameState == GameStates_Pause) && upgradeR->realtimeUpgradesToggleStatus()) {
			upgradeR->UpdateRealTime(MainCharObj2[0]);
		}

		
	}

	__declspec(dllexport) void __cdecl OnInput() {
		bool F1Press = GetKeyState(VK_F1) & 0x8000;
		if (F1Press && !prevF1Press) {
			displayMenus = !displayMenus;
			prevF1Press = true;
		}
		else if (!F1Press) {
			prevF1Press = false;
		}

		f_death->OnInput();
	}

	__declspec(dllexport) void __cdecl OnControl()
	{
		// unfortunately, no inputs are registered on restart until the fade-out is complete. 
		// lets assume that if the player presses y at any point during gamestate 13, they wanted
		// to reset upgrades back to story style.
		if (GameState == GameStates_NormalRestart) {
			if ((ControllerPointers[0]->on & Buttons_Y) && upgradeR->storyUpgradesToggleStatus()) {
				upgradeR->SetStoryUpgrades(CurrentLevel, MainCharObj2[0]);
			}
		}
	}
	
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}

