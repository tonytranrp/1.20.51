#pragma once

#include "SystemInterface.h"

#include <memory>

class InstallableSystem
{
public:
    InstallableSystem() = default;
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) = 0;
    [[nodiscard]] virtual bool readyToInstall(const std::shared_ptr<SystemInterface>& ownerPtr) const = 0;
};