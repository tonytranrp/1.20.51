#include "Disabler.h"
#include "../../../../Utils/LogicUtils.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"

Disabler::Disabler() : Module("Disabler", "Disables some or all anti-cheat checks.") {

    this->mode = this->addItem<ModeSetting<disablerMode_t>>("Mode", "Which server should the disabler target?");

    auto sentinelSettings = this->mode->getManagerFor(disablerMode_t::Sentinel);
    this->sentinelMode = sentinelSettings->addItem<ModeSetting<sentinelMode_t>>("Type", "Which type of sentinel disabler to use?");

};

void Disabler::onPacketEvent(PacketEvent& event)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return;

    disablerMode_t disableMode = this->mode->getCurrentMode();

    switch (disableMode)
    {
    case disablerMode_t::Sentinel:
    {
        sentinelMode_t sentinelMode = this->sentinelMode->getCurrentMode();
        switch (sentinelMode)
        {
        case sentinelMode_t::Spam:
        {
            switch (event.getPacket()->getId())
            {
            case PacketID::NetworkStackLatencyPacket:
                event.setCancelled(true);
                break;
            case PacketID::MovePlayerPacket:
            case PacketID::PlayerAuthInputPacket:
                for (int i = 0; i < 4; i++)
                {
                    ci->mPacketSender->sendToServer(event.getPacket());
                }
                break;
            default:
                break;
            }
            break;
        }
        case sentinelMode_t::Old:
        {
            if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket) {
                auto paip = (PlayerAuthInputPacket*)event.getPacket();
                paip->position = Vector3<float>(0, 0, 0); // silly anticheat, you can't stop me from flying
                MovePlayerPacket newMpp = MovePlayerPacket(player, paip->position);
                newMpp.onGround = player->isOnGround();
                newMpp.mode = 2;
                newMpp.rotation = paip->rotation;
                newMpp.headYaw = paip->headYaw;
                // send the packet
                ci->mPacketSender->sendToServer(&newMpp);
            }
            else {
                return;
            }
            break;
        }
        }

        break;
    }
    case disablerMode_t::Lifeboat:
        if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
        {
            auto* paip = reinterpret_cast<PlayerAuthInputPacket*>(event.getPacket());
            int32_t tick = paip->ticksAlive % (MC_JUMP_TICKS * 2);
            float perc = static_cast<float>(tick) / (MC_JUMP_TICKS * 2.0f);
            float lerpedY = std::lerp(paip->position.y, paip->position.y - 1.0f, perc);
            paip->position.y = lerpedY;
            paip->downVelocity = -(1.0f / (MC_JUMP_TICKS * 2.0f));
            if (tick == 0 || tick == MC_JUMP_TICKS)
            {
                paip->inputData |= AuthInputAction::START_JUMPING;
            }
            paip->inputData |= AuthInputAction::JUMPING;
        }
        if (event.getPacket()->getId() == PacketID::MovePlayerPacket)
        {
            auto* mpp = reinterpret_cast<MovePlayerPacket*>(event.getPacket());
            int32_t tick = mpp->tick % 24;
            float perc = static_cast<float>(tick) / 24.0f;
            float lerpedY = std::lerp(mpp->position.y, mpp->position.y - 1.0f, perc );
            mpp->position.y = lerpedY;
            mpp->onGround = true;
        }
        break;
	case disablerMode_t::Duplicate:
		ci->mPacketSender->sendToServer(event.getPacket());
		break;
    }
}

void Disabler::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<PacketEvent, &Disabler::onPacketEvent>(this);
};

void Disabler::onDisable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<PacketEvent, &Disabler::onPacketEvent>(this);
}