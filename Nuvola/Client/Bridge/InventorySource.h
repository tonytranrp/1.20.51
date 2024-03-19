#pragma once

#include <stdint.h>

enum class InventorySourceType {
    Invalid = -1,
    Container = 0,
    Global = 1,
    World = 2,
    Creative = 3
};

enum class ContainerID : uint8_t {
    Invalid = 0xff,
    Inventory = 0,
    First = 1,
    Last = 100,
    Offhand = 119,
    Armor = 120,
    SelectionSlots = 122,
    PlayerUIOnly = 124
};

enum class InventorySourceFlags
{
    NoFlag = 0,
    WorldInteraction_Random = 1
};

class InventorySource
{
public:
    InventorySourceType type = InventorySourceType::Invalid;
    ContainerID containerId;
    InventorySourceFlags flags = InventorySourceFlags::NoFlag;

    InventorySource(ContainerID containerId = ContainerID::Inventory, InventorySourceType type = InventorySourceType::Container, InventorySourceFlags flags = InventorySourceFlags::NoFlag)
    {
        this->type = type;
        this->containerId = containerId;
        this->flags = flags;
    }
};
static_assert(sizeof(InventorySource) == 0xC);