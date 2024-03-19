#include "SystemManager.h"

#include "Events/EventDispatcher.h"
#include "Features/Commands/CommandManager.h"
#include "Features/Config/ConfigManager.h"
#include "Features/IRC/ChatClient.h"
#include "Features/Keymap/KeymapManager.h"
#include "Features/Modules/ModuleManager.h"
#include "Features/Theme/ThemeManager.h"
#include "GUI/ScreenManager.h"
#include "Offsets/OffsetManager.h"
#include "Patches/PatchManager.h"
#include "Preferences/Preferences.h"
#include "Signatures/SigManager.h"

//#define USE_DEV_IRC

SystemManager::SystemManager() : Manager("SystemManager")
{

}

void SystemManager::installAllSystems() {
    this->chatClient = this->install<ChatClient>(
#if defined(_DEBUG) and defined(USE_DEV_IRC)
        "wss://wsc.nuvola.wtf/dev"
#else
        "wss://wsc.nuvola.wtf/pub"
#endif
    );
    this->commandManager = this->install<CommandManager>();
    this->configManager = this->install<ConfigManager>();
    this->eventDispatcher = this->install<EventDispatcher>();
    this->keymapManager = this->install<KeymapManager>();
    this->moduleManager = this->install<ModuleManager>();
    this->offsetManager = this->install<OffsetManager>();
    this->patchManager = this->install<PatchManager>();
    this->preferences = this->install<Preferences>();
    this->screenManager = this->install<ScreenManager>();
    this->sigManager = this->install<SigManager>();
    this->themeManager = this->install<ThemeManager>();

    this->installRemaining();
}

std::shared_ptr<ChatClient> SystemManager::getChatClient() const {
    if(!this->finishedInstall<ChatClient>())
    {
        return this->get<ChatClient>();
    }
    return this->chatClient;
}

std::shared_ptr<CommandManager> SystemManager::getCommandManager() const {
    //It is possible the variable isnt set yet
    if(!this->finishedInstall<CommandManager>())
    {
        return this->get<CommandManager>();
    }
    return this->commandManager;
}

std::shared_ptr<ConfigManager> SystemManager::getConfigManager() const
{
    if(!this->finishedInstall<ConfigManager>())
    {
        return this->get<ConfigManager>();
    }
    return this->configManager;
}

std::shared_ptr<EventDispatcher> SystemManager::getEventDispatcher() const
{
    if (!this->finishedInstall<EventDispatcher>())
    {
        return this->get<EventDispatcher>();
    }
    return this->eventDispatcher;
}

std::shared_ptr<KeymapManager> SystemManager::getKeymapManager() const {
    if(!this->finishedInstall<KeymapManager>())
    {
        return this->get<KeymapManager>();
    }
    return this->keymapManager;
}

std::shared_ptr<ModuleManager> SystemManager::getModuleManager() const
{
    if(!this->finishedInstall<ModuleManager>())
    {
        return this->get<ModuleManager>();
    }
    return this->moduleManager;
}

std::shared_ptr<OffsetManager> SystemManager::getOffsetManager() const
{
    if(!this->finishedInstall<OffsetManager>())
    {
        return this->get<OffsetManager>();
    }
	return this->offsetManager;
}

std::shared_ptr<PatchManager> SystemManager::getPatchManager() const
{
    if(!this->finishedInstall<PatchManager>())
    {
        return this->get<PatchManager>();
    }
	return this->patchManager;
}

std::shared_ptr<Preferences> SystemManager::getPreferences() const {
    if(!this->finishedInstall<Preferences>())
    {
        return this->get<Preferences>();
    }
    return this->preferences;
}

std::shared_ptr<ScreenManager> SystemManager::getScreenManager() const {
    if(!this->finishedInstall<ScreenManager>())
    {
        return this->get<ScreenManager>();
    }
    return this->screenManager;
}

std::shared_ptr<SigManager> SystemManager::getSigManager() const
{
    if(!this->finishedInstall<SigManager>())
    {
        return this->get<SigManager>();
    }
    return this->sigManager;
}

std::shared_ptr<ThemeManager> SystemManager::getThemeManager() const
{
    if(!this->finishedInstall<ThemeManager>())
    {
        return this->get<ThemeManager>();
    }
    return this->themeManager;
}

bool SystemManager::allInstalled() const {
    return this->installQueue.empty();
}

void SystemManager::installRemaining()
{
    const std::shared_ptr<SystemInterface> sharedThis(this, [](SystemInterface*){});

    bool installedOne = false;
    do {
        installedOne = false;
        for(auto it = this->installQueue.begin(); it != this->installQueue.end();)
        {
            auto& queuedSystem = *it;
            if(queuedSystem->readyToInstall(sharedThis))
            {
                queuedSystem->install(sharedThis);
                this->installQueue.erase(it++);
                installedOne = true;
            }
            else
            {
                ++it;
            }
        }
    }
    while(installedOne);
}

std::shared_ptr<SystemManager> systems;
