#include "PlayerOffsetProvider.h"

PlayerOffsetProvider::PlayerOffsetProvider() : OffsetProvider("Player")
{
	this->addResult(OffsetTable::Player_mInventoryTransactionManager, 0xE80);
	this->addResult(OffsetTable::Player_mPlayerInventory, 0x7C0);
	this->addResult(OffsetTable::Player_mGameMode, 0xF10);
	this->addResult(OffsetTable::Player_mItemStackNetManagerClient, 0x1930);
	this->addResult(OffsetTable::Player_mName, 0x1CB8);
	this->addResult(OffsetTable::Player_mIContainerManager, 0x7B0);
}
