#include "ModList.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GuiData.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RenderEvent.h"
#include "../../Theme/ThemeManager.h"
#include "../ModuleManager.h"

#include <imgui.h>
#include <imgui_custom.h>

#include <Fonts/FontRegistry.h>
#include <Graphics/imfx.h>

#include <map>

#include <xorstr.hpp>
#undef min
#undef max

ModList::ModList() : Module("ModList", "Displays enabled modules.") {
}

static std::map<void*, float> entryExitAnim;
const int padding = 3.f;
void ModList::onRenderEvent(RenderEvent& event) {
	if (event.getTime() != RenderTime::UNDERLAY) return;
	//Only render if we enable the module
	if (!this->isEnabled()) {
		return;
	}

	//Try to get the font scale (assume 1.0 if we fail)
	float fontScale = 1.0;
	ImGuiIO& io = ImGui::GetIO();
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci != nullptr) {
		GuiData* gd = ci->getGuiData();
		if (gd != nullptr) {
			fontScale = gd->scale;
		}
	}

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);

	Theme theme = systems->getThemeManager()->getActiveTheme();
	const ColorF& baseColorBegin = theme[ThemeColor::MODLIST_COLOR_BEGIN];
	const ColorF& shadowColorBegin = theme[ThemeColor::MODLIST_SHADOW_BEGIN];
	const ColorF& textColorBegin = theme[ThemeColor::MODLIST_TEXT_BEGIN];
	const ColorF& baseColorEnd = theme[ThemeColor::MODLIST_COLOR_END];
	const ColorF& shadowColorEnd = theme[ThemeColor::MODLIST_SHADOW_END];
	const ColorF& textColorEnd = theme[ThemeColor::MODLIST_TEXT_END];

	//Iterate each module
	const auto modMgr = systems->getModuleManager();
	const auto& allModules = modMgr->getAllModules([&](std::shared_ptr<Module> a, std::shared_ptr<Module> b) -> bool {
		ImVec2 sizeA = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0, a->getName().c_str());
		ImVec2 sizeB = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0, b->getName().c_str());
		return sizeA.x > sizeB.x;
		});

	//Update animations
	size_t curIdx = 0;
	for (auto mod : allModules)
	{
		if (mod->isEnabled()) {
			entryExitAnim[mod.get()] += io.DeltaTime * 4;
			if (entryExitAnim[mod.get()] >= 1.0) {
				entryExitAnim[mod.get()] = 1.0;
			}
		}
		else {
			entryExitAnim[mod.get()] -= io.DeltaTime * 4;
			if (entryExitAnim[mod.get()] <= 0.0) {
				entryExitAnim[mod.get()] = 0.0;
				continue;
			}
		}
	}

	curIdx = 0;
	float yOffset = padding;

	// Draw our background
	ImFX::Begin(drawList);
	for (auto mod : allModules) {
		if (!mod->isEnabled() && entryExitAnim[mod.get()] <= 0.001f)
			continue;

		// Calculate the size of the text
		ImVec2 textSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0, mod->getName().c_str());

		// Calculate padding values
		float xPadding = padding * 2;
		float yPadding = padding;

		// Calculate end and start positions
		Vector2<float> endPosition(
			io.DisplaySize.x - textSize.x - xPadding + 1.f,
			yOffset);

		Vector2<float> startPosition(
			io.DisplaySize.x + xPadding,
			yOffset);

		// Interpolate between the start and end positions
		Vector2<float> lerpedPos = startPosition.interpolate(endPosition, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));

		// Calculate fullRect with padding
		Vector4<float> fullRect(
			lerpedPos.x - xPadding,
			lerpedPos.y - yPadding,
			lerpedPos.x + textSize.x + xPadding,
			lerpedPos.y + textSize.y + yPadding);

		// Colorz
		ColorF baseColor = baseColorBegin.interpolate(baseColorEnd, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));
		ColorF shadowColor = shadowColorBegin.interpolate(shadowColorEnd, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));

		// Render effects n shiz
		ImFX::AddShadow(8.0f, shadowColor, fullRect);
		ImFX::AddBlur(8.0f * ImGui::easeInOutCubic(entryExitAnim[mod.get()]), fullRect);
		ImFX::AddColor(baseColor, Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f), fullRect);

		yOffset = std::lerp(yOffset, yOffset + 24 + yPadding * 2, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));
	}
	ImFX::End();

	// Reset yOffset
	yOffset = padding;

	// Draw our text
	ImFX::Begin(drawList);
	for (auto mod : allModules) {
		if (!mod->isEnabled() && entryExitAnim[mod.get()] <= 0.001f)
			continue;

		// Calculate the size of the text
		ImVec2 textSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0, mod->getName().c_str());

		// Calculate padding values
		float xPadding = padding * 2;
		float yPadding = padding;

		// Calculate end and start positions
		Vector2<float> endPosition(
			io.DisplaySize.x - textSize.x - xPadding,
			yOffset);

		Vector2<float> startPosition(
			io.DisplaySize.x + xPadding,
			yOffset);

		// Interpolate between the start and end positions
		Vector2<float> lerpedPos = startPosition.interpolate(endPosition, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));

		// Colorz
		ColorF textColor = textColorBegin.interpolate(textColorEnd, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));

		// Draw the text
		drawList->AddText(uiFont, 24.0f, lerpedPos, textColor.asInt(), mod->getName().c_str());

		yOffset = std::lerp(yOffset, yOffset + 24 + yPadding * 2, ImGui::easeInOutCubic(entryExitAnim[mod.get()]));
	}
	ImFX::End();
}

void ModList::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &ModList::onRenderEvent>(this);
}
void ModList::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<RenderEvent, &ModList::onRenderEvent>(this);
}