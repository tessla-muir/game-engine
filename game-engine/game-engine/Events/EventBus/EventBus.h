#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "Event.h"
#include "../../Debugger/Debugger.h"
#include <map>
#include <typeindex>
#include <list>
#include <memory> 

class BaseEventCallback {
	private:
		virtual void Call(Event& e) = 0;

	public:
		virtual ~BaseEventCallback() = default;

		void Execute(Event& e) {
			Call(e);
		}
};

template <typename TOwner, typename TEvent>
class EventCallback : public BaseEventCallback {
	private:
		typedef void (TOwner::* CallbackFunction)(TEvent&);

		TOwner* owner;
		CallbackFunction callback;

		virtual void Call(Event& e) override {
			std::invoke(callback, owner, static_cast<TEvent&>(e));
		}

	public:
		EventCallback(TOwner* owner, CallbackFunction callback) {
			this->owner = owner;
			this->callback = callback;
		}

		virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<BaseEventCallback>> CallbackList;

class EventBus {
	private:
		std::map<std::type_index, std::unique_ptr<CallbackList>> listeners;

	public:
		EventBus() {}
		~EventBus() {}

		// Clear all listeners
		void Reset() {
			listeners.clear();
		}

		// Subscribes to generic event <T>
		template <typename TEvent, typename TOwner>
		void ListenToEvent(TOwner* owner, void (TOwner::* callback)(TEvent&)) {
			if (!listeners[typeid(TEvent)].get()) {
				listeners[typeid(TEvent)] = std::make_unique<CallbackList>();
			}
			auto listener = std::make_unique<EventCallback<TOwner, TEvent>>(owner, callback);
			listeners[typeid(TEvent)]->push_back(std::move(listener));
		}

		// Removes listeners for a specific event type
		template <typename TEvent>
		void RemoveListeners() {
			listeners.erase(typeid(TEvent));
		}

		// Dispatch the generic event <T>
		// Immediately goes to execute callback functions
		template <typename TEvent, typename ...TArgs>
		void DispatchEvent(TArgs&& ...args) {
			auto callbacks = listeners[typeid(TEvent)].get();
			if (callbacks) {
				// Loop through all the callbacks
				for (auto it = callbacks->begin(); it != callbacks->end(); it++) {
					// Executes the event for the callback
					auto callback = it->get();
					TEvent event(std::forward<TArgs>(args)...);
					callback->Execute(event);

					if (Debugger::debugLevel == 3 || Debugger::debugLevel == 9) Debugger::Log("Event: Execute event " + std::string(typeid(TEvent).name()));
				}
			}
		}
};

#endif