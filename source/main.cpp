#include "pch.h"
#include "hooks.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "magic.h"
#include "upgraderemover.h"
#include "settings.h"
#include "FastDeath.h"
#include "Inifile.hpp"
#include "hunting.h"

extern "C"
{
	static UpgradeRemover* upgradeR = new UpgradeRemover();
	static Settings* settings = new Settings();
	static FastDeath* f_death = new FastDeath();
	static HuntingSettings* huntingSettings = new HuntingSettings();
	static bool displayMenus = true;
	static bool prevF1Press = false;
	//for later
	static ImGuiWindowFlags osd_windowflags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{

		HelperFunctions HelperFunctionsGlobal = helperFunctions;
		
		std::string modpath(path);
		const IniFile* config = new IniFile(modpath + "\\config.ini");
		bool useMultiViewports = config->getBool("GeneralSettings", "multiEnabled", false);


		// setup imgui - huge thanks to labrys for helping me with this
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO(); (void)io;
		if (useMultiViewports) {
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		}

		ImGui_ImplWin32_Init(MainWindowHandle);
		ImGui_ImplDX9_Init(g_pRenderDevice->m_pD3DDevice);
		ImGui::StyleColorsDark();
		initHooks(upgradeR, settings);

		huntingSettings->init();

	}
	__declspec(dllexport) void __cdecl OnRenderSceneStart() {
		if (displayMenus) {
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Practice Mod");

			if (ImGui::Button("Get 99 Lives")) {
				Life_Count[0] = 99;
			}

			upgradeR->RenderTab();
			f_death->RenderTab();
			settings->RenderTab();
			huntingSettings->RenderTab();
			
			ImGui::Text("Press F1 to toggle the windows on or off.\n(Does not work when windows are undocked)");

			// tess
			// ImGui::ShowDemoWindow();
			//ImGui::Text("Holy shit its level id %d", CurrentLevel);
			
			ImGui::End();
		}
	}

	__declspec(dllexport) void __cdecl OnRenderSceneEnd() {
		if (displayMenus) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			//i don't care about performance per-se atm lol
			ImGuiIO& io = ImGui::GetIO();
			// but you gotta do this at the end
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
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

		//unsure if this is the *best* idea but here we are
		settings->OnFrame();
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

