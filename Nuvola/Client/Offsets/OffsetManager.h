#pragma once

#include "OffsetProvider.h"

#include "../System.h"
#include "../SystemManager.h"

#include <Data/Manager.h>

#include <cstdint>

class OffsetManager : public ::System<Manager<OffsetProvider>>
{
    using system_trait = Manager<OffsetProvider>;
public:
	OffsetManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;
	int32_t findOffsetOf(OffsetTable signature);
};