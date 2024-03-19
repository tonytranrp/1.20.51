#include "Regen.h"

#include "../../../Bridge/DiggerItem.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/PlayerInventory.h"
#include "../../../Bridge/Block.h"
#include "../../../Bridge/BlockLegacy.h"
#include "../../../Bridge/BlockSource.h"
#include "../../../Bridge/GameMode.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"

#include <algorithm>
using namespace std;

Regen::Regen() : Module("Regen", "Automatically breaks redstone ore on The Hive: SkyWars.")
{
    this->Distance = this->addItem<Setting>("Distance", "Distance", SettingType::SLIDER_FLOAT, 5.1f, 0.f, 10.f);
    this->DestroySpeed = this->addItem<Setting>("Destroy Speed", "The Destroy Speed, in seconds", SettingType::SLIDER_FLOAT, 0.70f, 0.f, 1.f);
}

static std::map <int, Vector3<float>> SideMap = {
        {0, Vector3(0.f, 0.5f, 0.f)},
        {1, Vector3(0.f, -0.5f, 0.f)},
        {2, Vector3(0.f, 0.f, 0.5f)},
        {3, Vector3(0.f, 0.f, -0.5f)},
        {4, Vector3(0.5f, 0.f, 0.f)},
        {5, Vector3(-0.5f, 0.f, 0.f)}
};

static std::map<int, Vector3<int>> SideOffset = {
        {0, BlockPos(0, 1, 0)},
        {1, BlockPos(0, -1, 0)},
        {2, BlockPos(0, 0, 1)},
        {3, BlockPos(0, 0, -1)},
        {4, BlockPos(1, 0, 0)},
        {5, BlockPos(-1, 0, 0)}
};

/*
float Misc::GetDestroySpeed(int slot, Block *block, float destroySpeedDivisor = 1.0f) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;
    auto mp = player->GetMovementProxy();
    if (!mp) return -1;
    bool onGround = player->isOnGround();
    player->setOnGround(true);
    int currentSlot = player->getsupplies()->selectedHotbarSlot;
    player->SelectSlot(slot);
    float destroySpeed = player->gamemode->getDestroyRate(*block) / destroySpeedDivisor;
    player->SelectSlot(currentSlot);
    player->setOnGround(onGround);
    return destroySpeed;
}
 */

float Regen::getDestroySpeed(int slot, Block* block, float destroySpeedDivisor = 1.0f) {
    auto ci = Bridge::getClientInstance();
    if (!ci) return -1;
    auto player = ci->getClientPlayer();

    if (!player) return -1;

    bool onGround = player->isOnGround();
    player->setOnGround(true);
    int currentSlot = player->mInventory->selectedSlot;
    player->mInventory->selectedSlot = slot;
    float destroySpeed = player->mGameMode->getDestroyRate(block) / destroySpeedDivisor;
    player->mInventory->selectedSlot = currentSlot;
    player->setOnGround(onGround);

    return destroySpeed;
}

static bool isBreaking = false;
static Vector3<int> currentBlock = Vector3(0, 0, 0);
static float breakProgress = 0.0f;
static bool shouldRotate = true;


# define M_PI 3.14159265358979323846 /* pi */

Vector2<float> GetRotations(Vector3<float> playerEyePos, Vector3<float>* targetPos) {
    Vector3<float> delta = *targetPos - playerEyePos;
    float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
    float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
    return {-pitch, yaw - 90};
}

void Regen::onEnable()
{
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Regen::onLevelTickEvent>(this);
    dispatcher->listen<PacketEvent, &Regen::onPacketEvent>(this);
}

int Regen::getBestDigger(bool hotbaronly, Player* player, BlockPos block) {
    int slots = hotbaronly ? 9 : player->mInventory->container->stacks.size();
    float bestspeed = 0.0f;
    int bestslot = -1;
    for (int slot = 0; slots > slot; slot++) {
        ItemStack* itemstack = &player->mInventory->container->stacks[slot];
        Item* item = itemstack->item.get();
        if (item != nullptr && item->hasRecipeTag("minecraft:digger")) {
            float speed = reinterpret_cast<DiggerItem*>(item)->getDestroySpeed(itemstack, player->dimension->getBlockSource()->getBlock(block));
            if (speed > bestspeed) {
                bestslot = slot;
                bestspeed = speed;
            }
        }
    }
    return bestslot;
}


bool Regen::isRedstoneOreExposed(BlockSource* blockSource, const Vector3<int>& pos) {

    int exposedSides = 0;

    if (isAir(blockSource, pos + Vector3<int>(1, 0, 0))) exposedSides++;
    if (isAir(blockSource, pos + Vector3<int>(-1, 0, 0))) exposedSides++;
    if (isAir(blockSource, pos + Vector3<int>(0, 1, 0))) exposedSides++;
    if (isAir(blockSource, pos + Vector3<int>(0, -1, 0))) exposedSides++;
    if (isAir(blockSource, pos + Vector3<int>(0, 0, 1))) exposedSides++;
    if (isAir(blockSource, pos + Vector3<int>(0, 0, -1))) exposedSides++;

    return exposedSides > 0;
}

bool Regen::isAir(BlockSource* blockSource, const Vector3<int>& pos) {
    const Block* block = blockSource->getBlock(pos);
    return block && block->blockLegacy->blockId == BlockId::AIR;
}

void Regen::onLevelTickEvent(LevelTickEvent &event) {
    if (shouldRotate) shouldRotate = false;

    // Define some variables
    ClientInstance *ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer *player = ci->getClientPlayer();
    if (player == nullptr) return;
    PlayerInventory *inventory = player->mInventory;
    if (inventory == nullptr) return;

    // Get blocks near the player based on the distance setting
    Vector3<int> playerPos = player->getPosition();
    float dist = this->Distance->getValueAs<float>();
    std::vector<Vector3<int>> blocks = getBlocksWithinDistance(playerPos, dist);



    // Loop through all the blocks

    int side = -1;

    bool hasBlock = false;
    bool outBool = false;

    for (auto block : blocks) {
        if (currentBlock != Vector3<float>(0, 0, 0)) {
            block = currentBlock;
        } else {
            currentBlock = block;
        }

        side = getBlockBreakFace(currentBlock);

        // Check the distance and continue if it's too far
        if (playerPos.distanceTo(block) > dist) continue;
        // Check if the block is still exposed with getBlockBreakFace
        if (getBlockBreakFace(block) == -1) continue;
        // Check if the block is a redstone ore
        BlockId id = getBlockSourceForCurrentPlayer()->getBlock(block)->blockLegacy->blockId;
        if (id != BlockId::REDSTONE_ORE && id != BlockId::LIT_REDSTONE_ORE) continue;

        int slot = getBestDigger(false, player, currentBlock);

        const Block* blocki = getBlockSourceForCurrentPlayer()->getBlock(block);
        float destroySpeed = getDestroySpeed(slot, const_cast<Block *>(blocki), DestroySpeed->getValueAs<float>());

        hasBlock = true;

        if (!isBreaking) {
            currentBlock = block;
            Vector3<float> ppos = player->stateVectorComponent->currentPos;

            player->mGameMode->startDestroyBlock(&currentBlock, side, outBool);
            player->mGameMode->continueDestroyBlock(currentBlock, side, ppos, outBool);
            isBreaking = true;
        } else {

            currentBlock = block;
            breakProgress += destroySpeed;
        }

        if (breakProgress >= 1.0f) {
            shouldRotate = true;
            player->swing();

            bool oldIsDestroying = player->isDestroying;

            player->isDestroying = true;
            player->mGameMode->destroyProgress = 1.f;

            int oldSlot = inventory->selectedSlot;
            inventory->selectedSlot = slot;
            player->mGameMode->destroyBlock(&currentBlock, side);
            inventory->selectedSlot = oldSlot;

            player->mGameMode->destroyProgress = 0.f;
            player->isDestroying = oldIsDestroying;

            isBreaking = false;
            breakProgress = 0.0f;

        }

        break;
    }

    if (!hasBlock) {
        currentBlock = Vector3(0, 0, 0);
        isBreaking = false;
        breakProgress = 0.0f;
    }
}

void Regen::onPacketEvent(PacketEvent& event) {
    if (!shouldRotate) return;

    PlayerAuthInputPacket *paip = nullptr;
    MovePlayerPacket *mpp = nullptr;

    if (event.getPacket()->getId() == PacketID::MovePlayerPacket) {
        mpp = (MovePlayerPacket *) event.getPacket();
    } else if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket) {
        paip = (PlayerAuthInputPacket *) event.getPacket();
    } else {
        return;
    }

    ClientInstance *ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer *player = ci->getClientPlayer();
    if (player == nullptr) return;
    Vector3<float> blockF = currentBlock;
    auto rot = GetRotations(player->stateVectorComponent->currentPos, &blockF);

    if (paip != nullptr) {
        paip->rotation = rot;
        paip->headYaw = rot.y;
    }

}



std::vector<Vector3<int>> Regen::getBlocksWithinDistance(Vector3<float> pos, float maxDistance) {
    ClientInstance *ci = Bridge::getClientInstance();
    if (ci == nullptr) return {};
    LocalPlayer *player = ci->getClientPlayer();
    if (player == nullptr) return {};
    BlockSource *bs = getBlockSourceForCurrentPlayer();

    std::vector<Vector3<int>> blocks;

    for (int x = pos.x - maxDistance; x < pos.x + maxDistance; x++) {
        for (int y = pos.y - maxDistance; y < pos.y + maxDistance; y++) {
            for (int z = pos.z - maxDistance; z < pos.z + maxDistance; z++) {
                Vector3<int> blockPos = Vector3<int>(x, y, z);
                const Block* block = bs->getBlock(blockPos);
                if (block->blockLegacy->blockId == BlockId::REDSTONE_ORE || block->blockLegacy->blockId == BlockId::LIT_REDSTONE_ORE) {
                    blocks.push_back(blockPos);
                }
            }
        }
    }

    // Sort the blocks by distance to the player (player->getPosition())
    std::sort(blocks.begin(), blocks.end(), [pos](Vector3<int> a, Vector3<int> b) {
        return pos.distanceTo(a) < pos.distanceTo(b);
    });

    return blocks;
}

int Regen::getBlockBreakFace(Vector3<int> bp) {
    if (isAirBlock(bp + Vector3<int>(0, -1, 0))) return 1;
    if (isAirBlock(bp + Vector3<int>(0, 0, 1))) return 2;
    if (isAirBlock(bp + Vector3<int>(0, 0, -1))) return 3;
    if (isAirBlock(bp + Vector3<int>(1, 0, 0))) return 4;
    if (isAirBlock(bp + Vector3<int>(-1, 0, 0))) return 5;
    if (isAirBlock(bp + Vector3<int>(0, 1, 0))) return 0;

    return -1;
}

BlockSource* Regen::getBlockSourceForCurrentPlayer() {
    ClientInstance* ci = Bridge::getClientInstance();

    if (ci == nullptr) {
        return nullptr;
    }

    LocalPlayer* player = ci->getClientPlayer();

    if (player == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Dimension> dim = player->dimension;

    if (dim == nullptr) {
        return nullptr;
    }

    return dim->blockSource;
}

bool Regen::isAirBlock(Vector3<int> bp) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return false;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return false;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return false;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return false;
    const Block* block = blockSource->getBlock(bp);
    if (block->blockLegacy == nullptr) return false;
    return block->blockLegacy->isAir();
}

void Regen::onDisable()
{
    isBreaking = false;
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Regen::onLevelTickEvent>(this);
    dispatcher->deafen<PacketEvent, &Regen::onPacketEvent>(this);
    breakProgress = 0.0f;
    currentBlock = Vector3(0, 0, 0);

}