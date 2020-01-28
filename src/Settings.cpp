#include "Settings.h"


bool Settings::loadSettings(bool a_dumpParse)
{
	return Json2Settings::Settings::loadSettings(FILE_NAME, a_dumpParse);
}


decltype(Settings::disableDialogueCollision) Settings::disableDialogueCollision("disableDialogueCollision", true);
decltype(Settings::disableAllyCollision) Settings::disableAllyCollision("disableAllyCollision", true);
