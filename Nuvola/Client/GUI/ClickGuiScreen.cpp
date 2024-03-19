#include "ClickGuiScreen.h"

#include "../Bridge/ClientInstance.h"
#include "../Bridge/GuiData.h"

#include "../Features/Config/ConfigManager.h"
#include "../Features/Modules/Render/ClickGui.h"
#include "../Features/Modules/Render/Notifications.h"
#include "../Features/Modules/GroupSetting.h"
#include "../Features/Modules/IModeSetting.h"
#include "../Features/Modules/ModuleManager.h"
#include "../Features/Theme/ThemeManager.h"
#include "../Preferences/Preferences.h"

#include "BlockTypeSelectionScreen.h"
#include "EnchantWeightScreen.h"
#include "EntityTypeSelectionScreen.h"
#include "NuControls.h"

#include <Auth/User.h>
#include <Fonts/FontRegistry.h>
#include <Graphics/imfx.h>
#include "ScreenManager.h"

#include <imgui.h>
#include <imgui_custom.h>
#include <imgui_internal.h>

ClickGuiScreen::ClickGuiScreen() : WindowScreen("ClickGUI")
{

}

extern User* currentUser;

constexpr float sliderThiccness = 2.0f;
constexpr float animationSpeed = 8.0f;

#undef min
#undef max
#undef DELETE

bool ClickGuiScreen::drawHotkeyControl(ImDrawList* drawList, std::shared_ptr<Module> module, Vector4<float> controlBox)
{
	if (module == nullptr)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}

	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImGuiIO& io = ImGui::GetIO();

	ColorF hotkeyColor = theme[ThemeColor::BUTTON_COLOR];
	ColorF hotkeyHovered = theme[ThemeColor::BUTTON_HOVERED];
	bool changing = changingKey[module.get()];
	bool hovered = controlBox.contains(io.MousePos);
	bool clicked = hovered ? io.MouseClicked[ImGuiMouseButton_Left] : false;

	if (hovered)
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), hotkeyHovered.asInt(), 4.0f);
		if (clicked)
		{
			changingKey[module.get()] = true;
			NuControls::setActiveComponent(id);
		}
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), hotkeyColor.asInt(), 4.0f);
	}

	std::pair<ImFont*, std::string> keyDrawData;
	if (changingKey[module.get()])
	{
		keyDrawData.first = icoonFont;
		keyDrawData.second = theme[ThemeIcon::CHANGING_KEY].glyph;
	}
	else
	{
		keyDrawData.first = uiFont;
		char key = static_cast<char>(module->getHotkey());
		if (key == 0)
		{
			keyDrawData.second = "None";
		}
		else
		{
			keyDrawData.second = key;
		}
	}

	Vector2<float> keyDrawSize = keyDrawData.first->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, keyDrawData.second.c_str());
	Vector2<float> keyDrawPos = {
		controlBox.center().x - (keyDrawSize.x / 2.0f),
		controlBox.center().y - (keyDrawSize.y / 2.0f)
	};
	drawList->AddText(keyDrawData.first, 16.0f, keyDrawPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), keyDrawData.second.c_str());

	return true;
}
void ClickGuiScreen::drawSettingsRecursive(ImDrawList* drawList, const std::shared_ptr<Manager<Setting>>& settings, Vector2<float>& pos, Vector4<float> contentClip, float expandProg)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	auto screenMgr = systems->getScreenManager();

	if (settings == nullptr)
		return;

	for (auto& setting : settings->getItems()) {
		ImGui::PushID(setting.get());

		Vector2<float> labelSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, setting->getName().c_str());
		Vector2<float> labelPos = pos;
		drawList->AddText(uiFont, 24.0f, labelPos, theme[ThemeColor::CLICKGUI_SETTING_LABEL].asInt(), setting->getName().c_str());

		switch (setting->getType()) {
		case SettingType::SLIDER_FLOAT: {
			float min = std::any_cast<float>(setting->getMinimum());
			float max = std::any_cast<float>(setting->getMaximum());
			float value = std::any_cast<float>(setting->getValue());

			Vector4<float> sliderBox = {
				contentClip.center().x - 40.0f,
				labelPos.y,
				contentClip.z - 110.0f,
				labelPos.y + labelSize.y
			};
			NuControls::drawSliderFloatControl(drawList, &value, min, max, sliderBox);

			Vector4<float> valLabelBox = {
				contentClip.z - 90.0f,
				labelPos.y,
				contentClip.z - 40.0f,
				labelPos.y + labelSize.y
			};
			std::ostringstream oss;
			oss << std::setprecision(3) << value;
			drawList->AddText(uiFont, 24.0f, valLabelBox.min(), theme[ThemeColor::CLICKGUI_SETTING_LABEL].asInt(), oss.str().c_str());

			setting->setValue(value);
			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		case SettingType::SLIDER_INT: {
			int min = std::any_cast<int>(setting->getMinimum());
			int max = std::any_cast<int>(setting->getMaximum());
			int value = std::any_cast<int>(setting->getValue());

			Vector4<float> sliderBox = {
				contentClip.center().x - 40.0f,
				labelPos.y,
				contentClip.z - 110.0f,
				labelPos.y + labelSize.y
			};
			NuControls::drawSliderIntControl(drawList, &value, min, max, sliderBox);

			Vector4<float> valLabelBox = {
				contentClip.z - 90.0f,
				labelPos.y,
				contentClip.z - 40.0f,
				labelPos.y + labelSize.y
			};
			drawList->AddText(uiFont, 24.0f, valLabelBox.min(), theme[ThemeColor::CLICKGUI_SETTING_LABEL].asInt(), std::to_string(value).c_str());

			setting->setValue(value);
			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		case SettingType::TOGGLE: {
			bool value = std::any_cast<bool>(setting->getValue());
			Vector4<float> toggleBox = {
				contentClip.z - 90.0f,
				labelPos.y,
				contentClip.z - 50.0f,
				labelPos.y + labelSize.y
			};
			NuControls::drawToggleControl(drawList, &value, toggleBox);
			setting->setValue(value);
			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		case SettingType::ENUM: {
			int curSelect = setting->getValueAs<int>();
			const std::vector<std::string>& enumStrs = setting->getEnumStrs();

			Vector4<float> enumBox = {
				contentClip.center().x - 40.0f,
				labelPos.y,
				contentClip.z - 40.0f,
				labelPos.y + labelSize.y
			};
			NuControls::drawOptimalEnumControl(drawList, &curSelect, enumStrs, enumBox);

			setting->setValue(curSelect);
			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);

			//If it's a mode setting, we have to draw its settings too
			std::shared_ptr<IModeSetting> modeSetting = std::dynamic_pointer_cast<IModeSetting>(setting);
			if (modeSetting != nullptr)
			{
				modeSetting->updateState(ImGui::GetIO().DeltaTime * animationSpeed);
				auto state = modeSetting->getAnimState();
				if (state.lastActive)
				{
					theme.pushFilter([&](ThemeColor color, ColorF value)->ColorF {
						value.w *= (1.0f - state.hideLastProg);
						return value;
						});
					this->drawSettingsRecursive(drawList, state.lastActive, pos, contentClip, expandProg * (1.0f - state.hideLastProg));
					theme.popFilter();
				}
				theme.pushFilter([&](ThemeColor color, ColorF value)->ColorF {
					value.w *= state.showActiveProg;
					return value;
					});
				this->drawSettingsRecursive(drawList, state.nowActive, pos, contentClip, expandProg * state.showActiveProg);
				theme.popFilter();
			}
			break;
		}
		case SettingType::ENTITY_TYPES: {
			Vector4<float> buttonBox = {
				contentClip.z - 110.0f,
				labelPos.y,
				contentClip.z - 50.0f,
				labelPos.y + labelSize.y
			};
			if (NuControls::drawButtonControl(drawList, "Select", buttonBox))
			{
				EntityTypeSelectionScreenData data(setting);
				screenMgr->pushScreen<EntityTypeSelectionScreen>(&data);
				this->windowPos = ImGui::GetWindowPos(); //Store window pos for when clickgui renders again
			}

			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		case SettingType::BLOCK_TYPES: {
			Vector4<float> buttonBox = {
					contentClip.z - 110.0f,
					labelPos.y,
					contentClip.z - 50.0f,
					labelPos.y + labelSize.y
			};
			if (NuControls::drawButtonControl(drawList, "Select", buttonBox))
			{
				BlockTypeSelectionScreenData data(setting);
				screenMgr->pushScreen<BlockTypeSelectionScreen>(&data);
				this->windowPos = ImGui::GetWindowPos(); //Store window pos for when clickgui renders again
			}

			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		case SettingType::GROUP: {
			std::shared_ptr<GroupSetting> groupSetting = std::dynamic_pointer_cast<GroupSetting>(setting);
			if (groupSetting != nullptr)
			{
				groupSetting->updateState(ImGui::GetIO().DeltaTime * animationSpeed);
				Vector4<float> expanderBox = {
					contentClip.z - 110.0f,
					labelPos.y,
					contentClip.z - 50.0f,
					labelPos.y + labelSize.y
				};

				//Render the expand/collapse icon
				bool& expanded = groupSetting->expanded;
				std::string expandIco = expanded ? theme[ThemeIcon::GUI_COLLAPSE].glyph : theme[ThemeIcon::GUI_EXPAND].glyph;
				ImVec2 expandIcoSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, expandIco.c_str());
				//expandIcoSize.x *= 2.0f;
				Vector2<float> expandIcoPos = {
					expanderBox.x + ((expanderBox.size().x / 2.0f) - (expandIcoSize.x / 2.0f)),
					expanderBox.y + ((expanderBox.size().y / 2.0f) - (expandIcoSize.y / 2.0f))
				};
				if (ImGui::IsMouseHoveringRect(expandIcoPos, expandIcoPos + expandIcoSize))
				{
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						expanded = !expanded;
					}
				}
				drawList->AddText(icoonFont, 16.0f, expandIcoPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), expandIco.c_str());

				pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
				if (expanded)
				{
					this->drawSettingsRecursive(drawList, groupSetting, pos, contentClip, expandProg * groupSetting->expandProg);
				}
			}
			else
			{
				pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			}
			break;
		}
		case SettingType::ENCHANT_WEIGHTS:
		{
			Vector4<float> buttonBox = {
				contentClip.z - 110.0f,
				labelPos.y,
				contentClip.z - 50.0f,
				labelPos.y + labelSize.y
			};
			if (NuControls::drawButtonControl(drawList, "Edit", buttonBox))
			{
				auto casted = std::dynamic_pointer_cast<EnchantWeightSetting>(setting);
				assert(casted != nullptr);

				EnchantWeightScreenData data(casted);
				screenMgr->pushScreen<EnchantWeightScreen>(&data);
				this->windowPos = ImGui::GetWindowPos(); //Store window pos for when clickgui renders again
			}

			pos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * expandProg);
			break;
		}
		default: {
			ImGui::TextWrapped("This message should never be shown. If it is shown, something has gone terribly wrong with your settings.");
			break;
		}
		}

		if (ImGui::IsMouseHoveringRect(labelPos, labelPos + labelSize))
		{
			ImGui::SetTooltipThemed(setting->getDescription().c_str());
		}

		ImGui::PopID();

		ImGui::SetCursorPos(pos);
	}
}
void ClickGuiScreen::drawModuleContent(ImDrawList* drawList, const std::shared_ptr<Module>& module, Vector2<float> pos, Vector4<float> contentClip, Vector4<float>& moduleRegion)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return;
	ImGuiIO& io = ImGui::GetIO();
	Theme theme = systems->getThemeManager()->getActiveTheme();

	Vector4<float> controlBox = {
		pos.x,
		pos.y,
		contentClip.z - 30.0f,
		pos.y + 50.0f
	};

	Vector2<float> drawPos = pos + Vector2<float>(10.0f, 15.0f);

	ImGui::PushID(module.get());
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();

	if (moduleExpanded[module.get()])
	{
		animProgs[id] += io.DeltaTime * animationSpeed;
		if (animProgs[id] > 1.0f)
		{
			animProgs[id] = 1.0f;
		}
	}
	else
	{
		animProgs[id] -= io.DeltaTime * animationSpeed;
		if (animProgs[id] < 0.0f)
		{
			animProgs[id] = 0.0f;

			//We dont want to draw anything
			ImGui::PopID();
			return;
		}
	}

	theme.pushFilter([&](ThemeColor color, ColorF val) -> ColorF
		{
			val.w *= animProgs[id];
			return val;
		});

	try {
		{
			drawList->AddText(uiFont, 24.0f, drawPos, theme[ThemeColor::CLICKGUI_SETTING_LABEL].asInt(), "Hotkey");
			Vector2<float> labelSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, "None");
			Vector4<float> controlBox = {
					contentClip.z - 90.0f,
					drawPos.y,
					contentClip.z - 40.0f,
					drawPos.y + labelSize.y
			};
			this->drawHotkeyControl(drawList, module, controlBox);

			drawPos += Vector2<float>(0.0f, (labelSize.y + 10.0f) * animProgs[id]);
		}

		this->drawSettingsRecursive(drawList, module, drawPos, contentClip, animProgs[id]);
	}
	catch (const std::exception& ex)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "A rendering error occoured: %s", ex.what());
		ImGui::PopID();
	}

	theme.popFilter();

	moduleRegion.w = drawPos.y - pos.y;
	ImGui::PopID();
}
bool ClickGuiScreen::drawModuleControl(class ImDrawList* drawList, const std::shared_ptr<Module>& module, Vector2<float> pos, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();

	Vector4<float> controlBox = {
		pos.x,
		pos.y,
		contentClip.z - 30.0f,
		pos.y + 50.0f
	};
	//Render the module name text
	ImVec2 modNameSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, module->getName().c_str());
	Vector2<float> modNamePos = {
		pos.x + 20.0f,
		pos.y + (25.0f - (modNameSize.y / 2.0f))
	};
	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::CLICKGUI_MODULECONTROL_HOVERED].asInt(), 8.0f);
		drawList->AddText(uiFont, 24.0f, modNamePos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), module->getName().c_str());

		ImGui::SetTooltipThemed(module->getDescription().c_str());
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::CLICKGUI_MODULECONTROL_COLOR].asInt(), 8.0f);
		drawList->AddText(uiFont, 24.0f, modNamePos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), module->getName().c_str());
	}

	//Render the expand/collapse icon
	bool expanded = moduleExpanded[module.get()];
	std::string expandIco = expanded ? theme[ThemeIcon::GUI_COLLAPSE].glyph : theme[ThemeIcon::GUI_EXPAND].glyph;
	ImVec2 expandIcoSize = uiFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, expandIco.c_str());
	expandIcoSize.x *= 2.0f;
	Vector2<float> expandIcoPos = {
		controlBox.z - 40.0f,
		pos.y + (25.0f - (expandIcoSize.y / 2.0f))
	};
	if (ImGui::IsMouseHoveringRect(expandIcoPos, expandIcoPos + expandIcoSize))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			moduleExpanded[module.get()] = !moduleExpanded[module.get()];
		}
	}
	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			moduleExpanded[module.get()] = !moduleExpanded[module.get()];
		}
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !ImGui::IsMouseHoveringRect(expandIcoPos, expandIcoPos + expandIcoSize))
		{
			module->setEnabled(!module->isEnabled());
		}
	}
	drawList->AddText(icoonFont, 16.0f, expandIcoPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), expandIco.c_str());

	//Render the enabled toggle
	Vector2<float> center = controlBox.center();
	Vector4<float> toggleBox = {
		controlBox.z - 100.0f,
		controlBox.y + ((center.y - controlBox.y) / 2.0f),
		controlBox.z - 60.0f,
		center.y + ((controlBox.w - center.y) / 2.0f)
	};

	bool enabled = module->isEnabled();
	NuControls::drawToggleControl(drawList, &enabled, toggleBox);
	module->setEnabled(enabled);

	return expanded;
}
static int64_t scrollAmount = 0;
static int8_t lastEventScroll = 0;
void ClickGuiScreen::drawCategoryContent(ImDrawList* drawList, const std::shared_ptr<Category>& category, Vector4<float> contentClip)
{
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGui::PushClipRect(contentClip.min(), contentClip.max(), true);
	ImGuiIO& io = ImGui::GetIO();

	Vector2<float> drawPosition = { contentClip.x + 15.0f, (contentClip.y + 15.0f) - scrollAmount };
	Vector2<float> positionDiff = { 0.0f, 65.0f };

	if (category != nullptr)
	{
		// Draw a button for each item in the category
		for (const auto& module : category->getItems()) {
			ImGui::PushID(module.get());
			this->drawModuleControl(drawList, module, drawPosition, contentClip);

			Vector2<float> settingsDrawPos = {
				drawPosition.x,
				drawPosition.y + 50.0f
			};
			Vector4<float> moduleRegion;
			this->drawModuleContent(drawList, module, settingsDrawPos, contentClip, moduleRegion);
			drawPosition += { 0.0f, moduleRegion.w - moduleRegion.y };

			drawPosition += positionDiff;
			ImGui::PopID();
		}
	}
	else
	{
		//If category is nullptr, we render all of the modules
		for (const auto& module : systems->getModuleManager()->getAllModules([](auto a, auto b) -> bool {
			//Compare alphabetically
			return a->getName() < b->getName();
			}))
		{
			ImGui::PushID(module.get());
			if (!searchQuery.empty())
			{
				std::string lowerCaseQuery;
				std::transform(searchQuery.begin(), searchQuery.end(), std::back_inserter(lowerCaseQuery), [](unsigned char c) {
					return std::tolower(c);
					});

				std::string moduleName = module->getName();
				std::string lowerCaseName;
				std::transform(moduleName.begin(), moduleName.end(), std::back_inserter(lowerCaseName), [](unsigned char c) {
					return std::tolower(c);
					});

				if (lowerCaseName.find(lowerCaseQuery) == std::string::npos)
				{
					ImGui::PopID();
					continue;
				}
			}
			this->drawModuleControl(drawList, module, drawPosition, contentClip);

			Vector2<float> settingsDrawPos = {
					drawPosition.x,
					drawPosition.y + 50.0f
			};

			Vector4<float> moduleRegion;
			this->drawModuleContent(drawList, module, settingsDrawPos, contentClip, moduleRegion);
			drawPosition += { 0.0f, moduleRegion.w - moduleRegion.y };

			drawPosition += positionDiff;
			ImGui::PopID();
		}
	}

	drawPosition.y -= contentClip.y;
	drawPosition.y += scrollAmount;

	if (contentClip.contains(io.MousePos))
	{
		scrollAmount += lastEventScroll;
		lastEventScroll = 0;
	}

	if (scrollAmount <= 0)
		scrollAmount = 0;
	const int64_t scrollMax = std::max(1.0f, drawPosition.y - (contentClip.w - contentClip.y));
	if (scrollMax <= scrollAmount)
		scrollAmount = scrollMax;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiID id = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
	ImRect scrollRect = {
		contentClip.z - 15.0f,
		contentClip.y,
		contentClip.z,
		contentClip.w
	};
	ImGui::GetStyle().WindowRounding = 8.0f;
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, theme[ThemeColor::SCROLLBAR_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme[ThemeColor::SCROLLBAR_SCROLLER_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, theme[ThemeColor::SCROLLBAR_SCROLLER_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, theme[ThemeColor::SCROLLBAR_SCROLLER_MOVING]);
	if (ImGui::ScrollbarEx(scrollRect, id, ImGuiAxis_Y, &scrollAmount, contentClip.w - contentClip.y, drawPosition.y, ImDrawFlags_RoundCornersBottomRight))
	{
		NuControls::setActiveComponent(0);
	}
	ImGui::PopStyleColor(4);

	ImGui::PopClipRect();
}
bool ClickGuiScreen::drawCategoryControl(ImDrawList* drawList, const std::shared_ptr<Category>& category, Vector2<float> pos)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();

	ImVec2 iconSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, theme[category->getIcon()].glyph.c_str());
	ImVec2 textSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, category->getName().c_str());

	ColorF textColor = theme[ThemeColor::CLICKGUI_TEXT_COLOR];

	ImVec2 selectionRectMin = {
		pos.x - 10.0f,
		pos.y - 5.0f
	};
	ImVec2 selectionRectMax = {
		pos.x + textSize.x + 40.0f,
		pos.y + textSize.y + 5.0f
	};
	if (activeCategory == category)
	{
		textColor = theme[ThemeColor::CLICKGUI_TEXT_ACTIVE];
		drawList->AddRectFilled(selectionRectMin, selectionRectMax, theme[ThemeColor::BUTTON_ACTIVE].asInt(), 8.0f);
	}
	else if (ImGui::IsMouseHoveringRect(selectionRectMin, selectionRectMax))
	{
		textColor = theme[ThemeColor::CLICKGUI_TEXT_HOVERED];
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			if (activeCategory == category)
			{
				activeCategory = nullptr;
			}
			else
			{
				activeCategory = category;
				scrollAmount = 0;
				searchQuery.clear();
				activeMenu = 0;
			}
			NuControls::setActiveComponent(0);
		}
		drawList->AddRectFilled(selectionRectMin, selectionRectMax, theme[ThemeColor::BUTTON_HOVERED].asInt(), 8.0f);
	}
	else
	{
		drawList->AddRectFilled(selectionRectMin, selectionRectMax, theme[ThemeColor::BUTTON_COLOR].asInt(), 8.0f);
	}

	drawList->AddText(icoonFont, 16.0f, pos + Vector2<float>{ 0.0f, 3.0f }, textColor.asInt(), theme[category->getIcon()].glyph.c_str());
	drawList->AddText(uiFont, 24.0f, Vector2<float>(30.0f, 0.0f) + pos, textColor.asInt(), category->getName().c_str());

	return category == activeCategory;
}

bool ClickGuiScreen::drawTileControl(class ImDrawList* drawList, std::string tileTitle, std::string tileSubtitle, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ColorF textColor = theme[ThemeColor::CLICKGUI_TILE_TITLE];
	ColorF subtextColor = theme[ThemeColor::CLICKGUI_TILE_SUBTITLE];
	ColorF tileColor = theme[ThemeColor::CLICKGUI_TILE_COLOR];
	ColorF hoverColor = theme[ThemeColor::CLICKGUI_TILE_HOVER];
	ImGuiIO& io = ImGui::GetIO();

	drawList->AddRectFilled(contentClip.min(), contentClip.max(), tileColor.asInt(), 6.0f);

	Vector4<float> titleRect = {
		contentClip.x + 5.0f,
		contentClip.y + 5.0f,
		contentClip.z - 5.0f,
		contentClip.center().y + 15.0f
	};
	Vector2<float> titleSize = uiFont->CalcTextSizeA(20.0f, FLT_MAX, titleRect.size().x, tileTitle.c_str());
	Vector2<float> titlePos = {
		titleRect.center().x - (titleSize.x / 2.0f),
		titleRect.center().y - (titleSize.y / 2.0f)
	};
	drawList->AddText(uiFont, 20.0f, titlePos, textColor.asInt(), tileTitle.c_str());

	Vector4<float> subtitleRect = {
		titleRect.x,
		titleRect.w,
		titleRect.z,
		contentClip.w
	};
	Vector2<float> subtitleSize = uiFont->CalcTextSizeA(16.0f, FLT_MAX, subtitleRect.size().x, tileSubtitle.c_str());
	Vector2<float> subtitlePos = {
		subtitleRect.center().x - (subtitleSize.x / 2.0f),
		subtitleRect.y
	};
	drawList->AddText(uiFont, 16.0f, subtitlePos, subtextColor.asInt(), tileSubtitle.c_str(), 0, contentClip.size().x);

	if (contentClip.contains(io.MousePos))
	{
		drawList->AddRectFilled(contentClip.min(), contentClip.max(), hoverColor.asInt(), 6.0f);
		ImFX::Begin(drawList);
		ImFX::AddBlur(8.0f, contentClip, 8.0f);
		ImFX::End();
	}

	return contentClip.contains(io.MousePos);
}
bool ClickGuiScreen::drawPreferencesMenu(ImDrawList* drawList, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ColorF textCol = theme[ThemeColor::CLICKGUI_TEXT_COLOR];
	ColorF color = theme[ThemeColor::CLICKGUI_SIDEBAR_COLOR];
	ColorF seperator = theme[ThemeColor::SEGMENTED_SECONDARY];

	float fontSize = 20.0f;
	float margin = 10.0f;
	float padding = 10.0f;
	Vector2<float> labelPos = contentClip.min() + padding;
	drawList->AddText(labelPos, textCol.asInt(), "Settings");

	auto prefs = systems->getPreferences();
	int prefsCount = 2;
	
	Vector4<float> settingsArea = {
			contentClip.x,
			contentClip.y + 35.0f,
			contentClip.z,
			contentClip.w
	};

	Vector4<float> panelBox = {
		settingsArea.x + margin,
		settingsArea.y + margin,
		settingsArea.z - margin,
		settingsArea.y + (prefsCount * fontSize) + ((prefsCount + 1) * padding)
	};

	drawList->AddRectFilled(panelBox.min(), panelBox.max(), color.asInt(), 4.0f);

	bool runPayload = false;
	for (int i = 0; i < prefsCount; i++)
	{
		Vector4<float> selectionRegion = {
			panelBox.x,
			panelBox.y + (i * fontSize) + (i * padding),
			panelBox.z,
			panelBox.y + ((i + 1) * fontSize) + ((i + 1) * padding)
		};

		std::string labelText = "";
		switch (i)
		{
		case 0:
		{
			labelText = "Use config keybinds";
			Vector4<float> toggleBox = {
				selectionRegion.z - 50.0f,
				selectionRegion.center().y - ((fontSize / 2.0f) + (padding / 4.0f)),
				selectionRegion.z - 10.0f,
				selectionRegion.center().y + ((fontSize / 2.0f) + (padding / 4.0f))
			};
			if (NuControls::drawToggleControl(drawList, &prefs->configKeymap, toggleBox))
				runPayload = true;
			break;
		}
		case 1:
			labelText = "Graphics mode";
			Vector4<float> enumBox = {
				selectionRegion.center().x - 50.0f,
				selectionRegion.y + (padding / 2.0f),
				selectionRegion.z - 10.0f,
				selectionRegion.y + fontSize + (padding /2.0f)
			};
			auto names = magic_enum::enum_names<ImFX::FXMode>();
			std::vector<std::string> modes(names.begin(), names.end());
			if (NuControls::drawOptimalEnumControl(drawList, reinterpret_cast<int*>(&prefs->graphicsMode), modes, enumBox))
				runPayload = true;
			break;
		}

		Vector2<float> labelSize = uiFont->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, labelText.c_str());
		Vector4<float> labelRect = {
			selectionRegion.x + padding,
			selectionRegion.center().y - (labelSize.y / 2.0f),
			selectionRegion.x + padding + labelSize.x,
			selectionRegion.center().y + (labelSize.y / 2.0f)
		};
		drawList->AddText(uiFont, fontSize, labelRect.min(), textCol.asInt(), labelText.c_str());
	}

	if (runPayload)
	{
		prefs->runPayloadAndSave();
	}

	return false;
}
bool ClickGuiScreen::drawThemeMenu(class ImDrawList* drawList, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGui::PushClipRect(contentClip.min(), contentClip.max(), true);
	ImGuiIO& io = ImGui::GetIO();

	Vector2<float> titleLabelSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, "Themes");
	Vector2<float> titleLabelPos = {
		contentClip.x + 15.0f,
		contentClip.y + 15.0f
	};
	drawList->AddText(uiFont, 24.0f, titleLabelPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), "Themes");

	Vector4<float> themePanelBox = {
		titleLabelPos.x,
		titleLabelPos.y + titleLabelSize.y + 15.0f,
		contentClip.z - 30.0f,
		contentClip.w - 15.0f
	};
	drawList->AddRectFilled(themePanelBox.min(), themePanelBox.max(), theme[ThemeColor::CLICKGUI_CONFIG_PANEL].asInt(), 8.0f);

	Vector4<float> addThemeBox = {
		themePanelBox.z - 40.0f,
		contentClip.y + 15.0f,
		themePanelBox.z,
		contentClip.y + 45.0f
	};
	Vector4<float> themeNameBox = {
		themePanelBox.z - 250.0f,
		contentClip.y + 15.0f,
		themePanelBox.z - 50.0f,
		contentClip.y + 45.0f
	};

	std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();

	auto themes = systems->getThemeManager();
	if (NuControls::drawButtonControl(drawList, theme[ThemeIcon::REFRESH], addThemeBox))
	{
		themes->reloadAllThemes();
		notifs->notify("Themes reloaded!");
	}


	ImGui::PushClipRect(themePanelBox.min(), themePanelBox.max(), true);

	constexpr int tilesPerRow = 3;
	float tileWidth = themePanelBox.size().x / tilesPerRow;
	int gridY = 0;
	for (int i = 0; i < themes->getThemes().size(); i++)
	{
		const std::shared_ptr<Theme>& curTheme = themes->getThemes().at(i);
		int gridX = i % tilesPerRow;
		gridY = i / tilesPerRow;

		Vector4<float> tileBox = {
			themePanelBox.x + 5.0f + (tileWidth * gridX),
			themePanelBox.y + 5.0f + (tileWidth * gridY) - scrollAmount,
			(themePanelBox.x - 5.0f) + (tileWidth * (gridX + 1)),
			(themePanelBox.y - 5.0f) + (tileWidth * (gridY + 1)) - scrollAmount
		};
		if (this->drawTileControl(drawList, curTheme->getName(), "By " + curTheme->getAuthor(), tileBox))
		{
			/* Draw hover controls */
			Vector2<float> padding = { 10.0f, 10.0f };
			Vector2<float> loadLabelSize = padding + uiFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, "Load");

			Vector4<float> loadSplit = {
				tileBox.x,
				tileBox.y + (tileBox.size().y / 3.0f),
				tileBox.z,
				tileBox.y + ((tileBox.size().y / 3.0f) * 2)
			};

			Vector4<float> loadButtonBox = {
				loadSplit.center().x - (loadLabelSize.x / 2.0f),
				loadSplit.center().y - (loadLabelSize.y / 2.0f),
				loadSplit.center().x + (loadLabelSize.x / 2.0f),
				loadSplit.center().y + (loadLabelSize.y / 2.0f),
			};
			if (NuControls::drawButtonControl(drawList, "Load", loadButtonBox))
			{
				nextThemeIndex = i;
			}
		}
	}
	ImGui::PopClipRect();

	if (themePanelBox.contains(io.MousePos))
	{
		scrollAmount += lastEventScroll;
		lastEventScroll = 0;
	}

	if (scrollAmount <= 0)
		scrollAmount = 0;
	const int64_t scrollMax = std::max(1.0f, ((tileWidth + 5.0f) * (gridY + 1)) - (themePanelBox.w - themePanelBox.y));
	if (scrollMax <= scrollAmount)
		scrollAmount = scrollMax;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiID id = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
	ImRect scrollRect = {
		contentClip.z - 15.0f,
		contentClip.y,
		contentClip.z,
		contentClip.w
	};
	ImGui::GetStyle().WindowRounding = 8.0f;
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, theme[ThemeColor::SCROLLBAR_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme[ThemeColor::SCROLLBAR_SCROLLER_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, theme[ThemeColor::SCROLLBAR_SCROLLER_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, theme[ThemeColor::SCROLLBAR_SCROLLER_MOVING]);
	if (ImGui::ScrollbarEx(scrollRect, id, ImGuiAxis_Y, &scrollAmount, themePanelBox.w - themePanelBox.y, ((tileWidth + 5.0f) * (gridY + 1)), ImDrawFlags_RoundCornersBottomRight))
	{
		NuControls::setActiveComponent(0);
	}
	ImGui::PopStyleColor(4);

	ImGui::PopClipRect();

	return true;
}
static std::string configNameBuf;
bool ClickGuiScreen::drawConfigMenu(class ImDrawList* drawList, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGui::PushClipRect(contentClip.min(), contentClip.max(), true);
	ImGuiIO& io = ImGui::GetIO();

	Vector2<float> titleLabelSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, "Configs");
	Vector2<float> titleLabelPos = {
		contentClip.x + 15.0f,
		contentClip.y + 15.0f
	};
	drawList->AddText(uiFont, 24.0f, titleLabelPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), "Configs");

	Vector4<float> configPanelBox = {
		titleLabelPos.x,
		titleLabelPos.y + titleLabelSize.y + 15.0f,
		contentClip.z - 30.0f,
		contentClip.w - 15.0f
	};
	drawList->AddRectFilled(configPanelBox.min(), configPanelBox.max(), theme[ThemeColor::CLICKGUI_CONFIG_PANEL].asInt(), 8.0f);

	Vector4<float> reloadConfigBox = {
		configPanelBox.z - 40.0f,
		contentClip.y + 15.0f,
		configPanelBox.z,
		contentClip.y + 45.0f
	};
	Vector4<float> addConfigBox = {
		configPanelBox.z - 90.0f,
		contentClip.y + 15.0f,
		configPanelBox.z - 50.0f,
		contentClip.y + 45.0f
	};
	Vector4<float> configNameBox = {
		configPanelBox.z - 350.0f,
		contentClip.y + 15.0f,
		configPanelBox.z - 100.0f,
		contentClip.y + 45.0f
	};

	auto notifs = systems->getModuleManager()->findModule<Notifications>();

	auto configs = systems->getConfigManager();
	ImGui::PushID(&configNameBuf);
	NuControls::drawTextInputControl(drawList, &configNameBuf, "Config name", configNameBox, {});
	ImGui::PopID();
	if (NuControls::drawButtonControl(drawList, theme[ThemeIcon::ADD], addConfigBox) && !configNameBuf.empty())
	{
		auto newConfig = configs->createNew(configNameBuf);
		newConfig->captureFromClient();
		configs->saveToDisk(newConfig);

		notifs->notify("Config created!");
	}
	if (NuControls::drawButtonControl(drawList, theme[ThemeIcon::REFRESH], reloadConfigBox))
	{
		if (configs->reloadAllFromDisk())
		{
			notifs->notify("Configs reloaded!");
		}
		else
		{
			notifs->notify("An error occoured while reloading configs");
		}
	}


	ImGui::PushClipRect(configPanelBox.min(), configPanelBox.max(), true);

	constexpr int tilesPerRow = 3;
	float tileWidth = configPanelBox.size().x / tilesPerRow;
	int gridY = 0;
	for (int i = 0; i < configs->getConfigs().size(); i++)
	{
		const auto config = configs->getConfigs().at(i);
		int gridX = i % tilesPerRow;
		gridY = i / tilesPerRow;

		Vector4<float> tileBox = {
			configPanelBox.x + 5.0f + (tileWidth * gridX),
			configPanelBox.y + 5.0f + (tileWidth * gridY) - scrollAmount,
			(configPanelBox.x - 5.0f) + (tileWidth * (gridX + 1)),
			(configPanelBox.y - 5.0f) + (tileWidth * (gridY + 1)) - scrollAmount
		};
		if (this->drawTileControl(drawList, config->getName(), "By " + config->getAuthor(), tileBox))
		{
			/* Draw hover controls */
			IconGlyph deleteIcon = theme[ThemeIcon::DELETE];

			Vector2<float> padding = { 10.0f, 10.0f };
			Vector2<float> saveLabelSize = padding + uiFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, "Save");
			Vector2<float> loadLabelSize = padding + uiFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, "Load");
			Vector2<float> deleteLabelSize = padding + icoonFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, deleteIcon.glyph.c_str());


			Vector4<float> loadSplit = {
				tileBox.x,
				tileBox.y + (tileBox.size().y / 3.0f),
				tileBox.z,
				tileBox.y + ((tileBox.size().y / 3.0f) * 2)
			};
			Vector4<float> deleteSplit = {
				tileBox.x,
				tileBox.y + ((tileBox.size().y / 3.0f) * 2),
				tileBox.z,
				tileBox.w
			};
			if (config->madeByUser())
			{
				Vector4<float> saveSplit = {
					tileBox.x,
					tileBox.y,
					tileBox.z,
					tileBox.y + (tileBox.size().y / 3.0f)
				};

				Vector4<float> saveButtonBox = {
					saveSplit.center().x - (saveLabelSize.x / 2.0f),
					saveSplit.center().y - (saveLabelSize.y / 2.0f),
					saveSplit.center().x + (saveLabelSize.x / 2.0f),
					saveSplit.center().y + (saveLabelSize.y / 2.0f),
				};
				if (NuControls::drawButtonControl(drawList, "Save", saveButtonBox))
				{
					config->captureFromClient();
					configs->saveToDisk(config);
					notifs->notify("Config saved!");
				}
			}

			Vector4<float> loadButtonBox = {
				loadSplit.center().x - (loadLabelSize.x / 2.0f),
				loadSplit.center().y - (loadLabelSize.y / 2.0f),
				loadSplit.center().x + (loadLabelSize.x / 2.0f),
				loadSplit.center().y + (loadLabelSize.y / 2.0f),
			};
			if (NuControls::drawButtonControl(drawList, "Load", loadButtonBox))
			{
				try
				{
					config->applyToClient();
					notifs->notify("Config loaded!");
				} catch(std::exception& ex)
				{
					notifs->notify("Config load failed! (Error: " + std::string(ex.what()) + ")");
				}
			}

			Vector4<float> deleteButtonBox = {
				deleteSplit.center().x - (deleteLabelSize.x / 2.0f),
				deleteSplit.center().y - (deleteLabelSize.y / 2.0f),
				deleteSplit.center().x + (deleteLabelSize.x / 2.0f),
				deleteSplit.center().y + (deleteLabelSize.y / 2.0f),
			};
			if (NuControls::drawButtonControl(drawList, deleteIcon, deleteButtonBox))
			{
				configs->deleteConfig(config);
				notifs->notify("Config deleted!");
			}
		}
	}
	ImGui::PopClipRect();

	if (configPanelBox.contains(io.MousePos))
	{
		scrollAmount += lastEventScroll;
		lastEventScroll = 0;
	}

	if (scrollAmount <= 0)
		scrollAmount = 0;
	const int64_t scrollMax = std::max(1.0f, ((tileWidth + 5.0f) * (gridY + 1)) - (configPanelBox.w - configPanelBox.y));
	if (scrollMax <= scrollAmount)
		scrollAmount = scrollMax;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiID id = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
	ImRect scrollRect = {
		contentClip.z - 15.0f,
		contentClip.y,
		contentClip.z,
		contentClip.w
	};
	ImGui::GetStyle().WindowRounding = 8.0f;
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, theme[ThemeColor::SCROLLBAR_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme[ThemeColor::SCROLLBAR_SCROLLER_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, theme[ThemeColor::SCROLLBAR_SCROLLER_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, theme[ThemeColor::SCROLLBAR_SCROLLER_MOVING]);
	if (ImGui::ScrollbarEx(scrollRect, id, ImGuiAxis_Y, &scrollAmount, configPanelBox.w - configPanelBox.y, ((tileWidth + 5.0f) * (gridY + 1)), ImDrawFlags_RoundCornersBottomRight))
	{
		NuControls::setActiveComponent(0);
	}
	ImGui::PopStyleColor(4);

	ImGui::PopClipRect();

	return true;
}
bool ClickGuiScreen::drawSettingsPageSelector(ImDrawList* drawList, std::string title, int* pageId, const std::vector<std::string>& pages, Vector4<float> contentClip)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiIO& io = ImGui::GetIO();

	float fontSize = 20.0f;
	float margin = 10.0f;
	float padding = 10.0f;
	Vector4<float> panelBox = {
		contentClip.x + margin,
		contentClip.y + margin,
		contentClip.z - margin,
		contentClip.y + ((pages.size()) * fontSize) + ((pages.size() + 1) * padding)
	};
	Vector2<float> drawPos = panelBox.min() + padding;

	ColorF color = theme[ThemeColor::SEGMENTED_COLOR];
	ColorF hovered = theme[ThemeColor::SEGMENTED_SECONDARY];
	ColorF active = theme[ThemeColor::SEGMENTED_SELECTED];
	ColorF text = theme[ThemeColor::SEGMENTED_TEXT];
	ColorF textActive = theme[ThemeColor::SEGMENTED_TEXT_SELECTED];
	ColorF seperator = theme[ThemeColor::SEGMENTED_SECONDARY];

	drawList->AddRectFilled(panelBox.min(), panelBox.max(), color.asInt(), 4.0f);
	for (auto it = pages.begin(); it != pages.end(); it++)
	{
		auto index = std::distance(pages.begin(), it);
		Vector4<float> selectionRegion = {
			panelBox.x,
			panelBox.y + (index * fontSize) + (index * padding),
			panelBox.z,
			panelBox.y + ((index + 1) * fontSize) + ((index + 1) * padding)
		};
		bool hovered = ImGui::IsMouseHoveringRect(selectionRegion.min(), selectionRegion.max(), true);
		if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			*pageId = index;
		}
		if (hovered)
		{
			ImDrawFlags flags = ImDrawFlags_RoundCornersNone;
			if (it == pages.begin())
			{
				flags = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight;
			}
			else if (std::distance(it, pages.end()) == 1)
			{
				flags = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight;
			}
			drawList->AddRectFilled(selectionRegion.min(), selectionRegion.max(), active.asInt(), 4.0f, flags);
		}

		if (std::distance(it, pages.end()) > 1)
		{
			Vector4<float> seperatorLine = {
				selectionRegion.min().x,
				selectionRegion.max().y,
				selectionRegion.max().x,
				selectionRegion.max().y
			};
			drawList->AddLine(seperatorLine.min(), seperatorLine.max(), seperator.asInt());
		}

		Vector2<float> textSize = uiFont->CalcTextSizeA(fontSize, selectionRegion.size().x, 0.0f, it->c_str());
		Vector2<float> textPos = {
			selectionRegion.x + padding,
			selectionRegion.center().y - (textSize.y / 2.0f)
		};

		ColorF textCol = hovered ? textActive : text;
		drawList->AddText(uiFont, fontSize, textPos, textCol.asInt(), it->c_str());

		IconGlyph nextIcon = theme[ThemeIcon::NEXT];
		float iconSize = nextIcon.size.has_value() ? *nextIcon.size : fontSize;
		Vector2<float> measuredSize = icoonFont->CalcTextSizeA(iconSize, FLT_MAX, 0.0f, nextIcon.glyph.c_str());
		Vector4<float> iconBox = {
			selectionRegion.z - (measuredSize.x + padding),
			selectionRegion.center().y - (measuredSize.y / 2.0f),
			selectionRegion.z - padding,
			selectionRegion.center().y + (measuredSize.y / 2.0f)
		};
		drawList->AddText(icoonFont, iconSize, iconBox.min() + Vector2<float>(nextIcon.offsetX, nextIcon.offsetY), hovered ? textCol.asInt() : seperator.asInt(), nextIcon.glyph.c_str());

		drawPos += { 0.0f, fontSize };
	}

	return *pageId != -1;
}
static std::vector<std::string> pages = { "Client Settings", "Configs", "Themes" };
bool ClickGuiScreen::drawSettingsMenu(ImDrawList* drawList, Vector4<float> contentClip)
{
	ImGui::PushClipRect(contentClip.min(), contentClip.max(), true);

	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();

	ColorF text = theme[ThemeColor::CLICKGUI_TEXT_COLOR];

	if (settingsPage == -1)
	{
		float padding = 10.0f;
		Vector2<float> labelPos = contentClip.min() + padding;
		drawList->AddText(labelPos, text.asInt(), "Settings");

		Vector4<float> selectorArea = {
			contentClip.x,
			contentClip.y + 35.0f,
			contentClip.z,
			contentClip.w
		};

		this->drawSettingsPageSelector(drawList, "General", &settingsPage, pages, selectorArea);
	}
	else
	{
		ImGui::PushID("SettingsSubbmenu");
		switch (settingsPage)
		{
		case 0:
			this->drawPreferencesMenu(drawList, contentClip);
			break;
		case 1:
			this->drawConfigMenu(drawList, contentClip);
			break;
		case 2:
			this->drawThemeMenu(drawList, contentClip);
			break;
		}
		ImGui::PopID();
	}
	

	ImGui::PopClipRect();

	return true;
}
bool ClickGuiScreen::drawMenuControl(ImDrawList* drawList, ThemeIcon icon, Vector4<float> controlBox)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return false;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return false;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return false;
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImGuiIO& io = ImGui::GetIO();

	ColorF buttonColor = theme[ThemeColor::BUTTON_COLOR];
	ColorF buttonHovered = theme[ThemeColor::BUTTON_HOVERED];
	ColorF buttonSelected = theme[ThemeColor::BUTTON_ACTIVE];
	ColorF iconColor = theme[ThemeColor::CLICKGUI_TEXT_COLOR];
	const IconGlyph& iconGlyph = theme[icon];

	bool hovered = controlBox.contains(io.MousePos);
	bool clicked = hovered ? io.MouseDown[ImGuiMouseButton_Left] : false;

	if (clicked)
	{
		activeMenu = id;
		settingsPage = -1;
	}

	if (clicked || activeMenu == id)
	{
		iconColor = theme[ThemeColor::CLICKGUI_TEXT_ACTIVE];
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonSelected.asInt(), 4.0f);
	}
	else if (hovered)
	{
		iconColor = theme[ThemeColor::CLICKGUI_TEXT_HOVERED];
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonHovered.asInt(), 4.0f);
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonColor.asInt(), 4.0f);
	}

	Vector2<float> iconSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, iconGlyph.glyph.c_str());
	Vector2<float> iconPos = {
		controlBox.x + (iconSize.x / 2.0f) + iconGlyph.offsetX,
		controlBox.y + (iconSize.y / 2.0f) + iconGlyph.offsetY
	};
	drawList->AddText(icoonFont, 16.0f, iconPos, iconColor.asInt(), iconGlyph.glyph.c_str());

	return activeMenu == id;
}

void ClickGuiScreen::initialize(ScreenData* data)
{
	//ImGui::PushID("ConfigMenu");
	//Vector4<float> configBox = {
	//	titleBarRect.z - 100,
	//	titleBarRect.center().y - (titleBarRect.size().y / 3.0f),
	//	titleBarRect.z - 60,
	//	titleBarRect.center().y + (titleBarRect.size().y / 3.0f),
	//};
	//if (this->drawMenuControl(drawList, ThemeIcon::CONFIG, configBox))
	//{
	//	activeCategory = nullptr;
	//	searchQuery.clear();
	//	//Draw config menu
	//	this->drawConfigMenu(drawList, contentArea);
	//}
	//ImGui::PopID();
	//ImGui::PushID("ThemeMenu");
	//Vector4<float> themeBox = {
	//	titleBarRect.z - 150,
	//	titleBarRect.center().y - (titleBarRect.size().y / 3.0f),
	//	titleBarRect.z - 110,
	//	titleBarRect.center().y + (titleBarRect.size().y / 3.0f),
	//};
	//if (this->drawMenuControl(drawList, ThemeIcon::THEME, themeBox))
	//{
	//	activeCategory = nullptr;
	//	searchQuery.clear();
	//	//Draw theme menu
	//	this->drawThemeMenu(drawList, contentArea);
	//}
	//ImGui::PopID();
}

void ClickGuiScreen::renderWindowContent(float deltaTime)
{
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return;
	ImFont* icoonFont = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (icoonFont == nullptr)
		return;

	//Draw titlebar & sidebar
	Vector4<float> titleBarRect = {
		ImGui::GetWindowPos().x + 4.0f,
		ImGui::GetWindowPos().y + 1.0f,
		(ImGui::GetWindowPos().x + guiSize.x) - 4.0f,
		ImGui::GetWindowPos().y + 60.0f
	};
	Vector4<float> sideBarRect = {
		ImGui::GetWindowPos().x + 4.0f,
		ImGui::GetWindowPos().y + 60.0f,
		ImGui::GetWindowPos().x + 230,
		ImGui::GetWindowPos().y + guiSize.y - 1.0f
	};
	Vector4<float> contentArea = {
		sideBarRect.z,
		titleBarRect.w,
		ImGui::GetWindowPos().x + guiSize.x - 4.0f,
		ImGui::GetWindowPos().y + guiSize.y - 1.0f
	};
	Vector4<float> scrollRectOffset = {
		0,
		0,
		15.0f,
		0
	};

	//Get the module manager instance
	const auto modMgr = systems->getModuleManager();
	auto screenMgr = systems->getScreenManager();
	std::shared_ptr<ClickGui> clickGui = systems->getModuleManager()->findModule<ClickGui>();

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ColorF clickGuiBg = theme[ThemeColor::CLICKGUI_COLOR];
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	//Add an inactive widget to prevent window dragging
	bool hover, held;
	ImGui::ButtonBehavior(ImRect(contentArea - scrollRectOffset), ImGui::GetID("###NOMOVEID"), &hover, &held);

	ImFX::Begin(drawList);
	ImFX::AddColor(theme[ThemeColor::CLICKGUI_TITLEBAR_COLOR], ImVec4(8.0f, 8.0f, 0.0f, 0.0f), titleBarRect);
	ImFX::AddColor(theme[ThemeColor::CLICKGUI_SIDEBAR_COLOR], ImVec4(0.0f, 0.0f, 0.0f, 8.0f), sideBarRect);
	ImFX::AddColor(theme[ThemeColor::CLICKGUI_CONTENTPANEL_COLOR], ImVec4(0.0f, 0.0f, 8.0f, 0.0f), contentArea);
	//Draw mascot
	if (theme.hasTexture(0))
	{
		Vector2<int> mascotSize = { 150, 150 };
		Vector4<float> mascotRect = {
			sideBarRect.center().x - (mascotSize.x / 2),
			sideBarRect.w - mascotSize.y,
			sideBarRect.center().x + (mascotSize.x / 2),
			sideBarRect.w
		};

		//ImFX::AddColor(Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), mascotRect);
		ID2D1Bitmap1* texture = theme.getTextureAs<ID2D1Bitmap1*>(0);
		ImFX::DrawImage(texture, mascotRect, 0.0f);
	}
	ImFX::End();

	//Draw window title
	constexpr std::string title = "Nuvola";
	Vector2<float> textSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, title.c_str());
	Vector2<float> textPos = {
		titleBarRect.x + 30.0f,//(((titleBarRect.z - titleBarRect.x) / 2) - (textSize.x / 2)),
		titleBarRect.y + (((titleBarRect.w - titleBarRect.y) / 2) - (textSize.y / 2))
	};
	drawList->AddText(uiFont, 24.0f, textPos, theme[ThemeColor::CLICKGUI_BRAND_COLOR].asInt(), title.c_str());

	//Draw settings button
	ImGui::PushID("SettingsMenu");
	Vector4<float> settingsBox = {
		titleBarRect.z - 100,
		titleBarRect.center().y - (titleBarRect.size().y / 3.0f),
		titleBarRect.z - 60,
		titleBarRect.center().y + (titleBarRect.size().y / 3.0f),
	};
	if (this->drawMenuControl(drawList, ThemeIcon::SETTINGS, settingsBox))
	{
		activeCategory = nullptr;
		searchQuery.clear();
		//Draw settings menu
		this->drawSettingsMenu(drawList, contentArea);
	}
	ImGui::PopID();
	

	//Draw search bar
	ImGui::PushID(&searchQuery);
	constexpr int searchbarWidth = 300;
	Vector4<float> searchBox = {
		titleBarRect.center().x - (searchbarWidth / 2),
		titleBarRect.center().y - (titleBarRect.size().y / 4.0f),
		titleBarRect.center().x + (searchbarWidth / 2),
		titleBarRect.center().y + (titleBarRect.size().y / 4.0f)
	};
	NuControls::drawTextInputControl(drawList, &searchQuery, "Search", searchBox, theme[ThemeIcon::SEARCH].glyph);
	ImGui::PopID();

	if (!searchQuery.empty())
	{
		activeCategory = nullptr;
		activeMenu = 0;

		this->drawCategoryContent(drawList, nullptr, contentArea);
	}
	//Draw category selection
	///const char* categoryIcoons[] = { ICON_FA_HAND_ROCK, ICON_FA_CUBE, ICON_FA_WALKING, ICON_FA_USER, ICON_FA_WRENCH };
	ImVec2 categoryDrawPos = {
		sideBarRect.x + 60.0f,
		sideBarRect.y + 15.0f
	};
	for (int i = 0; i < modMgr->getItems().size(); i++)
	{
		const auto& cat = modMgr->getItem(i);

		if (this->drawCategoryControl(drawList, cat, categoryDrawPos))
		{
			this->drawCategoryContent(drawList, cat, contentArea);
		}

		categoryDrawPos.y += textSize.y + 15.0f;
	}
}

void ClickGuiScreen::render(float deltaTime)
{
	//Change theme if necessary
	if (nextThemeIndex != -1)
	{
        auto themes = systems->getThemeManager();
		std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
		const auto& allThemes = themes->getThemes();
		if (allThemes.size() > 0)
		{
			themes->setActiveTheme(allThemes.at(nextThemeIndex));
			notifs->notify("Theme loaded!");
		}
		nextThemeIndex = -1;
	}

	if (this->windowPos.has_value())
	{
		ImGui::SetNextWindowPos(*this->windowPos);
		this->windowPos.reset();
	}

	WindowScreen::render(deltaTime);
}

void ClickGuiScreen::cleanup()
{
	this->closeBehavior();
}

void ClickGuiScreen::handleInput(KeyEvent& event)
{
	if (event.getAction() != KeyAction::PRESSED) {
		return;
	}

	std::shared_ptr<ClickGui> clickGui = systems->getModuleManager()->findModule<ClickGui>();

	bool isChangingKey = false;
	for (auto& [mod, changing] : changingKey)
	{
		if (changing)
		{
			isChangingKey = true;
			break;
		}
	}
	if (!isChangingKey && NuControls::getActiveComponent() == 0)
	{
		if (event.getKey() == clickGui->getHotkey() || event.getKey() == VK_ESCAPE)
		{
			this->closeBehavior();
		}
	}
	for (auto& [mod, changing] : changingKey) {
		if (changing) {
			if (event.getKey() == VK_ESCAPE) {
				mod->setHotkey(0);
			}
			else {
				mod->setHotkey(event.getKey());
			}
			changingKey[mod] = false;
			NuControls::setActiveComponent(0);
		}
	}
	event.cancel();
}

void ClickGuiScreen::handleInput(MouseEvent& event)
{
	if (event.getAction() == MouseAction::SCROLL_UP)
	{
		lastEventScroll = -30.0f;
	}
	if (event.getAction() == MouseAction::SCROLL_DOWN)
	{
		lastEventScroll = 30.0f;
	}
}

void ClickGuiScreen::closeBehavior()
{
	std::shared_ptr<ClickGui> clickGui = systems->getModuleManager()->findModule<ClickGui>();
	if (clickGui != nullptr)
	{
		clickGui->setEnabled(false);
	}
}
