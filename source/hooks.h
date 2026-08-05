#pragma once
#include "pch.h"
#include "imgui_impl_win32.h"
#include "upgraderemover.h"
#include "settings.h"
#include "MonitorWindow.h"

class UpgradeRemover;
class Settings;
class MonitorWindow;

void initHooks(UpgradeRemover* u, Settings* s, MonitorWindow* m, const bool* displayMenus, const bool* displayMonitorWindow);

void RestartLevel();
void kart_delete();
