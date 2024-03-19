#pragma once

#include "../../../Bridge/Types.h"

#include "../Module.h"

class BlockStartDestroyEvent;
class AttackActorEvent;
class Autotool : public Module
{
	std::shared_ptr<Setting> hotbaronly = nullptr;
	std::shared_ptr<Setting> mining = nullptr;
	std::shared_ptr<Setting> attacking = nullptr;
	class ItemStack* olditem = nullptr;
public:
	Autotool();
	virtual void onEnable() override;
	virtual void onDisable() override;
	void onBlockStartDestroyEvent(BlockStartDestroyEvent& event);
	void onAttackActorEvent(AttackActorEvent& event);
	void swapSlots(int slot1, int slot2, class Player* player) const;
	int getEmptyHotbarSlot(class Player* player) const;
	void moveItemIntoHotbar(int itemslot, class Player* player, class ClientInstance*) const;
	int getBestDigger(bool hotbaronly, class Player* player, BlockPos block) const;
	int getBestSword(bool hotbaronly, class Player* player) const;
	void sendEquipPacket(int slot, class Player* player, class ClientInstance* ci) const;
};