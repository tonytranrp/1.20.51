#include "StackResultStorageEntitySigProvider.h"

#include <xorstr.hpp>

StackResultStorageEntitySigProvider::StackResultStorageEntitySigProvider() : SigProvider("StackResultStorageEntity")
{
	this->addResult(SigTable::StackResultStorageEntity_cctor, std::string("40 53 48 83 EC 20 C6 41 10 00 48"));
}
