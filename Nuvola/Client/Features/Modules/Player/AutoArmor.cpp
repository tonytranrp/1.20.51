#include "AutoArmor.h"

#include "../../../Bridge/ArmorItem.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ContainerScreenController.h"
#include "../../../Bridge/EnchantUtils.h"
#include "../../../Bridge/InventoryTransactionManager.h"
#include "../../../Bridge/ItemStack.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/ContainerScreenControllerTickEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

#include <magic_enum.hpp>
#include <magic_enum_utility.hpp>

enum class cleanModes_t { LEGIT, TRANSACTIONS };
static std::vector<std::string> modes = { "Legit", "Transactions" };

AutoArmor::AutoArmor() : Module("AutoArmor", "Automatically equips armor.")
{
	this->mode = this->addItem<Setting>("Mode", "The method used for moving inventory items. Legit required your inventory to be open!", SettingType::ENUM, 0, 0, modes.size() - 1, modes);
	this->delay = this->addItem<Setting>("Delay", "The delay in ticks between each inventory action.", SettingType::SLIDER_INT, 10, 0, 100);
	this->discardUseless = this->addItem<Setting>("Discard Useless", "Automatically drops useless armor on the ground.", SettingType::TOGGLE, true, false, true);

	std::map<EnchantType, float> defaultWeights = {
		{EnchantType::PROTECTION, 1.5f},
		{EnchantType::BLAST_PROTECTION, 1.0f},
		{EnchantType::FIRE_PROTECTION, 1.0f},
		{EnchantType::PROJECTILE_PROTECTION, 1.0f},
		{EnchantType::UNBREAKING, 1.2f},
		{EnchantType::MENDING, 1.2f},

		{EnchantType::CURSE_OF_BINDING, -2.0f},
		{EnchantType::CURSE_OF_VANISHING, -1.0f}
	};
	this->weightsSetting = this->addItem<EnchantWeightSetting>("Enchant Weights", "How important each enchantment is when considering which armor to keep or toss.", defaultWeights);
}

static int ticksEnabled = 0;
void AutoArmor::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ContainerScreenControllerTickEvent, &AutoArmor::onContainerScreenControllerTickEvent>(this);
	dispatcher->listen<LevelTickEvent, &AutoArmor::onLevelTickEvent>(this);
	ticksEnabled = 0;
}

void AutoArmor::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ContainerScreenControllerTickEvent, &AutoArmor::onContainerScreenControllerTickEvent>(this);
	dispatcher->deafen<LevelTickEvent, &AutoArmor::onLevelTickEvent>(this);
	this->running = false;
}

bool AutoArmor::isRunning() const
{
	return this->running;
}

float AutoArmor::getArmorValue(const ItemStackBase* stack) const
{
	const Item* item = stack->item.get();
	if (item == nullptr)
	{
		return 0;
	}

	if (!item->isArmor())
	{
		return 0;
	}

	const ArmorItem* armorItem = (const ArmorItem*)item;

	float score = 0.0f;
	score += armorItem->getArmorValue();
	score += armorItem->getArmorKnockbackResistance();

	magic_enum::enum_for_each<EnchantType>([&](auto val)
	{
		constexpr EnchantType enchantType = val;
		auto setting = this->weightsSetting;
		float weight = setting->getWeight(enchantType);
		score += EnchantUtils::getEnchantLevel(enchantType, *stack) * weight;
	});

	return score;
}

void AutoArmor::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event)
{
	cleanModes_t mode = static_cast<cleanModes_t>(this->mode->getValueAs<int>());
	if (mode != cleanModes_t::LEGIT) return;

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

	//A var to store the best armor in the inventory & its slot
	std::array<std::pair<int, ItemStack*>, 4> bestArmor;

	running = true;

	//Find the best armor
	for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++)
	{
		ItemStack* stack = &player->mInventory->container->stacks[slotId];
		const Item* item = stack->item.get();
		if (item == nullptr)
		{
			continue;
		}

		if (item->isArmor())
		{
			ArmorItem* armorItem = (ArmorItem*)item;
			int armorSlot = (int)armorItem->armorSlot;
			if (bestArmor[armorSlot].second != nullptr)
			{
				WeakPtr<Item>& pBestItem = bestArmor[armorSlot].second->item;
				ArmorItem* bestItem = (ArmorItem*)pBestItem.get();
				if (bestItem)
				{
					if (getArmorValue(bestArmor[armorSlot].second) >= getArmorValue(stack))
					{
						continue;
					}
				}
			}
			bestArmor[armorSlot] = std::pair<int, ItemStack*>(slotId, stack);
		}
	}

	//Check it against the current armor
	for (int slotId = 0; player->armorContainer->stacks.size() > slotId; slotId++)
	{
		ItemStack* bestStack = bestArmor[slotId].second;
		if (bestStack == nullptr) { continue; }
		ItemStack* currentStack = &player->armorContainer->stacks[slotId];
		if (currentStack == nullptr) { continue; }

		const ArmorItem* bestItem = (ArmorItem*)bestStack->item.get();
		const ArmorItem* currentItem = (ArmorItem*)currentStack->item.get();

		if (currentItem != nullptr && bestItem != nullptr)
		{
			if (getArmorValue(bestStack) <= getArmorValue(currentStack))
			{
				//The current is the best so we will set this to null to know not to swap
				bestArmor[slotId].first = 0;
				bestArmor[slotId].second = nullptr;
			}
		}
	}

	//Apply the best armor
	int armorSlot = 0;
	for (auto [slotId, stack] : bestArmor)
	{
		if (stack == nullptr)
		{
			armorSlot++;
			continue;
		}

		bool isHotbar = true;
		int correctedSlotId = 0;
		if (slotId < 9)
			correctedSlotId = slotId;
		else
		{
			correctedSlotId = slotId - 9;
			isHotbar = false;
		}
		controller->_handleAutoPlace(ItemSource::armor_items, armorSlot);
		controller->_handleTakeAll(isHotbar ? ItemSource::hotbar_items : ItemSource::inventory_items, correctedSlotId);
		controller->_handlePlaceAll(ItemSource::armor_items, armorSlot);

		armorSlot++;
		return;
	}

	if (discardUseless->getValueAs<bool>())
	{
		for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++)
		{
			ItemStack* stack = &player->mInventory->container->stacks[slotId];
			const Item* item = stack->item.get();
			if (item == nullptr)
			{
				continue;
			}

			bool isHotbar = true;
			int correctedSlotId = 0;
			if (slotId < 9)
				correctedSlotId = slotId;
			else
			{
				correctedSlotId = slotId - 9;
				isHotbar = false;
			}

			if (item->isArmor())
			{
				controller->_handleDropItem(isHotbar ? ItemSource::hotbar_items : ItemSource::inventory_items, correctedSlotId);
				return;
			}
		}
	}

	running = false;
}

void AutoArmor::onLevelTickEvent(LevelTickEvent& event)
{
	cleanModes_t mode = static_cast<cleanModes_t>(this->mode->getValueAs<int>());
	if (mode != cleanModes_t::TRANSACTIONS) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	ticksEnabled++;
	int idelay = this->delay->getValueAs<int>();
	if (idelay != 0 && ticksEnabled % idelay != 0) return;

	//A var to store the best armor in the inventory & its slot
	std::array<std::pair<int, ItemStack*>, 4> bestArmor;

	//Find the best armor
	for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++)
	{
		ItemStack* stack = &player->mInventory->container->stacks[slotId];
		const Item* item = stack->item.get();
		if (item == nullptr)
		{
			continue;
		}

		if (item->isArmor())
		{
			ArmorItem* armorItem = (ArmorItem*)item;
			int armorSlot = (int)armorItem->armorSlot;
			if (bestArmor[armorSlot].second != nullptr)
			{
				WeakPtr<Item>& pBestItem = bestArmor[armorSlot].second->item;
				ArmorItem* bestItem = (ArmorItem*)pBestItem.get();
				if (bestItem)
				{
					if (getArmorValue(bestArmor[armorSlot].second) >= getArmorValue(stack))
					{
						continue;
					}
				}
			}
			bestArmor[armorSlot] = std::pair<int, ItemStack*>(slotId, stack);
		}
	}

	//Check it against the current armor
	for (int slotId = 0; player->armorContainer->stacks.size() > slotId; slotId++)
	{
		ItemStack* bestStack = bestArmor[slotId].second;
		if (bestStack == nullptr) { continue; }
		ItemStack* currentStack = &player->armorContainer->stacks[slotId];
		if (currentStack == nullptr) { continue; }
		
		const ArmorItem* bestItem = (ArmorItem*)bestStack->item.get();
		const ArmorItem* currentItem = (ArmorItem*)currentStack->item.get();

		if (currentItem != nullptr && bestItem != nullptr)
		{
			if (getArmorValue(bestStack) <= getArmorValue(currentStack))
			{
				//The current is the best so we will set this to null to know not to swap
				bestArmor[slotId].first = 0;
				bestArmor[slotId].second = nullptr;
			}
		}
	}

	//Apply the best armor
	int armorSlot = 0;
	for (auto [slotId, stack] : bestArmor)
	{
		if (stack == nullptr)
		{
			armorSlot++;
			continue;
		}

		ItemStack* current = (ItemStack*)&player->getArmor((ArmorSlot)armorSlot);
		InventoryAction firstAction(
			InventorySource(
				ContainerID::Inventory
			),
			slotId,
			*stack,
			*current);

		InventoryAction secondAction(
			InventorySource(
				ContainerID::Armor
			),
			armorSlot,
			*current,
			*stack);

		player->getTransactionManager()->addAction(firstAction, false);
		player->getTransactionManager()->addAction(secondAction, false);

		ItemStack clone = *current;
		*current = *stack;
		*stack = clone;

		armorSlot++;
		return;
	}

	if (discardUseless->getValueAs<bool>())
	{
		for (int slotId = 0; player->mInventory->container->stacks.size() > slotId; slotId++)
		{
			ItemStack* stack = &player->mInventory->container->stacks[slotId];
			const Item* item = stack->item.get();
			if (item == nullptr)
			{
				continue;
			}

			if (item->isArmor())
			{
				ItemStack worldNull;
				worldNull.setNull();
				InventoryAction firstAction(
					InventorySource(
						ContainerID::Invalid,
						InventorySourceType::World
					),
					0,
					worldNull,
					*stack);

				InventoryAction secondAction(
					InventorySource(
						ContainerID::Inventory
					),
					slotId,
					*stack,
					worldNull);

				player->getTransactionManager()->addAction(secondAction, false);
				player->getTransactionManager()->addAction(firstAction, false);
				return;
			}
		}
	}
}