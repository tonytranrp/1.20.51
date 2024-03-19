#include "ThemeDebugger.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RenderEvent.h"
#include "../../Theme/ThemeManager.h"

#include <Fonts/FontRegistry.h>

#include <imgui.h>

ThemeDebugger::ThemeDebugger() : Module("ThemeDebugger", "Displays an overlay with theme errors.")
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &ThemeDebugger::onRenderEvent>(this);
}

void ThemeDebugger::onRenderEvent(RenderEvent& event)
{
	if (!this->isEnabled()) return;
	if (event.getTime() != RenderTime::OVERLAY) return;

	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	Theme theme = systems->getThemeManager()->getActiveTheme();

	const auto& errors = theme.getErrors();
	const auto& warns = theme.getWarnings();

	Vector2<float> cursor = { 0,0 };

	drawList->AddText(uiFont, 20.0f, cursor, 0xFFFFFFFF, std::string("Theme loaded with " + std::to_string(errors.size()) + " errors and " + std::to_string(warns.size()) + " warnings").c_str());
	cursor.y += 20.0f;

	for (const auto& error : errors)
	{
		drawList->AddText(uiFont, 20.0f, cursor, 0xFF0000FF, error.c_str());
		cursor.y += 20.0f;
	}
	for (const auto& warn : warns)
	{
		drawList->AddText(uiFont, 20.0f, cursor, 0xFFFF00FF, warn.c_str());
		cursor.y += 20.0f;
	}
}