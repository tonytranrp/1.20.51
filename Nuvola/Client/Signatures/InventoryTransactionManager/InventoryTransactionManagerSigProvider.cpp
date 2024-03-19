#include "InventoryTransactionManagerSigProvider.h"

InventoryTransactionManagerSigProvider::InventoryTransactionManagerSigProvider() : SigProvider("InventoryTransactionManager")
{
	this->addResult(SigTable::InventoryTransactionManager_addAction, "40 53 55 41 56 48 83 EC ? 48 8B 01 48 8B D9");
}