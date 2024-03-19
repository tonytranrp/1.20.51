#include "ScreenManager.h"

#include "BlockTypeSelectionScreen.h"
#include "ClickGuiScreen.h"
#include "EnchantWeightScreen.h"
#include "EntityTypeSelectionScreen.h"

#include "../Patches/PatchManager.h"

ScreenManager::ScreenManager() : System<Manager<Screen>>("ScreenManager")
{
    this->depends<PatchManager>();
}

std::shared_ptr<Screen> ScreenManager::getActiveScreen()
{
	if (this->screenStack.size() == 0)
		return nullptr;

	return this->screenStack.back();
}

void ScreenManager::pushScreen(std::shared_ptr<Screen> screen, ScreenData* data)
{
	this->screenStack.push_back(screen);
	this->getActiveScreen()->initialize(data);
}

bool ScreenManager::pushScreen(std::string_view screenName, ScreenData* data)
{
	for (auto& screen : this->getItems())
	{
		if (screen->getName() == screenName)
		{
			this->pushScreen(screen, data);
			return true;
		}
	}
	return false;
}

void ScreenManager::popScreen()
{
	assert(!this->screenStack.empty()); //ScreenManager::popScreen called with an empty screen stack!

	this->getActiveScreen()->cleanup();
	this->screenStack.pop_back();
}

void ScreenManager::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
    System::install(ownerPtr);

	this->registerScreen<BlockTypeSelectionScreen>();
    this->registerScreen<ClickGuiScreen>();
    this->registerScreen<EnchantWeightScreen>();
    this->registerScreen<EntityTypeSelectionScreen>();
}
