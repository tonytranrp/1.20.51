#include "NoFall.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"

#include <xorstr.hpp>

enum class modes_t { VANILLA, CUBECRAFT };
static std::vector<std::string> modes = { "Vanilla", "Cubecraft" };

NoFall::NoFall() : Module("NoFall", "Prevents the player from taking fall damage.")
{
    this->mode = this->addItem<Setting>("Mode", "The type of nofall to be used.", SettingType::ENUM, 0, 0, modes.size() - 1, modes);
}

void NoFall::onEnable() 
{
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &NoFall::onLevelTickEvent>(this);
    dispatcher->listen<PacketEvent, &NoFall::onPacketEvent>(this);
}
void NoFall::onDisable() 
{
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &NoFall::onLevelTickEvent>(this);
    dispatcher->deafen<PacketEvent, &NoFall::onPacketEvent>(this);
}

void NoFall::onTick() {}

void NoFall::onLevelTickEvent(LevelTickEvent& event)
{
    
}

static bool zeroYVel = false;
void NoFall::onPacketEvent(PacketEvent &event) 
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return;

    modes_t mode = static_cast<modes_t>(this->mode->getValueAs<int>());
    if (mode == modes_t::CUBECRAFT)
    {
        zeroYVel = !zeroYVel;
        PlayerAuthInputPacket* paip = nullptr;
        if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket) {
            paip = (PlayerAuthInputPacket*)event.getPacket();

            if (zeroYVel && player->getFallDistance() > 0.2f)
                paip->downVelocity = 0.0f;
        }
    }
    if (mode == modes_t::VANILLA)
    {
        if (event.getPacket()->getId() == PacketID::EntityFallPacket)
        {
            event.setCancelled(true);
        }

        bool overVoid = true;
        int groundy = 0;

        for (int y = player->getBlockPos().y; y > -64; y--)
        {
            int x = player->getBlockPos().x;
            int z = player->getBlockPos().z;

            const Block* block = blockSource->getBlock(Vector3<int>(x, y, z));
            if (block->blockLegacy != nullptr && !block->blockLegacy->isAir())
            {
                overVoid = false;
                groundy = y;
                break;
            }
        }


        if (!overVoid)
        {
            PlayerAuthInputPacket* paip = nullptr;
            if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket) {
                paip = (PlayerAuthInputPacket*)event.getPacket();

                if (paip->position.y - (groundy+4) > 1.0f)
                {
                    paip->position.y = groundy+4;
                }
                paip->downVelocity = 0.0f;
            }
        }
    }
}