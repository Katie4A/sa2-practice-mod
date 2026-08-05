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
#include "MonitorWindow.h"

extern "C"
{
	
	static UpgradeRemover upgradeR;
	static MonitorWindow monitorWindow;
	static Settings settings;
	static FastDeath f_death;
	static HuntingSettings huntingSettings;
	
	static bool displayMainMenus = true;
	static bool displayMonitorWindow = true;
	static bool prevF1Press = false;
	static bool prevF2Press = false;
	static bool imguiDx9DeviceObjectsValid = false;

	static bool HasValidImGuiDx9Texture() {
		for (ImTextureData* texture : ImGui::GetPlatformIO().Textures) {
			if (texture->RefCount == 1 && texture->TexID != ImTextureID_Invalid) {
				return true;
			}
		}

		return false;
	}

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		HelperFunctions HelperFunctionsGlobal = helperFunctions;

		std::string modpath(path);
		const IniFile config(modpath + "\\config.ini");
		bool useMultiViewports = config.getBool("GeneralSettings", "multiEnabled", false);

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
		initHooks(&upgradeR, &settings, &displayMenus);

		huntingSettings.init(&monitorWindow);
	}

	__declspec(dllexport) void __cdecl OnRenderSceneStart() {
		if (displayMainMenus) {
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Practice Mod");

			if (ImGui::Button("Get 99 Lives")) {
				Life_Count[0] = 99;
			}

			upgradeR.RenderTab();
			f_death.RenderTab();
			monitorWindow.RenderTab();
			settings.RenderTab();
			huntingSettings.RenderTab();

			ImGui::Text("Press F1 to toggle the windows on or off.\n(Does not work when windows are undocked)");
			ImGui::End();

			if (displayMonitorWindow) {
				monitorWindow.drawMonitorWindow();
			}
		}
	}

	__declspec(dllexport) void __cdecl OnRenderSceneEnd() {
		if (displayMainMenus) {
			ImGui::Render();
			ImDrawData* drawData = ImGui::GetDrawData();
			ImGui_ImplDX9_RenderDrawData(drawData);

			// we need to check on render scene end if our drawData is still valid
			if (drawData->DisplaySize.x > 0.0f && drawData->DisplaySize.y > 0.0f && HasValidImGuiDx9Texture()) {
				imguiDx9DeviceObjectsValid = true;
			}

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}
	}

	__declspec(dllexport) void __cdecl OnFrame() {
		if (settings.NeedsFrameTick()) {
			settings.OnFrame();
		}
		//monitorWindow.OnFrame();
	}

	__declspec(dllexport) void __cdecl OnInput() {
		bool F1Press = GetKeyState(VK_F1) & 0x8000;
		if (F1Press && !prevF1Press) {
			displayMainMenus = !displayMainMenus;
			prevF1Press = true;
		}
		else if (!F1Press) {
			prevF1Press = false;
		}

		bool F2Press = GetKeyState(VK_F2) & 0x8000;
		if (F2Press && !prevF2Press) {
			displayMonitorWindow = !displayMonitorWindow;
			prevF2Press = true;
		}
		else if (!F1Press) {
			prevF2Press = false;
		}


		f_death.OnInput();
	}

	__declspec(dllexport) void __cdecl OnControl() {
		upgradeR.OnControl();
	}

	__declspec(dllexport) void __cdecl OnRenderDeviceLost() {
		if (imguiDx9DeviceObjectsValid) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			imguiDx9DeviceObjectsValid = false;
		}
	}

	__declspec(dllexport) void __cdecl OnRenderDeviceReset() {
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}
