#pragma once

#include "../System.h"

#include <Data/ManagedItem.h>

#include <magic_enum_utility.hpp>

#include <functional>
#include <mutex>
#include <vector>

#include <entt/core/type_info.hpp>

/*
* nes - Nuvola Event System
*/
namespace nes
{
	//Internal utilities
	namespace internal
	{
		//Utils to extract type information
		template<typename class_t = std::false_type>
		struct extract_type
		{
			typedef class_t type;
		};
		template<typename return_t, typename class_t, typename... args_t>
		struct extract_type<return_t(class_t::*)(args_t...)>
		{
			typedef class_t type;
			typedef return_t ret;
		};
	}

	enum class Priority
	{
		First,
		Normal,
		Last
	};

	//Owns an event pointer, used to pass around an event
	template<typename event_t>
	class event_holder
	{
		std::unique_ptr<event_t> e;
	public:
		template<typename... args_t>
		event_holder(args_t... args) : e(std::make_unique<event_t>(args...)) {};

		event_t* get() const
		{
			return e.get();
		}
		event_t& ref() const
		{
			return *e.get();
		}
		event_t* operator->() const
		{
			return get();
		}
	};

	//Creates an event holder instance
	template<typename event_t, typename... args_t>
	event_holder<event_t> make_holder(args_t... args)
	{
		return event_holder<event_t>(args...);
	}

	//A type for holding the wrapper function that invokes the callback
	template<typename event_t>
	using event_wrapper = std::function<void(event_t&)>;

	template<typename event_t, typename wrapper_t = event_wrapper<event_t>>
	struct event_listener
	{
		using holder_t = event_holder<event_t>;
		template<typename class_t>
		event_listener(class_t* instance, wrapper_t&& wrapper, entt::id_type methodHash) : instance(instance), method(std::move(wrapper)), methodHash(methodHash) {};

		void invoke(const holder_t& holder) const
		{
			method(holder.ref());
		}

		void* instance;
		wrapper_t method;
		entt::id_type methodHash;
	};

	template<typename event_t>
	struct dispatcher
	{
		using type_t = event_t;
		using id_t = entt::type_hash<event_t>;
		using holder_t = event_holder<event_t>;
		using listener_t = event_listener<event_t>;

		const std::vector<listener_t>& getListeners() const
		{
			return this->listeners;
		}
		template<auto handler, auto priority = Priority::Normal, typename class_t = internal::extract_type<decltype(handler)>::class_t, typename wrapper_t = event_wrapper<event_t>>
		void listen(class_t* instance)
		{
			wrapper_t wrapper = [instance](event_t& e) {
				(instance->*handler)(e);
			};
			std::lock_guard<std::mutex> guard(lock);
			listeners[priority].emplace_back(instance, std::move(wrapper), entt::type_hash<decltype(handler)>::value());
		}
		template<typename handler_t, typename class_t = internal::extract_type<handler_t>::class_t>
		void deafen(class_t* instance, handler_t&& handler)
		{
			std::lock_guard<std::mutex> guard(lock);
			for(auto& [priority, theListeners] : listeners)
			{
				std::erase_if(theListeners, [&](auto &listener) -> bool { return listener.methodHash == entt::type_hash<handler_t>::value(); });
			}
		}
		void trigger(const holder_t& holder)
		{
			std::lock_guard<std::mutex> guard(lock);

			magic_enum::enum_for_each<Priority>([&](auto priority) -> void {
				for (const auto& listener : listeners[priority])
				{
					listener.invoke(holder);
				}
			});
		}
	private:
		//The event listeners for this dispatcher
		std::unordered_map<Priority, std::vector<listener_t>> listeners;
		std::mutex lock;
	};
}

//The main event dispatcher, use this to listen for and dispatch events
class EventDispatcher : public System<ManagedItem>
{
public:
	EventDispatcher();

	template<typename event_t>
	[[nodiscard]] auto& get() const
	{
		static nes::dispatcher<event_t> instance;
		return instance;
	}

	template<typename event_t>
	void trigger(const nes::event_holder<event_t>& e) const
	{
		auto& disp = get<event_t>();
		disp.trigger(e);
	}

	template<typename event_t, auto handler, auto priority = nes::Priority::Normal, typename class_t = nes::internal::extract_type<decltype(handler)>::class_t>
	[[nodiscard]] void listen(class_t* instance) const
	{
		auto& disp = get<event_t>();
		disp.listen<handler, priority>(instance);
	}

	template<typename event_t, auto handler, typename class_t = nes::internal::extract_type<decltype(handler)>::class_t>
	void deafen(class_t* instance) const
	{
		auto& disp = get<event_t>();
		disp.deafen(instance, static_cast<decltype(handler)>(handler));
	}
};