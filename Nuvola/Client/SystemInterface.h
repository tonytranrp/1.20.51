#pragma once

#include <entt/core/type_info.hpp>

class SystemInterface
{
public:
    virtual void installAllSystems() = 0;
    [[nodiscard]] virtual bool finishedInstall(entt::id_type systemType) const = 0;
};