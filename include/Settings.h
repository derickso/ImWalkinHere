#pragma once

#include "Json2Settings.h"


class Settings
{
public:
	using bSetting = Json2Settings::bSetting;


	Settings() = delete;

	static bool LoadSettings(bool a_dumpParse = false);


	static bSetting disableDialogueCollision;
	static bSetting disableAllyCollision;

private:
	static constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\ImWalkinHere.json";
};
