#pragma once
#include <entt/entt.hpp>
#include "EntityId.h"

class EntityRegistryBase
{
public:
	entt::basic_registry<EntityId>* mEnttRegistry = nullptr;
	EntityRegistryBase* mRegistry = nullptr;
};

class EntityRegistry : public EntityRegistryBase {
public:
	std::string mName;
	entt::basic_registry<EntityId> mOwnedRegistry;
};