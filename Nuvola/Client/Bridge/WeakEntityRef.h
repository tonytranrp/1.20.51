#pragma once

#include "EntityId.h"

#include <memory>

#pragma pack(push, 1)
class WeakEntityRef
{
public:
	std::shared_ptr<entt::registry> registryPtr;
	EntityId entityId; //0x0010
}; //Size: 0x0014
#pragma pack(pop)
static_assert(sizeof(WeakEntityRef) == 0x14);
