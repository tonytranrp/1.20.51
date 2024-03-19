#include "MouseActionHook.h"

#include "../../Events/EventDispatcher.h"

#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/GuiData.h"

#include <imgui.h>

#include <xorstr.hpp>

static void* s_mouseInputHandler = nullptr;
static short s_mouseX = 0;
static short s_mouseY = 0;

void MouseActionHook::mouseActionHookCallback_1_17_11_1(void* mouseInputHandler,char button, char action, short mouseX, short mouseY, short movementX, short movementY, bool scroll) {
	ImGuiContext* g = ImGui::GetCurrentContext();
	if (g == nullptr) {
		return;
	}

	//if (button >= 3) {
	//	//What??
	//	return;
	//}
	s_mouseInputHandler = mouseInputHandler;
	s_mouseX = mouseX;
	s_mouseY = mouseY;

	ImGuiIO& io = ImGui::GetIO();
	if (button > 0 && button < 3) {
		io.AddMouseButtonEvent(button - 1, action);
	}
	if (button == 4 /*scroll*/) {
		io.AddMouseWheelEvent(0, static_cast<int8_t>(action));
	}
	else
	{
		//The position values are 0 when it is a scroll, so we only add mouse events when the cursor pos is updated
		io.AddMousePosEvent(mouseX, mouseY);
	}

	auto event = nes::make_holder<MouseEvent>(button, static_cast<uint8_t>(action), mouseX, mouseY, movementX, movementY);
	
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	if(!event->isCancelled() && !io.WantCaptureMouse) {
		PLH::FnCast(funcOriginal, mouseActionHookCallback_1_17_11_1)(mouseInputHandler, (char)event->getButton(), event->getActionAsChar(), event->getMouseX(), event->getMouseY(), movementX, movementY, scroll);
	}
}

MouseActionHook::MouseActionHook() : IPatch(xorstr_("Global::MouseAction")) {
	this->addSignature(SigTable::Global_mouseAction);
}

auto MouseActionHook::apply() -> bool {
	return this->autoPatch(mouseActionHookCallback_1_17_11_1, &funcOriginal);
}

void MouseActionHook::createClick(MouseButton button, MouseAction action)
{
	PLH::FnCast(funcOriginal, mouseActionHookCallback_1_17_11_1)(s_mouseInputHandler, (char)button, (char)action, s_mouseX, s_mouseY, 0, 0, 0);
}
