#include "OffsetManager.h"

#include "OffsetProvider.h"

#include "Actor/ActorOffsetProvider.h"
#include "ClientHMDState/ClientHMDStateOffsetProvider.h"
#include "ClientInstance/ClientInstanceOffsetProvider.h"
#include "Level/LevelOffsetProvider.h"
#include "LevelRendererPlayer/LevelRendererPlayerOffsetProvider.h"
#include "Mob/MobOffsetProvider.h"
#include "Options/OptionsOffsetProvider.h"
#include "Player/PlayerOffsetProvider.h"
#include "LocalPlayer/LocalPlayerOffsetProvider.h"

#include <magic_enum.hpp>

#include <memory>

OffsetManager::OffsetManager() : System<system_trait>("OffsetManager")
{

}

int32_t OffsetManager::findOffsetOf(OffsetTable offset)
{
	for (auto& provider : this->getItems()) {
		if (provider->has(offset))
		{
			return provider->findFor(offset);
		}
	}
	__debugbreak(); //The offset couldn't be found, this debugbreak is here because throwing will likely call invoke_watson cuz there may not be an exception handler.
	throw std::exception(("No offset found for " + std::string(magic_enum::enum_name(offset))).c_str());
}

void OffsetManager::install(const std::shared_ptr<SystemInterface>& ownerPtr) {
    System::install(ownerPtr);
    this->addItem<ActorOffsetProvider>();
    this->addItem<ClientHMDStateOffsetProvider>();
    this->addItem<ClientInstanceOffsetProvider>();
	this->addItem<LevelOffsetProvider>();
	this->addItem<LevelRendererPlayerOffsetProvider>();
	this->addItem<MobOffsetProvider>();
	this->addItem<OptionsOffsetProvider>();
	this->addItem<PlayerOffsetProvider>();
	this->addItem<LocalPlayerOffsetProvider>();
}

