#pragma once

#include "EntityId.h"

#undef max
#include <entt/entity/registry.hpp>

#include <stdint.h>

struct EntityRefTraits {
	entt::basic_registry<EntityId>* registry;
	uint64_t another;
	uint64_t theresMore;
};