#include "AntiCrystal.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"

#include <optional>

AntiCrystal::AntiCrystal() : Module("AntiCrystal", "anti-crystal you know?") {
    this->mode = this->addItem<ModeSetting<antiCrystalMode_t>>("Mode", "How AntiCrystal should help you prevent crystal damage.");
    const auto& advancedManager = this->mode->getManagerFor(antiCrystalMode_t::Advanced);
    this->downMax = advancedManager->addItem<Setting>("Down Max", "The maximum amount of blocks to search for a safe location.", SettingType::SLIDER_INT, 3, 0, 10);
    this->hideJump = advancedManager->addItem<Setting>("Hide Jumps", "Prevents jumps from being sent to the server, useful for servers with authoritive movement.", SettingType::TOGGLE, true, false, true);
    this->depth = advancedManager->addItem<Setting>("Depth", "How far down into the block the player should be pushed.", SettingType::SLIDER_FLOAT, 1.0f, 0.0f, 1.0f);
    this->forceCentered = advancedManager->addItem<Setting>("Force Centered", "Forces the player to be in the center of the block.", SettingType::TOGGLE, false, false, true);
    this->jumpDisable = advancedManager->addItem<Setting>("Jump Disable", "Disables anti crystal while the player is jumping.", SettingType::TOGGLE, false, false, true);
};

void AntiCrystal::onPacketEvent(PacketEvent& event)
{
    ClientInstance* ci = Bridge::getClientInstance();
    CHECK(ci);
    LocalPlayer* player = ci->getClientPlayer();
    CHECK(player);
    auto dim = player->dimension;
    CHECK(dim);
    auto bsource = dim->blockSource;
    CHECK(bsource);

    antiCrystalMode_t mode = this->mode->getCurrentMode();

    if (this->jumpDisable->getValueAs<bool>())
    {
        if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
        {
            PlayerAuthInputPacket* paip = reinterpret_cast<PlayerAuthInputPacket*>(event.getPacket());
            if ((paip->inputData & AuthInputAction::JUMP_DOWN) == AuthInputAction::JUMP_DOWN)
            {
                return;
            }
        }
        if (event.getPacket()->getId() == PacketID::MovePlayerPacket)
        {
            MovePlayerPacket* mpp = reinterpret_cast<MovePlayerPacket*>(event.getPacket());
            //For MPP we can assume not being on ground = not jumping
            if (!mpp->onGround)
            {
                return;
            }
        }
    }

    switch (mode)
    {
    case antiCrystalMode_t::Basic:
        if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
        {
            PlayerAuthInputPacket* paip = reinterpret_cast<PlayerAuthInputPacket*>(event.getPacket());
            paip->position.y -= 1.0f;
        }
        if (event.getPacket()->getId() == PacketID::MovePlayerPacket)
        {
            MovePlayerPacket* mpp = reinterpret_cast<MovePlayerPacket*>(event.getPacket());
            mpp->position.y -= 1.0f;
        }
        break;
    case antiCrystalMode_t::Advanced:
    {
        auto dmVal = this->downMax->getValueAs<int>();
        std::optional<BlockPos> safestBlock;
        for (int y = player->getBlockPos().y; y > player->getBlockPos().y - dmVal; y--)
        {
            int x = player->getBlockPos().x;
            int z = player->getBlockPos().z;

            const Block* block = bsource->getBlock(x, y, z);
            if (block->blockLegacy != nullptr && !block->blockLegacy->isAir())
            {
                safestBlock = BlockPos{ x, y, z };
                break;
            }
        }

        if (safestBlock.has_value())
        {
            float depthVal = this->depth->getValueAs<float>();
            float hitboxHeight = player->aabbComponent->height;
            float calcedY = (safestBlock->y + (1.0f - depthVal)) + hitboxHeight;

            if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
            {
                PlayerAuthInputPacket* paip = reinterpret_cast<PlayerAuthInputPacket*>(event.getPacket());
                paip->position.y = calcedY;

                if (this->forceCentered->getValueAs<bool>())
                {
                    paip->position.x = floor(paip->position.x) + 0.5f;
                    paip->position.z = floor(paip->position.z) + 0.5f;
                }

                if (this->hideJump->getValueAs<bool>())
                {
                    paip->inputData &= AuthInputAction::JUMPING;
                    paip->inputData &= AuthInputAction::START_JUMPING;
                    paip->inputData &= AuthInputAction::JUMP_DOWN;
                    paip->inputData &= AuthInputAction::AUTO_JUMPING_IN_WATER;
                    paip->inputData &= AuthInputAction::NORTH_JUMP;
                }
            }
            if (event.getPacket()->getId() == PacketID::MovePlayerPacket)
            {
                MovePlayerPacket* mpp = reinterpret_cast<MovePlayerPacket*>(event.getPacket());
                mpp->position.y = calcedY;

                if (this->forceCentered->getValueAs<bool>())
                {
                    mpp->position.x = floor(mpp->position.x) + 0.5f;
                    mpp->position.z = floor(mpp->position.z) + 0.5f;
                }
            }
        }
        break;
    }
    }
}

void AntiCrystal::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<PacketEvent, &AntiCrystal::onPacketEvent>(this);
};

void AntiCrystal::onDisable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<PacketEvent, &AntiCrystal::onPacketEvent>(this);
}