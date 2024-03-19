#pragma once

#include "Components/AbilitiesComponent.h"

#include "ItemStackNetManagerClient.h"
#include "InventoryTransactionManager.h"
#include "IContainerManager.h"
#include "Mob.h"
#include "PlayerInventory.h"

#include <string>
#include <memory>

#pragma pack(push, 1)
struct Player : public Mob {
	DEFINE_MEMBER(std::weak_ptr<IContainerManager>, mContainerManager, OffsetTable::Player_mIContainerManager);
	DEFINE_MEMBER(PlayerInventory*, mInventory, OffsetTable::Player_mPlayerInventory);
	DEFINE_MEMBER(InventoryTransactionManager*, mTransactionManager, OffsetTable::Player_mInventoryTransactionManager);
	DEFINE_MEMBER(class GameMode*, mGameMode, OffsetTable::Player_mGameMode);
	DEFINE_MEMBER(ItemStackNetManagerClient*, mItemStackNetManager, OffsetTable::Player_mItemStackNetManagerClient);
	DEFINE_MEMBER(std::string, mPlayerName, OffsetTable::Player_mName);

	/* Virtuals */
	void startDestroying()
	{
		return vftCall<347, void, Actor*>(this);
	}

	/* Functions */
	InventoryTransactionManager* getTransactionManager()
	{
		return (InventoryTransactionManager*)&this->mTransactionManager;
	}
	ItemStackNetManagerClient* getStackNetManager()
	{
		return this->mItemStackNetManager;
	}

	[[nodiscard]] bool isFlying() {
		if (auto pComponent = this->entityContext.tryGetComponent<AbilitiesComponent>()) {
			const auto& abilities = pComponent->abilities;
			return abilities.getAbility(AbilitiesLayer::PLAYER, AbilitiesIndex::Flying).getBool();
		}
		return false;
	}
	void setFlying(bool value) {
		if (auto pComponent = this->entityContext.tryGetComponent<AbilitiesComponent>()) {
			pComponent->abilities.setAbility(AbilitiesLayer::PLAYER, AbilitiesIndex::Flying, value);
		}
	}
	void setFlightSpeed(float value) {
		if (auto pComponent = this->entityContext.tryGetComponent<AbilitiesComponent>()) {
			pComponent->abilities.setAbility(AbilitiesLayer::PLAYER, AbilitiesIndex::FlySpeed, value);
		}
	}
};
#pragma pack(pop)
