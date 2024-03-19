//Auth
#include <Auth/User.h>

#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/MinecraftGame.h"
#include "../../Events/KeyEvent.h"
#include "../../Offsets/OffsetManager.h"
#include "../../Patches/PatchManager.h"
#include "../../Signatures/SigManager.h"

#include "ModuleManager.h"
//Combat
#include "Combat/Aimbot.h"
#include "Combat/AutoClicker.h"
#include "Combat/Killaura.h"
#include "Combat/Reach.h"
//Misc
#include "Misc/AirPlace.h"
#include "Misc/AntiBot.h"
#include "Misc/AntiCrystal.h"
#include "Misc/Blink.h"
#include "Misc/ChestStealer.h"
#include "Misc/Disabler.h"
#include "Misc/IRC.h"
#include "Misc/NoFilter.h"
#include "Misc/NoPacket.h"
#include "Misc/Regen.h"
#include "Misc/Test.h"
#include "Misc/Timer.h"
#include "Misc/Uninject.h"
//Motion
#include "Motion/Airjump.h"
#include "Motion/AntiVoid.h"
#include "Motion/BHop.h"
#include "Motion/DamageBoost.h"
#include "Motion/Fly.h"
#include "Motion/NoWeb.h"
#include "Motion/Phase.h"
#include "Motion/Spider.h"
#include "Motion/Glide.h"
#include "Motion/Step.h"
#include "Motion/Velocity.h"
#include "Motion/Clicktp.h"
#include "Motion/HighJump.h"
#include "Motion/Jesus.h"
//Player
#include "Player/AutoArmor.h"
#include "Player/AutoTotem.h"
#include "Player/AutoTool.h"
#include "Player/AutoSprint.h"
#include "Player/Friction.h"
#include "Player/Instabreak.h"
#include "Player/InventoryCleaner.h"
#include "Player/NoFall.h"
#include "Player/NoSlowdown.h"
#include "Player/Scaffold.h"
#include "Player/STap.h"
//Render
#include "Render/ClickGui.h"
#include "Render/ESP.h"
#include "Render/Freelook.h"
#include "Render/Fullbright.h"
#include "Render/ModList.h"
#include "Render/Notifications.h"
#include "Render/BlockESP.h"
#include "Render/PacketDebugger.h"
#include "Render/ServerRotations.h"
#include "Render/ThemeDebugger.h"
#include "Render/Tracers.h"
#include "Render/Xray.h"
#include "Render/CustomSky.h"
//#include "Render/TabUI.h"

//Events
#include "../../Events/EventDispatcher.h"

//Keymap
#include "../Keymap/KeymapManager.h"

#include <utility>
#include <xorstr.hpp>

static std::string outScreenName;

void ModuleManager::onKeyEvent(KeyEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr)
		return;
	MinecraftGame* mcg = ci->mMinecraftGame;
	if (mcg == nullptr)
		return;
	LocalPlayer* lp = ci->getClientPlayer();

	ci->getScreenName(outScreenName);
	if (outScreenName == "chat_screen")
	{
		return;
	}
	std::shared_ptr<ClickGui> clickgui = this->findModule<ClickGui>();
	if (clickgui->isEnabled())
	{
		return;
	}

	if (lp == nullptr || mcg->canInputMovement)
	{
		const auto mgr = systems->getModuleManager();
		for (const auto& mod : mgr->getAllModules()) {
			mod->onHotkeyCheckEvent(event);
		}
	}
}

ModuleManager::ModuleManager() : System<Manager<Category>>("ModuleManager") {
    //Add dependencies for the system
	this->depends<ChatClient>();
	this->depends<EventDispatcher>();
    this->depends<OffsetManager>();
    this->depends<PatchManager>();
	this->depends<Preferences>();
    this->depends<SigManager>();
}

void ModuleManager::install(const std::shared_ptr<SystemInterface>& ownerPtr) {
    System<system_trait>::install(ownerPtr);

    //Create & register categories

    //Register categories
    auto combat = this->addItem<Category>("Combat", ThemeIcon::COMBAT);
    auto render = this->addItem<Category>("Render", ThemeIcon::RENDER);
    auto motion = this->addItem<Category>("Motion", ThemeIcon::MOTION);
    auto player = this->addItem<Category>("Player", ThemeIcon::PLAYER);
    auto misc = this->addItem<Category>("Misc", ThemeIcon::MISC);

	//Combat
	combat->addItem<Aimbot>();
	combat->addItem<AutoClicker>();
	combat->addItem<Killaura>();
	combat->addItem<Reach>();

	//Render
	render->addItem<BlockESP>();
	render->addItem<ClickGui>();
	render->addItem<CustomSky>();
	render->addItem<ESP>();
	render->addItem<Freelook>();
	render->addItem<Fullbright>();
	render->addItem<ModList>();
	render->addItem<Notifications>();
#ifdef _DEBUG
	render->addItem<PacketDebugger>();
#endif
	render->addItem<ServerRotations>();
	render->addItem<ThemeDebugger>();
	render->addItem<Tracers>();
#ifdef _DEBUG
	render->addItem<Xray>();
#endif

	//Motion
	motion->addItem<Airjump>();
	motion->addItem<AntiVoid>();
	motion->addItem<BHop>();
	motion->addItem<DamageBoost>();
	motion->addItem<Fly>();
	motion->addItem<Glide>();
	motion->addItem<HighJump>();
	//motion->addItem<Jesus>();
	motion->addItem<Phase>();
	motion->addItem<Spider>();
	motion->addItem<Step>();
	motion->addItem<Velocity>();
	//Player
	player->addItem<AutoArmor>();
	player->addItem<AutoSprint>();
	player->addItem<AutoTotem>();
	player->addItem<Autotool>();
	player->addItem<Instabreak>();
    player->addItem<InventoryCleaner>();
    player->addItem<NoFall>();
    player->addItem<NoSlowdown>();
    player->addItem<NoWeb>();
    player->addItem<Scaffold>();
    player->addItem<STap>();
    //Misc
    misc->addItem<AirPlace>();
    misc->addItem<AntiBot>();
    misc->addItem<AntiCrystal>();
    misc->addItem<Blink>();
    misc->addItem<ChestStealer>();
    misc->addItem<Disabler>();
    misc->addItem<IRC>();
    misc->addItem<NoFilter>();
    misc->addItem<NoPacket>();
    misc->addItem<Regen>();

#ifdef _DEBUG
    misc->addItem<Test>();
#endif
    misc->addItem<Timer>();
    misc->addItem<Uninject>();

	systems->getEventDispatcher()->listen<KeyEvent, &ModuleManager::onKeyEvent>(this);
}

std::vector<std::shared_ptr<Module>> ModuleManager::getAllModules(std::function<bool(std::shared_ptr<Module>, std::shared_ptr<Module>)> sort) {
    //Create new vector for returning all Modules
    auto allModules = std::vector<std::shared_ptr<Module>>();
    //Get all categories
    const auto& categories = getItems();
    //Loop through all categories
    for (auto& currentCategory : categories) {
        //Get all Modules from the Category
		const auto& modules = currentCategory->getItems();
        //Add the Modules to the allModules vector to be returned
        for (auto& module : modules) {
            allModules.push_back(module);
        }
    }

	std::sort(allModules.begin(), allModules.end(), std::move(sort));

    //Return allModules
    return allModules;
}
std::vector<std::shared_ptr<Module>> ModuleManager::getAllModules() {
	return this->getAllModules([](const std::shared_ptr<Module>&, const std::shared_ptr<Module>&) { return true; });
}

std::string toLower(std::string in) {
	std::string lower = "";
	for(auto c : in) {
		if(c <= 'Z'&& c >= 'A') {
			lower += (c - ('Z'- 'z'));
			continue;
		}
		lower += c;
	}
	return lower;
}

std::shared_ptr<Module> ModuleManager::findModule(std::string name) {
	std::vector<std::shared_ptr<Module>> allModules = this->getAllModules();
	for(auto mod : allModules) {
		std::string modName = mod->getName();
		if(toLower(modName) == toLower(name)) {
			return mod;
		}
	}
	return nullptr;
}
