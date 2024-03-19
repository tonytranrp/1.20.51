#pragma once

#include "System.h"
#include "SystemInterface.h"
#include "SystemsInst.h"

#include <Data/Manager.h>

#include <entt/core/type_info.hpp>

#include <list>
#include <memory>
#include <set>

class ChatClient;
class CommandManager;
class ConfigManager;
class EventDispatcher;
class KeymapManager;
class ModuleManager;
class OffsetManager;
class PatchManager;
class Preferences;
class ThemeManager;
class ScreenManager;
class SigManager;

class SystemManager final : public Manager<ManagedItem>, public SystemInterface
{
    std::set<entt::id_type> installedSystems;
    std::set<std::shared_ptr<InstallableSystem>> installQueue;

	std::shared_ptr<ChatClient> chatClient = nullptr;
	std::shared_ptr<CommandManager> commandManager = nullptr;
	std::shared_ptr<ConfigManager> configManager = nullptr;
	std::shared_ptr<EventDispatcher> eventDispatcher = nullptr;
    std::shared_ptr<KeymapManager> keymapManager = nullptr;
	std::shared_ptr<ModuleManager> moduleManager = nullptr;
	std::shared_ptr<OffsetManager> offsetManager = nullptr;
	std::shared_ptr<PatchManager> patchManager = nullptr;
    std::shared_ptr<Preferences> preferences = nullptr;
    std::shared_ptr<ScreenManager> screenManager = nullptr;
    std::shared_ptr<SigManager> sigManager = nullptr;
    std::shared_ptr<ThemeManager> themeManager = nullptr;

public:
	SystemManager();
	~SystemManager() override = default;

	SystemManager(const SystemManager&) = delete;
	SystemManager(SystemManager&&) = delete;
	void operator=(const SystemManager&) = delete;
	void operator=(SystemManager&&) = delete;

    virtual void installAllSystems() override;

	template<class system_t, typename... args_t>
	std::shared_ptr<system_t> install(args_t... args)
	{
        static_assert(std::is_base_of_v<ManagedItem, system_t> == true, "system_t must be an extension of System of a ManagedItem");
		auto system = this->addItem<system_t>(args...);
        std::shared_ptr<SystemInterface> sharedThis(this, [](SystemInterface*){});
        if(system->readyToInstall(sharedThis))
        {
            system->install(sharedThis);
            this->installRemaining();
        }
        else
        {
            this->installQueue.insert(system);
        }
        this->installedSystems.insert(entt::type_hash<system_t>::value());
        return system;
	}

    void installRemaining();
    bool allInstalled() const;

	template<typename system_t>
	std::shared_ptr<system_t> get() const
	{
		for (auto item : this->getItems())
		{
			auto casted = std::dynamic_pointer_cast<system_t>(item);
			if (casted != nullptr)
			{
				return casted;
			}
		}
		return nullptr;
	}

    template<class system_t>
    [[nodiscard]] bool finishedInstall() const
    {
        return this->finishedInstall(entt::type_hash<system_t>::value());
    }

    [[nodiscard]] bool finishedInstall(entt::id_type systemType) const override
    {
        return this->installedSystems.contains(systemType);
    }

	[[nodiscard]] std::shared_ptr<ChatClient> getChatClient() const;
	[[nodiscard]] std::shared_ptr<CommandManager> getCommandManager() const;
	[[nodiscard]] std::shared_ptr<ConfigManager> getConfigManager() const;
	[[nodiscard]] std::shared_ptr<EventDispatcher> getEventDispatcher() const;
    [[nodiscard]] std::shared_ptr<KeymapManager> getKeymapManager() const;
	[[nodiscard]] std::shared_ptr<ModuleManager> getModuleManager() const;
	[[nodiscard]] std::shared_ptr<OffsetManager> getOffsetManager() const;
	[[nodiscard]] std::shared_ptr<PatchManager> getPatchManager() const;
    [[nodiscard]] std::shared_ptr<Preferences> getPreferences() const;
	[[nodiscard]] std::shared_ptr<ThemeManager> getThemeManager() const;
    [[nodiscard]] std::shared_ptr<ScreenManager> getScreenManager() const;
    [[nodiscard]] std::shared_ptr<SigManager> getSigManager() const;
};