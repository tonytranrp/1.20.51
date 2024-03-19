#include "ChestStealer.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ContainerScreenController.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/IContainerManager.h"
#include "../../../Events/ContainerScreenControllerTickEvent.h"
#include "../../../Events/EventDispatcher.h"

ChestStealer::ChestStealer() : Module("ChestStealer", "Automatically moves items from chests into your inventory.")
{
	this->delay = this->addItem<Setting>("Delay", "The amount of ticks between each automatic inventory action.", SettingType::SLIDER_INT, 5, 0, 100);
}

static int ticksEnabled = 0;
void ChestStealer::onEnable()
{
	ticksEnabled = 0;
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ContainerScreenControllerTickEvent, &ChestStealer::onContainerScreenControllerTickEvent>(this);
}

void ChestStealer::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ContainerScreenControllerTickEvent, &ChestStealer::onContainerScreenControllerTickEvent>(this);
}

void ChestStealer::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	ticksEnabled++;
	int idelay = delay->getValueAs<int>();
	if (idelay != 0 && ticksEnabled % idelay != 0) return;

	std::weak_ptr<IContainerManager> wpManager = player->mContainerManager;
	std::shared_ptr<IContainerManager> manager = wpManager.lock();
	if (manager == nullptr) return;

	ContainerType type = manager->getContainerType();
	if (type == ContainerType::CHEST)
	{
		ContainerScreenController* controller = (ContainerScreenController*)event.getController();
		for (int currentSlot = 0; currentSlot < 9 * 3; currentSlot++)
		{
			int8_t containerId = manager->getContainerId();
			ItemStack& currentStack = manager->getSlot(currentSlot);
			if (!currentStack.item)
			{
				continue;
			}

			const int emptySlot = player->mInventory->container->getEmptySlotFromContainer();
			if (emptySlot == -1) /* No empty slots available */
			{
				break;
			}

			controller->_handleAutoPlace(ItemSource::container_items, currentSlot);
			break;
		}
	}
}
