#include "Events.h"

#include "SKSE/API.h"


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


	auto MenuOpenCloseHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		auto intfcStr = RE::InterfaceStrings::GetSingleton();
		if (a_event->menuName == intfcStr->dialogueMenu) {
			_inDialogue = a_event->opening;
		}

		return EventResult::kContinue;
	}


	bool MenuOpenCloseHandler::IsInDialogue() const
	{
		return _inDialogue;
	}


	void Install()
	{
		MenuOpenCloseHandler::Install();
		_MESSAGE("Installed all event handlers");
	}
}
