#pragma once
#include "pch.h"
#include "imgui_impl_win32.h"
#include "upgraderemover.h"
#include "settings.h"

class UpgradeRemover;
class Settings;

void initHooks(UpgradeRemover* u, Settings* s, const bool* displayMenus);

void RestartLevel();
void kart_delete();
