#include "BlockTypeListSetting.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Bridge/BlockPalette.h"
#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/Level.h"
#include "../../Bridge/LocalPlayer.h"

BlockTypeListSetting::BlockTypeListSetting(std::string name, std::string description) : Setting(name, description, SettingType::BLOCK_TYPES, nullptr, nullptr, nullptr)
{

}

const std::set<std::string>& BlockTypeListSetting::getSelectedTypes() const
{
	return this->selected;
}

void BlockTypeListSetting::setSelectedTypes(const std::set<std::string>& selected)
{
	this->selected = selected;
}

bool BlockTypeListSetting::isSelected(const std::string& blockType) const
{
	return this->selected.contains(blockType);
}

void BlockTypeListSetting::select(const std::string& BlockType)
{
	this->selected.insert(BlockType);
}

void BlockTypeListSetting::deselect(const std::string& blockType)
{
	this->selected.erase(blockType);
}

std::set<const Block *> BlockTypeListSetting::getSelectedBlocks() const
{
	std::set<const Block*> blockSet;
	auto ci = Bridge::getClientInstance();
	if(ci == nullptr) return blockSet;
	auto lp = ci->getClientPlayer();
	if(lp == nullptr) return blockSet;
	auto level = lp->level;
	if(level == nullptr) return blockSet;
	auto palette = level->mBlockPalette;
	if(palette == nullptr) return blockSet;

	for(auto& select : this->getSelectedTypes())
	{
		const auto* block = palette->getBlock(select);
		if(block != nullptr)
		{
			blockSet.insert(block);
		}
	}

	return blockSet;
}
