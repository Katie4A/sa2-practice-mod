#include "pch.h"
#include "hooks.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "magic.h"
#include "upgraderemover.h"



extern "C"
{
	static int numRestarts = 0;
	static bool check = 0;
	static UpgradeRemover* UpgradeR = new UpgradeRemover();

	// This function runs code one time when the game starts up. Great for loading assets and setting things up.
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// setup imgui - huge thanks to labrys for helping me with this
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(MainWindowHandle);
		ImGui_ImplDX9_Init(g_pRenderDevice->m_pD3DDevice);
		ImGui::StyleColorsDark();
		//for the window message handling for imgui
		initHooks();

	}
	__declspec(dllexport) void __cdecl OnRenderSceneStart() {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Begin("Practice Mod");
		if (GameState == GameStates_Ingame) {
			ImGui::SliderInt("Number of Restarts/Deaths", &numRestarts, 0, 999);
		}
		else {
			ImGui::Text("Not In Game (womp womp)");
		}
		//render tabs
		UpgradeR->RenderTab();
		ImGui::Text("Check flag! %d", check);

		ImGui::End();
	}

	__declspec(dllexport) void __cdecl OnRenderSceneEnd() {

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	// Optional.
	// This function runs code on every frame of the game, INCLUDING being in menus, cutscenes, etc.
	// It is recommended to test for game state so that you only run code at a specific time.
	__declspec(dllexport) void __cdecl OnFrame()
	{
		TimesRestartedOrDied = (short) numRestarts;
		if (GameState == GameStates_LoadFinished or GameState == GameStates_Ingame or GameState == GameStates_Pause) {
			// we can surely access the main character object here, right?
			UpgradeR->UpdateRealTime(MainCharObj2[0]);
		}
	}

	// Optional.
	// This function runs code every time the player inputs. Good for adding custom inputs / overriding events.
	__declspec(dllexport) void __cdecl OnInput()
	{
	}

	// Optional.
	// This function runs while the game processes input.
	__declspec(dllexport) void __cdecl OnControl()
	{
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}
