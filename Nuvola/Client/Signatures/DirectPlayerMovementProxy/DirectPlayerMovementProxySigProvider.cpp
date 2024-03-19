#include "DirectPlayerMovementProxySigProvider.h"

#include <xorstr.hpp>

DirectPlayerMovementProxySigProvider::DirectPlayerMovementProxySigProvider() : SigProvider("DirectPlayerMovementProxy")
{
	this->addResult(SigTable::DirectPlayerMovementProxy_isSlowedByItemUse, std::string("48 83 ec ? 48 8b 09 48 85 c9 74 ? 48 8b 09"));
}
