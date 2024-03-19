#pragma once

#include "InstallableSystem.h"
#include "SystemInterface.h"

#include <entt/core/type_info.hpp>

#include <algorithm>
#include <cassert>
#include <ranges>
#include <set>

template<typename data_t>
class System : public data_t, public InstallableSystem
{
/*public:
    typedef data_t system_type;*/
private:
    std::set<entt::id_type> dependencies;
public:
    template<typename... args_t>
    explicit System(args_t ...args) : data_t(args...), InstallableSystem() {}

    /// Function for the SystemManager to call when installing the system
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override
    {
        //If this fails, not all of the system's dependencies are installed and this system is to be installed prematurely
        assert(this->readyToInstall(ownerPtr));
    }

    /// A function to indicate that the system depends on another system
    /// \tparam system_t
    template<typename system_t>
    void depends()
    {
        dependencies.insert(entt::type_hash<system_t>::value());
    }

    /// A function to indicate that the system is now ready to be installed
    [[nodiscard]] virtual bool readyToInstall(const std::shared_ptr<SystemInterface>& ownerPtr) const override
    {
        return std::ranges::all_of(this->dependencies, [&](auto dep) {
            if(!ownerPtr->finishedInstall(dep))
                return false;
            return true;
        });
    }
};