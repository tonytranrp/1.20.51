#pragma once

#include "../LayeredAbilities.h"

#include <array>

struct AbilitiesComponent
{
public:
	LayeredAbilities abilities;
}; //Size: 0x0478
static_assert(sizeof(AbilitiesComponent) == 0x478);