#include "Events.h"

#include "SKSE/API.h"


namespace Events
{
	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
	{
		static MenuOpenCloseEventHandler singleton;
		return &singleton;
	}


	auto MenuOpenCloseEventHandler::ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		auto uiStrHolder = RE::UIStringHolder::GetSingleton();
		if (a_event->menuName == uiStrHolder->dialogueMenu) {
			auto isOpening = a_event->isOpening;
			auto task = SKSE::GetTaskInterface();
			task->AddTask([isOpening]()
			{
			auto player = RE::PlayerCharacter::GetSingleton();
			if (isOpening) {
				player->SetCollision(false);
			} else {
				player->SetCollision(true);
			}
			});
		}

		return EventResult::kContinue;
	}


	void SinkEventHandlers()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->AddEventSink(MenuOpenCloseEventHandler::GetSingleton());
		_MESSAGE("Added menu open/close event sink");
	}
}
