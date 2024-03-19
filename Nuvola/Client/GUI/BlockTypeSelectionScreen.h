#pragma once

#include "../Bridge/NamespacedKey.h"

#include "../Features/Modules/BlockTypeListSetting.h"

#include "TypeSelectionScreen.h"

#include <string>

enum class BlockTypeGroup
{
	Anvil,
	Banner,
	Bed,
	Chest,
	Chest_Boat,
	Concrete,
	Concrete_Powder,
	Coral,
	Coral_Decorations,
	Crop,
	Door,
	Fence,
	Fence_Gate,
	Glass,
	Glass_Pane,
	Glazed_Terracotta,
	Grass,
	Hanging_Sign,
	Leaves,
	Log,
	Nether_Wart_Block,
	Ore,
	Planks,
	Rail,
	Sandstone,
	Sapling,
	Sculk,
	Shulker_Box,
	Sign,
	Slab,
	Stained_Clay,
	Stairs,
	Stone,
	Stone_Brick,
	Trapdoor,
	Walls,
	Wood,
	Wool,
	Wool_Carpet,
	Misc,
	Custom
};

struct BlockTypeSelectionScreenData : public TypeSelectionScreenData<BlockTypeGroup, std::string>
{
	std::shared_ptr<BlockTypeListSetting> settingPtr = nullptr;
public:
	BlockTypeSelectionScreenData(std::shared_ptr<Setting> settingPtr);

	BlockTypeGroup getGroupFor(const std::string& canonicalName);
};

class BlockTypeSelectionScreen : public TypeSelectionScreen<BlockTypeGroup, std::string>
{
	std::shared_ptr<BlockTypeListSetting> settingPtr = nullptr;
public:
	BlockTypeSelectionScreen();

	virtual void initialize(ScreenData* data) override;
	virtual void render(float deltaTime) override;
	virtual void cleanup() override;
};