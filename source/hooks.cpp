#include "hooks.h"
#include "UsercallFunctionHandler.h"

// restart function address stolen from https://github.com/StarlitLuna/sa2-story-style-upgrades
FunctionHook<void> hRestartLevel((intptr_t)0x43C370);
// kart_delete function (thank you tenzit)
FunctionHook<void> hkart_delete((intptr_t)0x61A910);
UsercallFuncVoid(SetPhysicsAndGiveUpgrades, (ObjectMaster* character, int playerIndex), (character, playerIndex), 0x4599C0, rEAX, rECX);

static StdcallFunctionHook<LRESULT, HWND, UINT, WPARAM, LPARAM> wndProcHook(WndProc);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

UpgradeRemover* g_upgradeR = nullptr;
Settings* g_settings = nullptr;
static const bool* g_displayMenus = nullptr;

LRESULT __stdcall wndProc_h(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) {
	if (g_displayMenus != nullptr && *g_displayMenus && ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wParam, lParam)) {
		return true;
	}

	return wndProcHook.Original(hwnd, umsg, wParam, lParam);
}

void SetPhysicsAndGiveUpgrades_Hook(ObjectMaster* character, int playerIndex) {
	SetPhysicsAndGiveUpgrades.Original(character, playerIndex);

	if (g_upgradeR == nullptr) {
		return;
	}

	CharObj2Base* player = nullptr;
	if (playerIndex >= 0 && playerIndex < 8) {
		player = MainCharObj2[playerIndex];
	}

	if (player == nullptr) {
		player = MainCharObj2[0];
	}

	g_upgradeR->OnPlayerInit(player);
}

void initHooks(UpgradeRemover* u, Settings* s, const bool* displayMenus) {
	g_upgradeR = u;
	g_settings = s;
	g_displayMenus = displayMenus;

	wndProcHook.Hook(wndProc_h);
	hRestartLevel.Hook(RestartLevel);
	hkart_delete.Hook(kart_delete);
	SetPhysicsAndGiveUpgrades.Hook(SetPhysicsAndGiveUpgrades_Hook);
}

// function hooks listed here can used by multiple tabs
void RestartLevel() {
	if (g_settings != nullptr && g_settings->ppSkipToggleStatus()) {
		ppSkipTimer = 0;
	}

	hRestartLevel.Original();

	if (g_upgradeR != nullptr) {
		g_upgradeR->ApplyPendingRestartUpgradeReset(MainCharObj2[0]);
	}
}

// called when exiting the level
void kart_delete() {
	hkart_delete.Original();
	if (g_settings != nullptr && g_settings->ppSkipToggleStatus()) {
		ppSkipTimer = 0;
	}
}
