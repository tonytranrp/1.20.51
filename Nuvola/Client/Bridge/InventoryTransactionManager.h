#pragma once

#include "../Signatures/SigManager.h"

#include "InventoryAction.h"

#include <vector>

class InventoryTransactionManager
{
public:
	void addAction(const InventoryAction& action, bool param_2)
	{
		auto func = (void(__thiscall*)(InventoryTransactionManager*, const InventoryAction*, bool)) systems->getSigManager()->findAddressOf(
                SigTable::InventoryTransactionManager_addAction);
		func(this, &action, param_2);
	}
};