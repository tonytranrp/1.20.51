#pragma once

#include "ItemDescriptorCount.h"
#include "ItemStack.h"

#include "../Signatures/SigManager.h"

class NetworkItemStackDescriptor : public ItemDescriptorCount
{
public:
	char pad_0018[72]; //0x0018

	NetworkItemStackDescriptor() {};
	NetworkItemStackDescriptor(const ItemStack& stack)
	{
		memset(this, 0, sizeof(NetworkItemStackDescriptor));
		auto func = systems->getSigManager()->findAddressAs<void (__thiscall *)(NetworkItemStackDescriptor *, const ItemStack *)>(
                SigTable::NetworkItemStackDescriptor_cctor);
		func(this, &stack);
	}
}; //Size: 0x0060
static_assert(sizeof(NetworkItemStackDescriptor) == 0x60);
