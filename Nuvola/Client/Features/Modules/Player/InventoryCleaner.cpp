#include "InventoryCleaner.h"

#include "AutoArmor.h"
#include "../ModuleManager.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ContainerScreenController.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/EnchantUtils.h"
#include "../../../Events/ContainerScreenControllerTickEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

static std::vector<std::string> slotSettingTypes = { 
	"None", /* Added */
	"Any", /* Added - Equivelant to NONE StackCategory */
	"Blocks", /* Added */
	"Sword", /* Added */
	"Bow", /* Added */
	"Pickaxe", /* Added */
	"Axe", /* Added */
	"Shovel", /* Added */
	"Hoe", /* Added */
	"Shears", /* Added */
	"Flint & Steel", /* Added */
	"Food", /* Added */
	"Pearls" /* Added */
};

StackCategory slotSettingTypeToStackCategory(int index)
{
	switch (index) {
	case 0:
	case 1:
		return StackCategory::NONE;
	case 2:
		return StackCategory::BLOCKS;
	case 3:
		return StackCategory::SWORD;
	case 4:
		return StackCategory::BOW;
	case 5:
		return StackCategory::PICKAXE;
	case 6:
		return StackCategory::AXE;
	case 7:
		return StackCategory::SHOVEL;
	case 8:
		return StackCategory::HOE;
	case 9:
		return StackCategory::SHEARS;
	case 10:
		return StackCategory::FIRE;
	case 11:
		return StackCategory::FOOD;
	case 12:
		return StackCategory::PEARLS;
	default:
		// Throw an exception or return a default value if the index is out of range
		__debugbreak();
		//throw std::out_of_range("Invalid index for slotSettingTypes: " + std::to_string(index));
	}
	return StackCategory::NONE;
}

int stackCategoryToSlotSettingType(StackCategory category)
{
	switch (category) {
	case StackCategory::NONE:
		return 0;
	case StackCategory::BLOCKS:
		return 2;
	case StackCategory::SWORD:
		return 3;
	case StackCategory::BOW:
		return 4;
	case StackCategory::PICKAXE:
		return 5;
	case StackCategory::AXE:
		return 6;
	case StackCategory::SHOVEL:
		return 7;
	case StackCategory::HOE:
		return 8;
	case StackCategory::SHEARS:
		return 9;
	case StackCategory::FIRE:
		return 10;
	case StackCategory::FOOD:
		return 11;
	case StackCategory::PEARLS:
		return 12;
	default:
		// Throw an exception or return a default value if the enum value is not recognized
		__debugbreak();
		//throw std::invalid_argument("Invalid StackCategory value");
	}
	return -1;
}

enum class cleanModes_t { LEGIT, TRANSACTIONS };
static std::vector<std::string> modes = { "Legit", "Transactions" };
InventoryCleaner::InventoryCleaner() : Module("InventoryCleaner", "Automatically drops unwanted items from your inventory.")
{
	this->mode = this->addItem<Setting>("Mode", "The method used for moving inventory items. Legit required your inventory to be open!", SettingType::ENUM, 0, 0, modes.size() - 1, modes);
	this->delay = this->addItem<Setting>("Delay", "The amount of ticks between each automatic inventory action.", SettingType::SLIDER_INT, 10, 0, 100);
	this->storeExtra = this->addItem<Setting>("Store Extra", "If extra wanted items should be kept.", SettingType::TOGGLE, false, false, true);

	for (int i = 0; i < slotSettings.size(); i++)
	{
		slotSettings[i] = this->addItem<Setting>("Slot " + std::to_string(i), "Determines what item should be in the slot.", SettingType::ENUM, 0, 0, slotSettingTypes.size(), slotSettingTypes);
	}
}

static int ticksEnabled = 0;

void InventoryCleaner::onEnable()
{
	ticksEnabled = 0;
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ContainerScreenControllerTickEvent, &InventoryCleaner::onContainerScreenControllerTickEvent>(this);
	dispatcher->listen<LevelTickEvent, &InventoryCleaner::onLevelTickEvent>(this);
}

void InventoryCleaner::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ContainerScreenControllerTickEvent, &InventoryCleaner::onContainerScreenControllerTickEvent>(this);
	dispatcher->deafen<LevelTickEvent, &InventoryCleaner::onLevelTickEvent>(this);
}

void InventoryCleaner::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event)
{
	std::shared_ptr<AutoArmor> aa = systems->getModuleManager()->findModule<AutoArmor>();
	if (aa != nullptr)
	{
		if (aa->isRunning())
		{
			return;
		}
	}
	auto cleanMode = static_cast<cleanModes_t>(this->mode->getValueAs<int>());
	if (cleanMode != cleanModes_t::LEGIT) return;

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

	/* Find the best items for the hotbar */
	for (int currentSlot = 0; currentSlot < 9; currentSlot++)
	{
		ItemStack& currentStack = player->mInventory->container->stacks[currentSlot];
		const auto [bestSlot, bestStack] = bestOfCategory(slotSettingTypeToStackCategory(slotSettings[currentSlot]->getValueAs<int>()));
		if (bestStack == nullptr)
		{
			/* There is no best item, skip */
			continue;
		}
		if (currentSlot == bestSlot)
		{
			/* The best item is already in the slot so skip */
			continue;
		}
		
		bool bestIsHotbar = true;
		int correctedBestSlot = 0;
		if (bestSlot < 9)
			correctedBestSlot = bestSlot;
		else
		{
			bestIsHotbar = false;
			correctedBestSlot = bestSlot - 9;
		}

		controller->_handleTakeAll(bestIsHotbar ? ItemSource::hotbar_items : ItemSource::inventory_items, correctedBestSlot);
		controller->_handlePlaceAll(ItemSource::hotbar_items, currentSlot);
		controller->_handlePlaceAll(bestIsHotbar ? ItemSource::hotbar_items : ItemSource::inventory_items, correctedBestSlot);
		return;
	}

	/* Move items from hotbar that shouldnt be there */
	for (int currentSlot = 0; currentSlot < 9; currentSlot++)
	{
		if (slotSettings[currentSlot]->getValueAs<int>() == 1 /* Any item */)
		{
			continue;
		}

		/* Find an empty slot */
		int emptySlot = player->mInventory->container->getEmptySlotFromHotbar();
		if (emptySlot == -1)
		{
			//If there is no free slot available, we cant clean the hotbar until the next tick
			break;
		}

		ItemStack& currentStack = player->mInventory->container->stacks[currentSlot];
		if (!currentStack.item)
			continue;
		StackCategory currentCategory = this->getCategory(&currentStack);
		StackCategory slotSetting = slotSettingTypeToStackCategory(slotSettings[currentSlot]->getValueAs<int>());

		ItemStack& emptyStack = player->mInventory->container->stacks[emptySlot];
		if (slotSettings[currentSlot]->getValueAs<int>() == 0 /* No item */
			|| currentCategory != slotSetting /* The item is not the right type for the slot */)
		{
			controller->_handleAutoPlace(ItemSource::hotbar_items, currentSlot);
			return;
		}
	}

	if (!storeExtra->getValueAs<bool>())
	{
		for (int currentSlot = 9; currentSlot < player->mInventory->container->stacks.size(); currentSlot++)
		{
			ItemStack& currentStack = player->mInventory->container->stacks[currentSlot];
			if (!currentStack.item)
				continue;
			int correctedSlot = currentSlot - 9;
			controller->_handleDropItem(ItemSource::inventory_items, correctedSlot);
			return;
		}
	}
};

void InventoryCleaner::onLevelTickEvent(LevelTickEvent& event)
{
	std::shared_ptr<AutoArmor> aa = systems->getModuleManager()->findModule<AutoArmor>();
	if (aa != nullptr)
	{
		if (aa->isRunning())
		{
			return;
		}
	}
	auto cleanMode = static_cast<cleanModes_t>(this->mode->getValueAs<int>());
	if (cleanMode != cleanModes_t::TRANSACTIONS) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	ticksEnabled++;
	int idelay = delay->getValueAs<int>();
	if (idelay != 0 && ticksEnabled % idelay != 0) return;

	/* Find the best items for the hotbar */
	for (int currentSlot = 0; currentSlot < 9; currentSlot++)
	{
		ItemStack& currentStack = player->mInventory->container->stacks[currentSlot];
		const auto [bestSlot,  bestStack] = bestOfCategory(slotSettingTypeToStackCategory(slotSettings[currentSlot]->getValueAs<int>()));
		if (bestStack == nullptr)
		{
			/* There is no best item, skip */
			continue;
		}
		if (currentSlot == bestSlot)
		{
			/* The best item is already in the slot so skip */
			continue;
		}

		ItemStack worldNull;
		worldNull.setNull();

		/* Pick up item */
		InventoryAction firstAction(
			InventorySource(
				ContainerID::Inventory
			),
			currentSlot,
			currentStack,
			*bestStack);

		InventoryAction secondAction(
			InventorySource(
				ContainerID::Inventory
			),
			bestSlot,
			*bestStack,
			currentStack);

		player->getTransactionManager()->addAction(firstAction, false);
		player->getTransactionManager()->addAction(secondAction, false);


		/*ItemStack clone = currentStack;
		currentStack = *bestStack;
		*bestStack = clone;
		return;*/
	}

	/* Move items from hotbar that shouldnt be there */
	for (int currentSlot = 0; currentSlot < 9; currentSlot++)
	{
		if (slotSettings[currentSlot]->getValueAs<int>() == 1 /* Any item */)
		{
			continue;
		}

		/* Find an empty slot */
		int emptySlot = player->mInventory->container->getEmptySlotFromHotbar();
		if (emptySlot == -1)
		{
			//If there is no free slot available, we cant clean the hotbar until the next tick
			break;
		}

		ItemStack& currentStack = player->mInventory->container->stacks[currentSlot];
		StackCategory currentCategory = this->getCategory(&currentStack);
		StackCategory slotSetting = slotSettingTypeToStackCategory(slotSettings[currentSlot]->getValueAs<int>());

		ItemStack& emptyStack = player->mInventory->container->stacks[emptySlot];
		if (slotSettings[currentSlot]->getValueAs<int>() == 0 /* No item */
			|| currentCategory != slotSetting /* The item is not the right type for the slot */)
		{
			//Swap the items
			InventoryAction firstAction(
				InventorySource(
					ContainerID::Inventory
				),
				currentSlot,
				currentStack,
				emptyStack);

			InventoryAction secondAction(
				InventorySource(
					ContainerID::Inventory
				),
				emptySlot,
				emptyStack,
				currentStack);

			player->getTransactionManager()->addAction(firstAction, false);
			player->getTransactionManager()->addAction(secondAction, false);

			ItemStack clone = currentStack;
			currentStack = emptyStack;
			emptyStack = clone;
		}
	}

	if (!storeExtra->getValueAs<bool>())
	{
		for (int currentSlot = 9; currentSlot < player->mInventory->container->stacks.size(); currentSlot++)
		{
			ItemStack* stack = &player->mInventory->container->stacks[currentSlot];
			const Item* item = stack->item.get();
			if (item == nullptr)
			{
				continue;
			}

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
				currentSlot,
				*stack,
				worldNull);

			player->getTransactionManager()->addAction(secondAction, false);
			player->getTransactionManager()->addAction(firstAction, false);

			*stack = worldNull;
			return;
		}
	}
}

std::pair<int, const ItemStack*> InventoryCleaner::bestOfCategory(StackCategory category)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return { 0, nullptr };
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return { 0, nullptr };

	switch (category)
	{
	case StackCategory::BLOCKS:
		return this->bestBlocks();
	case StackCategory::SWORD:
		return this->bestSword();
	case StackCategory::BOW:
		return this->bestBow();
	case StackCategory::PICKAXE:
		return this->bestPickaxe();
	case StackCategory::AXE:
		return this->bestAxe();
	case StackCategory::SHOVEL:
		return this->bestShovel();
	case StackCategory::HOE:
		return this->bestHoe();
	case StackCategory::SHEARS:
		return this->bestShears();
	case StackCategory::FIRE:
		return this->bestFire();
	case StackCategory::FOOD:
		return this->bestFood();
	case StackCategory::PEARLS:
		return this->bestPearls();
	case StackCategory::NONE:
	default:
		return { 0, nullptr };
	}
}

std::pair<int, const ItemStack*> InventoryCleaner::bestBlocks()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::BLOCKS);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (best.second->nbt != nullptr && stack->nbt == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (best.second->stackCount < stack->stackCount)
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestSword()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::SWORD);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateSword = [](const ItemStack* stack) -> float {
			float score = 0.0f;
			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += stack->item.get()->getAttackDamage() * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::BANE_OF_ARTHROPODS, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::FIRE_ASPECT, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::KNOCKBACK, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::LOOTING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::SHARPNESS, *stack) * 2.5f;
			score += EnchantUtils::getEnchantLevel(EnchantType::SMITE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);
			return score;
		};

		if (rateSword(best.second) < rateSword(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestBow()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::BOW);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateBow = [](const ItemStack* stack) -> float {
			float score = 0.0f;
			if (stack->item.get()->key.get() == "crossbow")
				score += 1.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::FLAME, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::INFINITY, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::POWER, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::PUNCH, *stack) * 1.2f;
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);
			return score;
		};

		if (rateBow(best.second) < rateBow(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestPickaxe()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::PICKAXE);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto ratePickaxe = [](const ItemStack* stack) -> float {
			float score = 0.0f;
			
			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::SILK_TOUCH, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::FORTUNE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);

			return score;
		};

		if (ratePickaxe(best.second) < ratePickaxe(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestAxe()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::AXE);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateAxe = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::BANE_OF_ARTHROPODS, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::FORTUNE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::SHARPNESS, *stack) * 2.5f;
			score += EnchantUtils::getEnchantLevel(EnchantType::SILK_TOUCH, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::SMITE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::LOOTING, *stack);

			return score;
		};

		if (rateAxe(best.second) < rateAxe(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestShovel()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::SHOVEL);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateShovel = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::FORTUNE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::SILK_TOUCH, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);

			return score;
		};

		if (rateShovel(best.second) < rateShovel(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestHoe()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::HOE);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateHoe = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::FORTUNE, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::SILK_TOUCH, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);

			return score;
		};

		if (rateHoe(best.second) < rateHoe(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestShears()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::SHEARS);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateShears = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);

			return score;
		};

		if (rateShears(best.second) < rateShears(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestFire()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::FIRE);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateFire = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += (stack->item.get()->maxDurability - stack->uses) / 100.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::EFFICIENCY, *stack) * 2.0f;
			score += EnchantUtils::getEnchantLevel(EnchantType::MENDING, *stack);
			score += EnchantUtils::getEnchantLevel(EnchantType::UNBREAKING, *stack);

			return score;
		};

		if (rateFire(best.second) < rateFire(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestFood()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::FOOD);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (!best.second->item && stack->item)
		{
			best = { slotId, stack };
			continue;
		}

		auto rateFood = [](const ItemStack* stack) -> float {
			float score = 0.0f;

			score += stack->stackCount * 0.05f;
			const IFoodItemComponent* foodComponent = stack->item.get()->getFood();
			score += foodComponent->hungerPoints;
			score += foodComponent->saturationModifier;
			for (const FoodEffect& effect : foodComponent->effects)
			{
				if(effect.isPositive())
					score += effect.chance + effect.amplifier + (effect.duration / 200.0f);
			}

			return score;
		};

		if (rateFood(best.second) < rateFood(stack))
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::pair<int, const ItemStack*> InventoryCleaner::bestPearls()
{
	std::vector<std::pair<int, const ItemStack*>> stacks = allOfCategory(StackCategory::PEARLS);
	std::pair<int, const ItemStack*> best;
	for (const auto [slotId, stack] : stacks)
	{
		if (best.second == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (best.second->nbt != nullptr && stack->nbt == nullptr)
		{
			best = { slotId, stack };
			continue;
		}

		if (best.second->stackCount < stack->stackCount)
		{
			best = { slotId, stack };
			continue;
		}
	}
	return best;
}

std::vector<std::pair<int, const ItemStack*>> InventoryCleaner::allOfCategory(StackCategory category)
{
	ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return {};
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return {};

	std::vector<std::pair<int, const ItemStack*>> stacks;
	int slotId = 0;
	for (const ItemStack& stack : player->mInventory->container->stacks)
	{
		StackCategory cat = this->getCategory(&stack);
		if(cat == category)
			stacks.push_back({ slotId, &stack });
		slotId++;
	}
	return stacks;
}

StackCategory InventoryCleaner::getCategory(const ItemStack* stack)
{
	/* If the stack has no item, it must be none */
	if (!stack->item)
	{
		return StackCategory::NONE;
	}

	/* If the stack has a block pointer set, it must be a block */
	if (stack->block)
	{
		//However if it is tnt, it should be considered a utility item
		if (stack->block->blockLegacy->itemKey.get() == "tnt")
		{
			return StackCategory::FIRE;
		}

		return StackCategory::BLOCKS;
	}

	if (stack->item)
	{
		const Item* item = stack->item.get();
		if (item->hasRecipeTag("minecraft:is_sword"))
			return StackCategory::SWORD;
		if (item->key.get() == "bow" || item->key.get() == "crossbow")
			return StackCategory::BOW;
		if (item->hasRecipeTag("minecraft:is_pickaxe"))
			return StackCategory::PICKAXE;
		if (item->hasRecipeTag("minecraft:is_axe"))
			return StackCategory::AXE;
		if (item->hasRecipeTag("minecraft:is_shovel"))
			return StackCategory::SHOVEL;
		if (item->hasRecipeTag("minecraft:is_hoe"))
			return StackCategory::HOE;
		if (item->key.get() == "shears")
			return StackCategory::SHEARS;
		if (item->key.get() == "flint_and_steel" || item->key.get() == "fire_charge")
			return StackCategory::FIRE;
		if (item->hasRecipeTag("minecraft:is_food"))
			return StackCategory::FOOD;
		if (item->key.get() == "ender_pearl")
			return StackCategory::PEARLS;
	}

    return StackCategory::NONE;
}