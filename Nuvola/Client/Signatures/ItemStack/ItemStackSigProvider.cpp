#include "ItemStackSigProvider.h"

#include <xorstr.hpp>

ItemStackSigProvider::ItemStackSigProvider() : SigProvider("ItemStack")
{
	this->addResult(SigTable::ItemStackBaseOperatorEquals, "48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B EA 48 8B F1 C7 44 24");
	this->addResult(SigTable::ItemStackBase_cctor, std::string("48 89 4C 24 ? 53 55 56 57 41 56 41 57 48 83 EC ? 48 8B EA 48 8B F1"));
}
