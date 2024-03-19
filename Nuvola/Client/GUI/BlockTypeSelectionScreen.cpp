#include "BlockTypeSelectionScreen.h"

#include "../Bridge/ClientInstance.h"
#include "../Bridge/Level.h"
#include "../Bridge/LocalPlayer.h"

BlockTypeSelectionScreenData::BlockTypeSelectionScreenData(std::shared_ptr<Setting> settingPtr)
{
	std::shared_ptr<BlockTypeListSetting> castedSetting = std::dynamic_pointer_cast<BlockTypeListSetting>(settingPtr);
	assert(castedSetting != nullptr);
	this->settingPtr = castedSetting;
	this->initializer = [&](GroupedType<BlockTypeGroup, std::string>* groups) -> void {
		ClientInstance* ci = Bridge::getClientInstance();
		LocalPlayer* lp = ci->getClientPlayer();
		if (lp == nullptr) return;
		Level* level = lp->level;
		if (level == nullptr) return;

		auto& palette = level->mBlockPalette;
		for(const auto* block : palette->blocks)
		{
			const auto& key = block->blockLegacy->itemId.get();
			BlockTypeGroup group = this->getGroupFor(key);
			groups->registerType(key, group);
		}
	};
}

BlockTypeGroup BlockTypeSelectionScreenData::getGroupFor(const std::string& canonicalName)
{
	std::string _namespace = "unknown";
	std::size_t pos = canonicalName.find(':');
	if (pos != std::string::npos) {
		_namespace = canonicalName.substr(0, pos);
	}
	if (_namespace != "minecraft")
	{
		return BlockTypeGroup::Custom;
	}

	ClientInstance* ci = Bridge::getClientInstance();
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return BlockTypeGroup::Misc;
	Level* level = lp->level;
	if (level == nullptr) return BlockTypeGroup::Misc;
	auto& palette = level->mBlockPalette;
	if (palette == nullptr) return BlockTypeGroup::Misc;

	const Block* block = palette->getBlock(canonicalName);

	std::string itemGroup = block->blockLegacy->itemGroup;
	if(itemGroup.empty()) return BlockTypeGroup::Misc;
	// Remove the initial "itemGroup.name." prefix
	std::string groupName = itemGroup.substr(15);

	auto enumVal = magic_enum::enum_cast<BlockTypeGroup>(groupName, magic_enum::case_insensitive);
	if(enumVal.has_value())
	{
		return *enumVal;
	}

	return BlockTypeGroup::Misc;
}

BlockTypeSelectionScreen::BlockTypeSelectionScreen() : TypeSelectionScreen<BlockTypeGroup, std::string>("BlockTypeSelectionScreen")
{
}

void BlockTypeSelectionScreen::initialize(ScreenData* data)
{
	auto* castedData = dynamic_cast<BlockTypeSelectionScreenData*>(data);
	assert(castedData != nullptr);

	this->settingPtr = castedData->settingPtr;

	TypeSelectionScreen<BlockTypeGroup, std::string>::initialize(data);
}

void BlockTypeSelectionScreen::render(float deltaTime)
{
	TypeSelectionScreen<BlockTypeGroup, std::string>::render(deltaTime);
}

void BlockTypeSelectionScreen::cleanup() {
	WindowScreen::cleanup();

	ClientInstance* ci = Bridge::getClientInstance();
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	Level* level = lp->level;
	if (level == nullptr) return;
	auto& palette = level->mBlockPalette;

	this->settingPtr->setSelectedTypes(this->selected);
}
