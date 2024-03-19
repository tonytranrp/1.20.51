#pragma once

#include "../../../Bridge/ItemStack.h"

#include "../Module.h"

#include <array>
#include <memory>

enum class StackCategory
{
	NONE,
	BLOCKS,
	SWORD,
	BOW,
	PICKAXE,
	AXE,
	SHOVEL,
	HOE,
	SHEARS,
	FIRE, /* Flint & Steel / Firecharge / etc */
	FOOD,
	PEARLS
};

class ContainerScreenControllerTickEvent;
class LevelTickEvent;
class InventoryCleaner : public Module
{
	std::shared_ptr<Setting> mode;
	std::shared_ptr<Setting> delay;
	std::shared_ptr<Setting> storeExtra;
	std::array<std::shared_ptr<Setting>, 9> slotSettings;
public:
	InventoryCleaner();

	void onEnable() override;
	void onDisable() override;

	void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
	
	std::pair<int, const ItemStack*> bestOfCategory(StackCategory category);
	std::pair<int, const ItemStack*> bestBlocks();
	std::pair<int, const ItemStack*> bestSword();
	std::pair<int, const ItemStack*> bestBow();
	std::pair<int, const ItemStack*> bestPickaxe();
	std::pair<int, const ItemStack*> bestAxe();
	std::pair<int, const ItemStack*> bestShovel();
	std::pair<int, const ItemStack*> bestHoe();
	std::pair<int, const ItemStack*> bestShears();
	std::pair<int, const ItemStack*> bestFire();
	std::pair<int, const ItemStack*> bestFood();
	std::pair<int, const ItemStack*> bestPearls();
	std::vector<std::pair<int, const ItemStack*>> allOfCategory(StackCategory category);
	StackCategory getCategory(const ItemStack* stack);
};