#pragma once

#include "../EntityId.h"

class UpdatePlayerFromCameraComponent
{
public:
	EntityId entt;

	UpdatePlayerFromCameraComponent() : entt() {}
	explicit UpdatePlayerFromCameraComponent(EntityId entt) : entt(entt) {}
};