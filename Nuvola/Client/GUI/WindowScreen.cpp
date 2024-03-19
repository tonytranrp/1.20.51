#include "WindowScreen.h"

#include "../Bridge/ClientInstance.h"

#include "../Features/Modules/Render/ClickGui.h"
#include "../Features/Modules/Render/Notifications.h"
#include "../Features/Modules/ModuleManager.h"
#include "../Features/Theme/ThemeManager.h"

#include "NuControls.h"
#include "ScreenManager.h"

#include <Graphics/imfx.h>
#include <Math/Math.h>

#include <imgui.h>
#include <imgui_custom.h>
#include <Fonts/FontRegistry.h>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include <windows.h>

WindowScreen::WindowScreen(std::string name) : ClientScreen(std::move(name))
{
}

void WindowScreen::initialize(ScreenData* data)
{
	this->guiSize = { 800, 500 };
}

void WindowScreen::render(float deltaTime)
{
	//Try to get the font scale (assume 1.0 if we fail)
	fontScale = 1.0f;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiContext* g = ImGui::GetCurrentContext();
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci != nullptr) {
		GuiData* gd = ci->getGuiData();
		if (gd != nullptr) {
			fontScale = gd->scale;
		}
	}

	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return;

	//Get the module manager instance
	const auto modMgr = systems->getModuleManager();
	auto screenMgr = systems->getScreenManager();
	std::shared_ptr<ClickGui> clickGui = systems->getModuleManager()->findModule<ClickGui>();

	if (clickGui->isEnabled()) {
		io.WantCaptureMouse = true;
		windowOpacity += io.DeltaTime * 4;
		if (windowOpacity >= 1.0) {
			windowOpacity = 1.0;
		}
	}
	else {
		windowOpacity -= io.DeltaTime * 4;
		if (windowOpacity <= 0.0) {
			windowOpacity = 0.0f;
		}
	}
	blurValue = windowOpacity * 9.0f;

	if (windowOpacity <= 0.0f) {
		screenMgr->popScreen();
		return;
	}

	//Setup colors
	Theme theme = systems->getThemeManager()->getActiveTheme();
	theme.pushFilter([&](ThemeColor name, ColorF color) -> ColorF
		{
			ColorF newColor = color;
			newColor.w *= windowOpacity;
			return newColor;
		});
	ColorF clickGuiBg = theme[ThemeColor::CLICKGUI_COLOR];

	//Blur screen
	ImFX::Begin(ImGui::GetBackgroundDrawList());
	ImFX::AddBlur(blurValue);
	ImFX::End();

	//Apply style vars
	ImGui::PushStyleColor(ImGuiCol_WindowBg, theme[ThemeColor::TOOLTIP_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_Text, theme[ThemeColor::TOOLTIP_TEXT]);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::GetStyle().WindowShadowSize = 0.0f;

	//Draw window
	ImGui::SetNextWindowSize(guiSize);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
	flags |= this->movable ? 0 : ImGuiWindowFlags_NoMove;
	if (ImGui::Begin(name.c_str(), nullptr, flags))
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImFX::Begin(drawList);
		ImFX::AddShadow(8.0f, theme[ThemeColor::CLICKGUI_SHADOW]);
		ImFX::AddBlur(blurValue * 4);
		ImFX::AddColor(theme[ThemeColor::CLICKGUI_COLOR]);
		ImFX::End();

		this->renderWindowContent(deltaTime);

		//Draw close button
		Vector2<float> closePos = Vector2<float>(guiSize.x - 35, 30) + ImGui::GetWindowPos();
		ImU32 hitboxCol = theme[ThemeColor::CLICKGUI_DEBUG_CLOSE_HITBOX].asInt();
		ImU32 closeCol = theme[ThemeColor::CLICKGUI_CLOSE_COLOR].asInt();
		ImU32 closeColHovered = theme[ThemeColor::CLICKGUI_CLOSE_HOVERED].asInt();
		Vector2<float> hitboxMin = closePos + (Vector2<float>(-4, -4) * fontScale);
		Vector2<float> hitboxMax = closePos + (Vector2<float>(4, 4) * fontScale);
		if (ImGui::IsMouseHoveringRect(hitboxMin, hitboxMax)) {
			closeCol = closeColHovered;
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				this->closeBehavior();
			}
		}
		drawList->AddCircleFilled(closePos, 10, closeCol);
	}
	ImGui::End();

	//Pop style vars
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);

	theme.popFilter();
}

void WindowScreen::cleanup()
{
}

void WindowScreen::handleInput(KeyEvent& event)
{
	if (event.getAction() != KeyAction::PRESSED) {
		return;
	}

	if (NuControls::getActiveComponent() == 0)
	{
		if (event.getKey() == VK_ESCAPE)
		{
			this->closeBehavior();
		}
	}

	event.cancel();
}

void WindowScreen::handleInput(MouseEvent& event)
{
}

void WindowScreen::closeBehavior()
{
	auto screenMgr = systems->getScreenManager();
	if (screenMgr != nullptr)
	{
		screenMgr->popScreen();
	}
}
