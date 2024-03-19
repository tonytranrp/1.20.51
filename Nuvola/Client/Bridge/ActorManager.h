#pragma once

#include "EntityContext.h"

#include <optional>
#include <vector>

class ActorManager
{
public:
	char pad_0000[24]; //0x0000
	std::vector<std::optional<EntityContext>> entities; //0x0018
}; //Size: 0x0048