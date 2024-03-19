#pragma once

#include "Category.h"
#include "Module.h"

#include "../../System.h"

#include <functional>
#include <vector>

class KeyEvent;
class ModuleManager final : public ::System<Manager<Category>> {
    using system_trait = Manager<Category>;
public:
    ModuleManager();

    void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

    std::vector<std::shared_ptr<Module>> getAllModules(std::function<bool(std::shared_ptr<Module>, std::shared_ptr<Module>)> sort);
    std::vector<std::shared_ptr<Module>> getAllModules();
    std::shared_ptr<Module> findModule(std::string name);

    template<typename module_t>
    std::shared_ptr<module_t> findModule()
    {
        static_assert(std::is_base_of_v<Module, module_t>, "module_t must derive from Module");
        for (const auto& mod : this->getAllModules())
        {
            std::shared_ptr<module_t> casted = std::dynamic_pointer_cast<module_t>(mod);
            if (casted != nullptr)
            {
                return casted;
            }
        }
        return nullptr;
    }

	void onKeyEvent(KeyEvent& event);
};
