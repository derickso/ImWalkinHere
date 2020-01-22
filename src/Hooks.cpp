#include "Hooks.h"

#include "CollisionHandler.h"


namespace Hooks
{
	void Install()
	{
		CollisionHandler::Install();
		_MESSAGE("Installed all hooks");
	}
}
