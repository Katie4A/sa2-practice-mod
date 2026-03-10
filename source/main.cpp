#include "pch.h"
#include "hooks.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "magic.h"
#include "upgraderemover.h"
#include "settings.h"

extern "C"
{
	static UpgradeRemover* UpgradeR = new UpgradeRemover();
	static Settings* settings = new Settings();


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// setup imgui - huge thanks to labrys for helping me with this
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(MainWindowHandle);
		ImGui_ImplDX9_Init(g_pRenderDevice->m_pD3DDevice);
		ImGui::StyleColorsDark();
		initHooks(); // for the window message handling for imgui

		settings->init();

	}
	__declspec(dllexport) void __cdecl OnRenderSceneStart() {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Begin("Practice Mod");
		
		UpgradeR->RenderTab();
		settings->RenderTab();

		// tests
		ImGui::Text("Holy shit its level id %d", CurrentLevel);

		ImGui::End();
	}

	__declspec(dllexport) void __cdecl OnRenderSceneEnd() {

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (GameState == GameStates_Loading) {
			UpgradeR->SetStoryUpgrades(CurrentLevel, MainCharObj2[0]);
		}
		
		if (GameState == GameStates_LoadFinished or GameState == GameStates_Ingame or GameState == GameStates_Pause) {
			// update upgrades in real time
			UpgradeR->UpdateRealTime(MainCharObj2[0]);
		}
	}

	__declspec(dllexport) void __cdecl OnControl()
	{
		if (GameState == GameStates_Pause) {
			// todo: figure out restoring upgrades on restart
		}
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}
