#include "AutoTotem.h"

#include "../../../Bridge/Components/RuntimeIDComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ContainerScreenController.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/MobEquipmentPacket.h"
#include "../../../Bridge/MinecraftPackets.h"

#include "../../../Events/ContainerScreenControllerTickEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../Render/Notifications.h"
#include "../ModuleManager.h"


AutoTotem::AutoTotem() : Module("AutoTotem", "Automatically equips totems.")
{
	this->mode = this->addItem<ModeSetting<autoTotemModes_t>>("Mode", "The method used for moving inventory items. Legit required your inventory to be open!");
	this->delay = this->addItem<Setting>("Delay", "The delay in ticks between each inventory action.", SettingType::SLIDER_INT, 10, 0, 100);
}

static int ticksEnabled = 0;
void AutoTotem::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ContainerScreenControllerTickEvent, &AutoTotem::onContainerScreenControllerTickEvent>(this);
	dispatcher->listen<LevelTickEvent, &AutoTotem::onLevelTickEvent>(this);
	ticksEnabled = 0;
}

void AutoTotem::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ContainerScreenControllerTickEvent, &AutoTotem::onContainerScreenControllerTickEvent>(this);
	dispatcher->deafen<LevelTickEvent, &AutoTotem::onLevelTickEvent>(this);
}

void AutoTotem::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event)
{
	autoTotemModes_t mode = this->mode->getCurrentMode();
	if (mode != autoTotemModes_t::Legit) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	std::weak_ptr<IContainerManager> wpManager = player->mContainerManager;
	std::shared_ptr<IContainerManager> manager = wpManager.lock();
	if (manager != nullptr)
	{
		if (manager->getContainerType() != ContainerType::INVENTORY)
		{
			return;
		}
	}

	ticksEnabled++;
	int idelay = delay->getValueAs<int>();
	if (idelay != 0 && ticksEnabled % idelay != 0) return;

	ContainerScreenController* controller = reinterpret_cast<ContainerScreenController*>(event.getController());
	
	ItemStack* offhandstack = &player->handContainer->stacks[1];
	if (offhandstack->item.get() == nullptr) {
		for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++) {
			ItemStack* stack = &player->mInventory->container->stacks[slotId];
			if (stack->item.get() == nullptr) continue;
			if (stack->item.get()->texture_name == "totem") { 
				bool isHotbar = true;
				int correctedSlotId = 0;
				if (slotId < 9)
					correctedSlotId = slotId;
				else
				{
					correctedSlotId = slotId - 9;
					isHotbar = false;
				}
				controller->_handleAutoPlace(isHotbar ? ItemSource::hotbar_items : ItemSource::inventory_items, correctedSlotId);
				break;
			}
			
		}
	} 
	else {
		return;
	}

}

void AutoTotem::onLevelTickEvent(LevelTickEvent& event)
{
	autoTotemModes_t mode = this->mode->getCurrentMode();
	if (mode != autoTotemModes_t::Transactions) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	ticksEnabled++;
	int idelay = delay->getValueAs<int>();
	if (idelay != 0 && ticksEnabled % idelay != 0) return;

	ItemStack* offhandstack = &player->handContainer->stacks[1];
	if (offhandstack->item.get() == nullptr) {
		for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++) {
			ItemStack* stack = &player->mInventory->container->stacks[slotId];
			if (stack->item.get() == nullptr) continue;
			if (stack->item.get()->texture_name == "totem") {

				//do transaction stuff
				InventoryAction firstAction(
					InventorySource(
						ContainerID::Inventory
					),
					slotId,
					*stack,
					*offhandstack);

				InventoryAction secondAction(
					InventorySource(
						ContainerID::Offhand
					),
					0,
					*offhandstack,
					*stack);

				player->getTransactionManager()->addAction(firstAction, false);
				player->getTransactionManager()->addAction(secondAction, false);

				ItemStack clone = *offhandstack;
				*offhandstack = *stack;
				*stack = clone;

				auto packet = MinecraftPackets::createPacket(PacketID::MobEquipmentPacket);
				auto packetcasted = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
	
				packetcasted->ContainerID = 119;
				packetcasted->ContainerID2 = 119;
				packetcasted->InventorySlot = 1;
				packetcasted->InventorySlot2 = 1;
				packetcasted->HotbarSlot = 0;
				packetcasted->HotbarSlot2 = 0;
				ItemStack* updatedstack = &player->handContainer->stacks[1];
				NetworkItemStackDescriptor itemstackdescriptor(*updatedstack);
				packetcasted->ItemStackDescriptor = itemstackdescriptor;
				packetcasted->ActorRuntimeID = player->entityContext.tryGetComponent<RuntimeIDComponent>()->runtimeId;
				ci->getPacketSender()->sendToServer(packetcasted.get());
				break;
			}

		}
	}
	else {
		return;
	}
}