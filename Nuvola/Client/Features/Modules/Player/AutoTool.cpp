#include "AutoTool.h"

#include "../../../Bridge/Components/RuntimeIDComponent.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/DiggerItem.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/MinecraftPackets.h"
#include "../../../Bridge/MobEquipmentPacket.h"

#include "../../../Events/AttackActorEvent.h"
#include "../../../Events/BlockStartDestroyEvent.h"
#include "../../../Events/EventDispatcher.h"

Autotool::Autotool() : Module("AutoTool", "Automatically switch to the most effective tool.")
{
	this->hotbaronly = this->addItem<Setting>("Hotbar Only", "Wether to only switch to tools in the hotbar.", SettingType::TOGGLE, true, false, true);
	this->mining = this->addItem<Setting>("Auto digger", "Wether to switch to the best digging tool when mining.", SettingType::TOGGLE, true, false, true);
	this->attacking = this->addItem<Setting>("Auto sword", "Wether to switch to the best attacking tool when attacking.", SettingType::TOGGLE, true, false, true);
}

void Autotool::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<AttackActorEvent, &Autotool::onAttackActorEvent>(this);
	dispatcher->listen<BlockStartDestroyEvent, &Autotool::onBlockStartDestroyEvent>(this);
}

void Autotool::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<AttackActorEvent, &Autotool::onAttackActorEvent>(this);
	dispatcher->deafen<BlockStartDestroyEvent, &Autotool::onBlockStartDestroyEvent>(this);
}

void Autotool::swapSlots(int slot1, int slot2, Player* player) const{
	InventoryAction firstAction(
		InventorySource(
			ContainerID::Inventory
		),
		slot1,
		player->mInventory->container->stacks[slot1],
		player->mInventory->container->stacks[slot2]);

	InventoryAction secondAction(
		InventorySource(
			ContainerID::Inventory
		),
		slot2,
		player->mInventory->container->stacks[slot2],
		player->mInventory->container->stacks[slot1]);

	player->getTransactionManager()->addAction(firstAction, false);
	player->getTransactionManager()->addAction(secondAction, false);

	ItemStack clone = player->mInventory->container->stacks[slot1];
	player->mInventory->container->stacks[slot1] = player->mInventory->container->stacks[slot2];
	player->mInventory->container->stacks[slot2] = clone;

}

int Autotool::getEmptyHotbarSlot(Player* player) const{
	int emptyslot = -1;
	for (int slotId = 0; 9 > slotId; slotId++) {
		if (player->mInventory->container->stacks[slotId].item.get() == nullptr) { emptyslot = slotId; break; }
	}
	return emptyslot;
}

void Autotool::moveItemIntoHotbar(int slot, Player* player, ClientInstance* ci) const {
	int empty = this->getEmptyHotbarSlot(player);
	
	if (empty < 0) {
		this->swapSlots(player->mInventory->selectedSlot, slot, player);
	}
	else if (empty < 9) {
		this->swapSlots(empty, slot, player);
		this->sendEquipPacket(empty, player, ci);
		player->mInventory->selectedSlot = empty;
	}
}

int Autotool::getBestDigger(bool hotbaronly, Player* player, BlockPos block) const {
	int slots = hotbaronly ? 9 : player->mInventory->container->stacks.size();
	float bestspeed = 0.0f;
	int bestslot = -1;
	for (int slot = 0; slots > slot; slot++) {
		ItemStack* itemstack = &player->mInventory->container->stacks[slot];
		const Item* item = itemstack == nullptr ? nullptr : itemstack->item.get();
		if (item != nullptr && item->hasRecipeTag("minecraft:digger")) { 
			float speed = reinterpret_cast<const DiggerItem*>(item)->getDestroySpeed(itemstack, player->dimension->getBlockSource()->getBlock(block));
			if (speed > bestspeed) {
				bestslot = slot;
				bestspeed = speed;
			}
		}
	}
	return bestslot;
}

int Autotool::getBestSword(bool hotbaronly, Player* player) const {
	int bestdamage = 0;
	int bestslot = -1;
	int slots = hotbaronly ? 9 : player->mInventory->container->stacks.size();
	for (int slot = 0; slots > slot; slot++) {
		ItemStack* itemstack = &player->mInventory->container->stacks[slot];
		Item* item = itemstack->item.get();
		if (item != nullptr && item->hasRecipeTag("minecraft:is_sword")) {
			int damage = item->getAttackDamage();
			if (damage > bestdamage) {
				bestslot = slot;
				bestdamage = damage;
			}
		}
	}
	return bestslot;
}

void Autotool::sendEquipPacket(int slot, Player* player, ClientInstance* ci) const {
	auto packet = MinecraftPackets::createPacket(PacketID::MobEquipmentPacket);
	auto packetcasted = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
	packetcasted->ContainerID = 0;
	packetcasted->ContainerID2 = 0;
	packetcasted->InventorySlot = slot;
	packetcasted->InventorySlot2 = slot;
	packetcasted->HotbarSlot = slot;
	packetcasted->HotbarSlot2 = slot;
	NetworkItemStackDescriptor itemstackdescriptor(player->mInventory->container->stacks[slot]);
	packetcasted->ItemStackDescriptor = itemstackdescriptor;
	packetcasted->ActorRuntimeID = player->entityContext.tryGetComponent<RuntimeIDComponent>()->runtimeId;
	ci->getPacketSender()->sendToServer(packetcasted.get());
}
void Autotool::onBlockStartDestroyEvent(BlockStartDestroyEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	if (!this->mining->getValueAs<bool>()) return;
	int bestslot = this->getBestDigger(this->hotbaronly->getValueAs<bool>(), player, *event.getBlockPos());
	if (bestslot < 0) return;
	if (bestslot < 9) {
		this->sendEquipPacket(bestslot, player, ci);
		player->mInventory->selectedSlot = bestslot;
		return; 
	}
	this->moveItemIntoHotbar(bestslot, player, ci);
}

void Autotool::onAttackActorEvent(AttackActorEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	if (player->mGameMode != event.getGameMode()) return;

	if (!this->attacking->getValueAs<bool>()) return;
	int bestslot = this->getBestSword(this->hotbaronly->getValueAs<bool>(), player);
	if (bestslot < 0) return;
	if (bestslot < 9) { 
		
		this->sendEquipPacket(bestslot, player, ci);
		player->mInventory->selectedSlot = bestslot;
	
		return;
	}
	this->moveItemIntoHotbar(bestslot, player, ci);
}