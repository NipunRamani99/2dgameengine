#pragma once
#include "../ECS/ECS.h"
#include <functional>
#include <map>
class EventBus {
private:
	std::map<std::type_index, std::vector<std::function<void(void*)>>> eventHandlers;
public:

	template <typename ConsumerT, typename EventT>
	void subscribe(ConsumerT* consumer, void (ConsumerT::* eventHandler)(EventT*)) {
		if (eventHandlers.find(std::type_index(typeid(EventT))) == eventHandlers.end()) {
			eventHandlers.insert({ std::type_index(typeid(EventT)), std::vector<std::function<void(void*)>>{} });
		}
		eventHandlers[std::type_index(typeid(EventT))].push_back(std::move([consumer, eventHandler](void* event) { (consumer->*eventHandler)(static_cast<EventT*>(event)); }));
	}

	template<typename EventT>
	void publishEvent(EventT* event) {
		auto _eventHandlers = eventHandlers[std::type_index(typeid(EventT))];
		for (const auto& handler : _eventHandlers) {
			handler(event);
		}
	}
};
