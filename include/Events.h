#pragma once

#include <atomic>

#include "RE/Skyrim.h"


namespace Events
{
	class MenuOpenCloseHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;


		static MenuOpenCloseHandler* GetSingleton();
		static void Install();

		virtual	EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		bool IsInDialogue() const;

	private:
		MenuOpenCloseHandler() = default;
		MenuOpenCloseHandler(const MenuOpenCloseHandler&) = delete;
		MenuOpenCloseHandler(MenuOpenCloseHandler&&) = delete;
		virtual ~MenuOpenCloseHandler() = default;

		MenuOpenCloseHandler& operator=(const MenuOpenCloseHandler&) = delete;
		MenuOpenCloseHandler& operator=(MenuOpenCloseHandler&&) = delete;


		std::atomic_bool _inDialogue;
	};


	void Install();
}
