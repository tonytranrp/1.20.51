#pragma once

#include "HashedString.h"
#include "Macro.h"

#include <cstdint>

enum class BlockId : uint16_t
{
    AIR = 0,
    GOLD_ORE = 14,
    IRON_ORE = 15,
    COAL_ORE = 16,
    LAPIZ_ORE = 21,
    CHEST = 54,
    DIAMOND_ORE = 56,
    FURNACE = 61,
    REDSTONE_ORE = 73,
    LIT_REDSTONE_ORE = 74,
    EMERALD_ORE = 129,
    ENDER_CHEST = 130,
    TRAPPED_CHEST = 146,
    BARREL = 458
};

enum BlockRenderLayer : int32_t {
    RENDERLAYER_DOUBLE_SIDED = 0x0,
    RENDERLAYER_RAY_TRACED_WATER = 0x1,
    RENDERLAYER_BLEND = 0x2,
    RENDERLAYER_OPAQUE = 0x3,
    RENDERLAYER_OPTIONAL_ALPHATEST = 0x4,
    RENDERLAYER_ALPHATEST = 0x5,
    RENDERLAYER_SEASONS_OPAQUE = 0x6,
    RENDERLAYER_SEASONS_OPTIONAL_ALPHATEST = 0x7,
    RENDERLAYER_ALPHATEST_SINGLE_SIDE = 0x8,
    RENDERLAYER_ENDPORTAL = 0x9,
    RENDERLAYER_BARRIER = 0xA,
    RENDERLAYER_LIGHT = 0xB,
    RENDERLAYER_STRUCTURE_VOID = 0xC,
    _RENDERLAYER_COUNT = 0xD,
};

class BlockLegacy
{
public:
    char pad_0008[32]; //0x0008
    std::string tileName; //0x0028
    HashedString itemKey; //0x0048
    std::string itemNamespace; //0x0078
    HashedString itemId; //0x0098
    char pad_00C8[120]; //0x00C8
    std::string itemGroup; //0x0140
    char pad_0160[70]; //0x0160
    BlockId blockId; //0x01A6
	char pad_01A8[360]; //0x01A8
	class Block* renderBlock; //0x0310

	virtual ~BlockLegacy() = default;
    bool isAir() const
    {
        return vftCall<29, bool, const BlockLegacy*>(this);
    }
};