#include "KeyPressHook.h"

#include "../../Events/EventDispatcher.h"
#include "../../SystemManager.h"

#include <imgui.h>
#include <imgui_custom.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void KeyPressHook::keyPressCallback_1_17_10_4(int key, int action) {
    ImGuiContext* g = ImGui::GetCurrentContext();
    if (g == nullptr) {
        return;
    }

	ImGuiIO& io = ImGui::GetIO();
    ImGuiKey imKey = ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key);
    io.AddKeyEvent(imKey, action);
    if (action)
    {
        HKL layout = GetKeyboardLayout(0);
        int scanCode = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
        BYTE translation[2];
        BYTE keyState[256] = { 0 };
        GetKeyboardState(keyState);
        int result = ToAscii(key, scanCode, keyState, (LPWORD)translation, 0);

        if (result == 1) {
            // If a single character is returned, return it
            char c = static_cast<char>(translation[0]);
            io.AddInputCharacter(c);
        }
        else if (result == 2) {
            // If a dead key or a special character is returned, return the second character in the buffer
            char sc = static_cast<char>(translation[1]);
            io.AddInputCharacter(sc);
        }
    }

	auto event = nes::make_holder<KeyEvent>(key, action);
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->trigger(event);
    if (!event->isCancelled() && !io.WantCaptureKeyboard) {
        PLH::FnCast(funcOriginal, keyPressCallback_1_17_10_4)(event->getKey(), (int)event->getAction());
    }
}

KeyPressHook::KeyPressHook() : IPatch("Global::KeyPress") {
	this->addSignature(SigTable::Global_keyAction);
}

auto KeyPressHook::apply() -> bool {
	return this->autoPatch(keyPressCallback_1_17_10_4, &funcOriginal);
}

void KeyPressHook::createPress(int key, KeyAction action)
{
    PLH::FnCast(funcOriginal, keyPressCallback_1_17_10_4)(key, (int)action);
}
