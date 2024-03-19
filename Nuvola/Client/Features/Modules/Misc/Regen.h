#pragma once

#include "../../../Bridge/Types.h"
#include "../Module.h"

class LevelTickEvent;
class PacketEvent;
class Block;
struct BlockSource;
struct Player;
class Regen : public Module
{
    std::shared_ptr<Setting> Distance;
    std::shared_ptr<Setting> DestroySpeed;


public:
    Regen();
    void onEnable() override;
    void onDisable() override;
    void onLevelTickEvent(LevelTickEvent &event);
    void onPacketEvent(PacketEvent &event);

    int getBlockBreakFace(Vector3<int> pos);
    std::vector<Vector3<int>> getBlocksWithinDistance(Vector3<float> pos, float maxDistance);
    bool isAirBlock(Vector3<int> bp);
    bool isRedstoneOreExposed(BlockSource* blockSource, const Vector3<int>& pos);
    bool isAir(BlockSource* blockSource, const Vector3<int>& pos);
    BlockSource* getBlockSourceForCurrentPlayer();
    float getDestroySpeed(int slot, Block* block, float destroySpeedDivisor);

    int getBestDigger(bool hotbaronly, Player* player, BlockPos block);
};