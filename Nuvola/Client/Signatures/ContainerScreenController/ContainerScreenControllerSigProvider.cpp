#include "ContainerScreenControllerSigProvider.h"

#include <xorstr.hpp>

ContainerScreenControllerSigProvider::ContainerScreenControllerSigProvider() : SigProvider("ContainerScreenController")
{
	this->addResult(SigTable::ContainerScreenController__handleAutoPlace, "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 45 8b e1 49 8b f0 44 8b fa");
	this->addResult(SigTable::ContainerScreenController__handleDropItem, "48 89 5c 24 ? 55 56 57 41 56 41 57 48 83 ec ? 4d 8b f1 45 8b f8");
	this->addResult(SigTable::ContainerScreenController__handlePlaceAll, "48 89 5c 24 ? 48 89 6c 24 ? 56 57 41 56 48 83 ec ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 44 24 ? 41 8b e8 48 8b fa");
	this->addResult(SigTable::ContainerScreenController__handleTakeAll, "48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 48 89 7c 24 ? 41 56 48 83 ec ? 45 8b f0 48 8b fa");
	this->addResult(SigTable::ContainerScreenController_tick, "48 89 5c 24 ? 48 89 6c 24 ? 56 57 41 56 48 83 ec ? 0f 29 74 24 ? 48 8b f1");
	//ContainerScreenController::_sendFlyingItem   40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 45 8b f1 49 8b f8
	//CraftingScreenController::_sendFlyingItem    40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d 6c 24 ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 45 ? 41 8b f1 44 89 4c 24
}
