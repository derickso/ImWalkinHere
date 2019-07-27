#pragma once

#include "RE/Skyrim.h"


namespace Events
{
	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		using EventResult = RE::EventResult;


		static MenuOpenCloseEventHandler* GetSingleton();

		virtual	EventResult ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	private:
		MenuOpenCloseEventHandler() = default;
		MenuOpenCloseEventHandler(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler(MenuOpenCloseEventHandler&&) = delete;
		~MenuOpenCloseEventHandler() = default;

		MenuOpenCloseEventHandler& operator=(const MenuOpenCloseEventHandler&) = delete;
		MenuOpenCloseEventHandler& operator=(MenuOpenCloseEventHandler&&) = delete;
	};


	void SinkEventHandlers();
}
