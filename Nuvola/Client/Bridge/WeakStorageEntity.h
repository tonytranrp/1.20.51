#pragma once

#include "EntityRegistry.h"

#include <memory>
#include <optional>

struct WeakStorageEntity {
    std::weak_ptr<EntityRegistry> registry;
    EntityId entity;
};