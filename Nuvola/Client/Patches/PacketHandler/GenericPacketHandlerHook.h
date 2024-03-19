#pragma once

#include "../IPatch.h"

#include "../../Bridge/MinecraftPackets.h"
#include "../../Bridge/Packet.h"

#include "../../Events/EventDispatcher.h"
#include "../../Events/ReceivePacketEvent.h"

#include <polyhook2/Virtuals/VFuncSwapHook.hpp>
#include <magic_enum.hpp>

#include <memory>

template<PacketID id>
class GenericPacketHandlerHook : public IPatch {
public:
	static inline bool HandlerExists()
	{
		auto pkt = MinecraftPackets::createPacket(id);
		if (pkt != nullptr)
		{
			//Get the handler dispatcher thing
			auto dispatcher = pkt->packetHandlerDispatcher;
			if (dispatcher == nullptr) return false;
			//Get the vtable for the dispatcher
			void** dispatcherVft = *reinterpret_cast<void***>(dispatcher);
			if (dispatcherVft == nullptr) return false;
			//Get the handler function from the vtable
			auto handlerFunc = dispatcherVft[1];
			return handlerFunc != nullptr;
		}
		return false;
	}

	GenericPacketHandlerHook() : IPatch("PacketHandlerHook::handle" + std::string(magic_enum::enum_name<PacketID>(id)))
	{
		//Make the packet with the ID
		auto pkt = MinecraftPackets::createPacket(id);
		if (pkt != nullptr)
		{
			// Get the handler dispatcher thing, even though the packet shared_ptr will go out of scope and be deleted,
            // dispatcher instances are statically initialized and are not tied to individual packets. so this is fine.
			this->dispatcher = pkt->packetHandlerDispatcher;
            this->redirectMap[1] = (uint64_t) &GenericPacketHandlerCallback_1_20_12_1;
		}
		else
		{
			throw std::exception(std::string("No handler for " + this->getName()).c_str());
		}
	}

	bool apply() override
	{
		//Create the hook
		if (this->redirectMap.empty()) {
			return false;
		}

        hook = std::make_unique<PLH::VFuncSwapHook>((uint64_t) this->dispatcher,
												   this->redirectMap,
												   &originalMap);

		if (!hook->hook()) {
			return false;
		}

		return true;
	}
private:
    void* dispatcher{};
    PLH::VFuncMap redirectMap{};
    static inline PLH::VFuncMap originalMap{};

    static void __fastcall GenericPacketHandlerCallback_1_20_12_1(class PacketHandlerDispatcherInstance* self, class NetworkIdentifier* param_1, class NetEventCallback* param_2, const std::shared_ptr<Packet>& param_3)
    {
        auto event = nes::make_holder<ReceivePacketEvent>(param_3.get());

        auto dispatcher = systems->getEventDispatcher();
        dispatcher->trigger(event);

        if (!event->isCancelled())
            PLH::FnCast(originalMap[1], &GenericPacketHandlerCallback_1_20_12_1)(self, param_1, param_2, param_3);
    }
};