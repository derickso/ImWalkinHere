#pragma once

#include "Json2Settings.h"


class Settings : public Json2Settings::Settings
{
public:
	Settings() = delete;
	static bool loadSettings(bool a_dumpParse = false);


	static bSetting disableDialogueCollision;
	static bSetting disableAllyCollision;

private:
	static constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\ImWalkinHere.json";
};
