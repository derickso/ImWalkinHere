#include "Events.h"

#include "SKSE/API.h"

#include "CollisionHandler.h"


namespace Events
{
	MenuOpenCloseHandler* MenuOpenCloseHandler::GetSingleton()
	{
		static MenuOpenCloseHandler singleton;
		return &singleton;
	}


	void MenuOpenCloseHandler::Install()
	{
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(MenuOpenCloseHandler::GetSingleton());
		_MESSAGE("Added menu open/close event sink");
	}


	auto MenuOpenCloseHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		auto intfcStr = RE::InterfaceStrings::GetSingleton();
		if (a_event->menuName == intfcStr->dialogueMenu) {
			CollisionHandler::SetInDialogue(a_event->opening);
		}

		return EventResult::kContinue;
	}


	void Install()
	{
		MenuOpenCloseHandler::Install();
		_MESSAGE("Installed all event handlers");
	}
}
