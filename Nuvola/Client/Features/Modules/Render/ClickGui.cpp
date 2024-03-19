#include "ClickGui.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GuiData.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/KeyEvent.h"
#include "../../../Events/MouseEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../GUI/ClientScreen.h"
#include "../../../GUI/ClickGuiScreen.h"
#include "../../../GUI/ScreenManager.h"

#include "../ModuleManager.h"
#include "../IModeSetting.h"
#include "Notifications.h"

#include <Auth/User.h>
#include <Graphics/imfx.h>
#include <Math/Math.h>

#include <intrin.h>
#include <Windows.h>

#include <map>

#undef min
#undef max

void ClickGui::onRenderEvent(RenderEvent& event) {
	if (event.getTime() != RenderTime::MENU) return;

	auto screenMgr = systems->getScreenManager();
	std::shared_ptr<Screen> screen = screenMgr->getActiveScreen();
	if (screen != nullptr)
	{
		screen->render(ImGui::GetIO().DeltaTime);
	}
}

void ClickGui::onKeyEvent(KeyEvent& event)
{
	if (!this->isEnabled()) {
		return;
	}
	
	auto screenMgr = systems->getScreenManager();
	std::shared_ptr<Screen> screen = screenMgr->getActiveScreen();
	std::shared_ptr<ClientScreen> clientScreen = std::dynamic_pointer_cast<ClientScreen>(screen);
	if (clientScreen != nullptr)
	{
		clientScreen->handleInput(event);
	}
}

void ClickGui::onMouseEvent(MouseEvent& event)
{
	auto screenMgr = systems->getScreenManager();
	std::shared_ptr<Screen> screen = screenMgr->getActiveScreen();
	std::shared_ptr<ClientScreen> clientScreen = std::dynamic_pointer_cast<ClientScreen>(screen);
	if (clientScreen != nullptr)
	{
		clientScreen->handleInput(event);
	}
}

ClickGui::ClickGui() : Module("ClickGui", "A gui for interacting with client settings.", VK_INSERT) {
	this->scrollSpeed = this->addItem<Setting>("Scroll speed", "The speed at which you will be able to scroll in the menu", SettingType::SLIDER_FLOAT, 30.0f, 0.0f, 100.0f);
	
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &ClickGui::onRenderEvent>(this);
}
void ClickGui::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	//Get the module manager instance
	const auto modMgr = systems->getModuleManager();

	auto screenMgr = systems->getScreenManager();
	ScreenData genericData;
	screenMgr->pushScreen<ClickGuiScreen>(&genericData);

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) {
		return;
	}
	ci->acquireCursor();

	dispatcher->listen<KeyEvent, &ClickGui::onKeyEvent>(this);
	dispatcher->listen<MouseEvent, &ClickGui::onMouseEvent>(this);
}
void ClickGui::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) {
		return;
	}
	if (ci->isInGame()) {
		ci->dropCursor();
	}

	dispatcher->deafen<KeyEvent, &ClickGui::onKeyEvent>(this);
	dispatcher->deafen<MouseEvent, &ClickGui::onMouseEvent>(this);
}