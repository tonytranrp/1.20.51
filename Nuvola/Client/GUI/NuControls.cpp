#include "NuControls.h"

#include "../Features/Theme/ThemeManager.h"
#include "../Features/Theme/Theme.h"

#include <Fonts/FontRegistry.h>
#include <Graphics/imfx.h>

#include <imgui.h>
#include <imgui_custom.h>
#include <imgui_internal.h>

#include <Windows.h>

#undef min
#undef max

static std::map<ImGuiID, bool> collapsed;
static std::map<ImGuiID, float> animProgs;
static ImGuiID activeComponent = 0;

constexpr float sliderThiccness = 2.0f;
constexpr float animationSpeed = 8.0f;

void NuControls::Internal::drawCheckmark(ImDrawList* drawList, TriState* state, Vector4<float> controlBox, float thickness)
{
	if (*state == TriState::OFF)
	{
		return;
	}

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ColorF checkCol = theme[ThemeColor::CHECKBOX_CHECK];

	float padding = 5.0f;
	static std::vector<Vector2<float>> vertices;
	vertices.clear();
	if (*state == TriState::ON)
	{
		vertices.emplace_back(
			controlBox.x + padding,
			controlBox.center().y
		);
		vertices.emplace_back(
			controlBox.center().x - (controlBox.size().x / 3.5f) + padding,
			controlBox.w - (padding * 1.5f)
		);
		vertices.emplace_back(
			controlBox.z - (padding * 1.2f),
			controlBox.y + (padding * 1.2f)
		);
	}
	else
	{
		vertices.emplace_back(
			controlBox.x + padding,
			controlBox.center().y
		);
		vertices.emplace_back(
			controlBox.z - padding,
			controlBox.center().y
		);
	}

	for (Vector2<float> vert : vertices)
	{
		drawList->PathLineTo(vert);
	}
	drawList->PathStroke(checkCol.asInt(), 0, thickness);
}

ImGuiID NuControls::getActiveComponent()
{
	return activeComponent;
}
void NuControls::setActiveComponent(ImGuiID id)
{
	activeComponent = id;
}

bool NuControls::drawButtonControl(ImDrawList* drawList, IconGlyph icon, Vector4<float> controlBox)
{
	ImFont* font = FontRegistry::getFont(FontRegistry::FontID::SOLID_ICOONS);
	if (font == nullptr)
		return false;

	Theme theme = systems->getThemeManager()->getActiveTheme();

	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImGuiIO& io = ImGui::GetIO();

	ColorF buttonColor = theme[ThemeColor::BUTTON_COLOR];
	ColorF buttonHovered = theme[ThemeColor::BUTTON_HOVERED];
	ColorF buttonActive = theme[ThemeColor::BUTTON_ACTIVE];
	bool hovered = controlBox.contains(io.MousePos);
	bool clicked = hovered ? io.MouseClicked[ImGuiMouseButton_Left] : false;
	bool active = hovered ? io.MouseDown[ImGuiMouseButton_Left] : false;

	if (hovered)
	{
		if (active)
		{
			drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonActive.asInt(), 4.0f);
		}
		else
		{
			drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonHovered.asInt(), 4.0f);
		}
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonColor.asInt(), 4.0f);
	}

	float size = icon.size.has_value() ? *icon.size : 20.0f;
	Vector2<float> labelSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, icon.glyph.c_str());
	Vector2<float> labelPos = {
		controlBox.center().x - (labelSize.x / 2.0f),
		controlBox.center().y - (labelSize.y / 2.0f)
	};
	drawList->AddText(font, size, labelPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), icon.glyph.c_str());

	return clicked;
};
bool NuControls::drawButtonControl(ImDrawList* drawList, std::string text, Vector4<float> controlBox)
{
	ImFont* font = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (font == nullptr)
		return false;

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImGuiIO& io = ImGui::GetIO();

	ColorF buttonColor = theme[ThemeColor::BUTTON_COLOR];
	ColorF buttonHovered = theme[ThemeColor::BUTTON_HOVERED];
	ColorF buttonActive = theme[ThemeColor::BUTTON_ACTIVE];
	bool hovered = controlBox.contains(io.MousePos);
	bool clicked = hovered ? io.MouseClicked[ImGuiMouseButton_Left] : false;
	bool active = hovered ? io.MouseDown[ImGuiMouseButton_Left] : false;

	Vector2<float> labelSize = font->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, text.c_str());
	Vector2<float> labelPos = {
		controlBox.center().x - (labelSize.x / 2.0f),
		controlBox.center().y - (labelSize.y / 2.0f)
	};

	if (hovered)
	{
		if (active)
		{
			drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonActive.asInt(), 4.0f);
			drawList->AddText(font, 20.0f, labelPos, theme[ThemeColor::CLICKGUI_TEXT_ACTIVE].asInt(), text.c_str());
		}
		else
		{
			drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonHovered.asInt(), 4.0f);
			drawList->AddText(font, 20.0f, labelPos, theme[ThemeColor::CLICKGUI_TEXT_HOVERED].asInt(), text.c_str());
		}
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), buttonColor.asInt(), 4.0f);
		drawList->AddText(font, 20.0f, labelPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), text.c_str());
	}

	return clicked;
}
bool NuControls::drawComboControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox)
{
	if (selection == nullptr)
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

	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::COMBO_HOVERED].asInt(), 4.0f);
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			if (activeComponent != id)
				activeComponent = id;
			else
				activeComponent = 0;
		}
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::COMBO_COLOR].asInt(), 4.0f);
	}

	//Draw dropdown ico
	const std::string& dropdownIco = theme[ThemeIcon::COMBO_OPEN].glyph;
	Vector2<float> dropdownIcoSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, dropdownIco.c_str());
	Vector2<float> dropdownBoxSize = {
		controlBox.size().y,
		controlBox.size().y
	};
	Vector4<float> dropdownBox = {
		(controlBox.max().x - dropdownBoxSize.x) + 2.0f,
		(controlBox.max().y - dropdownBoxSize.y) + 2.0f,
		controlBox.max().x - 2.0f,
		controlBox.max().y - 2.0f
	};
	Vector2<float> dropdownIcoLoc = {
		dropdownBox.center().x - (dropdownIcoSize.x / 2.0f),
		dropdownBox.center().y - (dropdownIcoSize.y / 2.0f)
	};
	drawList->AddRectFilled(dropdownBox.min(), dropdownBox.max(), theme[ThemeColor::COMBO_DROPDOWN_COLOR].asInt(), 4.0f);
	drawList->AddText(icoonFont, 16.0f, dropdownIcoLoc, theme[ThemeColor::COMBO_TEXT].asInt(), dropdownIco.c_str());

	//Draw selected item text
	Vector2<float> labelSize = uiFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, values[*selection].c_str());
	Vector2<float> labelLoc = {
		controlBox.x + 5.0f,
		controlBox.center().y - (labelSize.y / 2.0f)
	};
	drawList->AddText(uiFont, 16.0f, labelLoc, theme[ThemeColor::COMBO_TEXT].asInt(), values[*selection].c_str());

	bool newSelected = false;
	if (activeComponent == id)
	{
		Vector4<float> dropdownOverlay = {
			controlBox.x,
			controlBox.w,
			controlBox.z,
			controlBox.w + 5.0f
		};
		for (const std::string& option : values)
		{
			Vector2<float> textSize = uiFont->CalcTextSizeA(16.0f, controlBox.size().x, controlBox.size().x + 10.0f, option.c_str());
			dropdownOverlay.w += textSize.y + 5.0f;
		}
		ImDrawList* overlayDrawlist = ImGui::GetForegroundDrawList();
		ImFX::Begin(overlayDrawlist);
		ImFX::AddBlur(16.0f, dropdownOverlay);
		ImFX::End();
		overlayDrawlist->AddRectFilled(dropdownOverlay.min(), dropdownOverlay.max(), theme[ThemeColor::COMBO_BACKGROUND].asInt(), 4.0f);
		Vector1<float> drawY = controlBox.w + 5.0f;
		int index = 0;
		for (const std::string& option : values)
		{
			Vector2<float> textSize = uiFont->CalcTextSizeA(16.0f, controlBox.size().x, controlBox.size().x + 10.0f, option.c_str());
			Vector2<float> drawPos = {
				controlBox.x + 5.0f,
				drawY.x
			};
			Vector4<float> selectionRect = {
				drawPos.x - 2.0f,
				drawPos.y - 2.0f,
				controlBox.z - 3.0f,
				drawPos.y + textSize.y + 2.0f
			};
			/* Gotta use imgui io for this because the dropdown can drop outside of the window */
			if (selectionRect.contains(io.MousePos))
			{
				overlayDrawlist->AddRectFilled(selectionRect.min(), selectionRect.max(), theme[ThemeColor::COMBO_ITEM_HOVERED].asInt(), 4.0f);
				if (io.MouseClicked[ImGuiMouseButton_Left])
				{
					*selection = index;
					activeComponent = 0;
					newSelected = true;
				}
			}
			if (*selection == index)
			{
				overlayDrawlist->AddRectFilled(selectionRect.min(), selectionRect.max(), theme[ThemeColor::COMBO_ITEM_SELECTED].asInt(), 4.0f);
			}
			overlayDrawlist->AddText(uiFont, 16.0f, drawPos, theme[ThemeColor::COMBO_TEXT].asInt(), option.c_str());
			drawY += textSize.y + 5.0f;
			index++;
		}
	}

	return newSelected;
}
bool NuControls::drawSegmentedControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox)
{
	if (selection == nullptr)
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

	drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::SEGMENTED_COLOR].asInt(), 4.0f);

	float tickDistance = controlBox.size().x / static_cast<float>(values.size());
	for (int i = 0; i < values.size(); i++)
	{
		ColorF textColor = theme[ThemeColor::SEGMENTED_TEXT];
		Vector4<float> selectionBox = {
				controlBox.x + (tickDistance * i),
				controlBox.y + 1.0f,
				controlBox.x + (tickDistance * (i + 1)),
				controlBox.w - 1.0f
		};
		if (ImGui::IsMouseHoveringRect(selectionBox.min(), selectionBox.max()))
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				*selection = i;
			}
		}

		if (i == *selection)
		{
			textColor = theme[ThemeColor::SEGMENTED_TEXT_SELECTED];
			drawList->AddRectFilled(selectionBox.min(), selectionBox.max(), theme[ThemeColor::SEGMENTED_SELECTED].asInt(), 4.0f);
			drawList->AddShadowRect(selectionBox.min(), selectionBox.max(), theme[ThemeColor::SEGMENTED_SHADOW].asInt(), 12.0f, { 0,0 }, 0, 4.0f);
		}
		else
		{
			Vector4<float> tickBox = {
				controlBox.x + (tickDistance * i),
				controlBox.center().y + (controlBox.size().y / 4.0f),
				controlBox.x + (tickDistance * i) + 1.0f,
				controlBox.center().y - (controlBox.size().y / 4.0f),
			};

			if (i > 0 && *selection == i + 1)
				drawList->AddRectFilled(tickBox.min(), tickBox.max(), theme[ThemeColor::SEGMENTED_SECONDARY].asInt());
			if (*selection == 0 && i > 1)
				drawList->AddRectFilled(tickBox.min(), tickBox.max(), theme[ThemeColor::SEGMENTED_SECONDARY].asInt());
		}

		Vector2<float> labelSize = uiFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, values[i].c_str());
		Vector2<float> labelPos = {
			selectionBox.center().x - (labelSize.x / 2.0f),
			selectionBox.center().y - (labelSize.y / 2.0f)
		};
		drawList->AddText(uiFont, 16.0f, labelPos, textColor.asInt(), values[i].c_str());
	}
	return true;
}
bool NuControls::drawOptimalEnumControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox)
{
	if (selection == nullptr)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}
	if (values.size() <= 3)
	{
		/* Draw segmented control */
		return NuControls::drawSegmentedControl(drawList, selection, values, controlBox);
	}
	else
	{
		/* Draw combo box */
		return NuControls::drawComboControl(drawList, selection, values, controlBox);
	}

	return false;
}
bool NuControls::drawSliderIntControl(ImDrawList* drawList, int* val, int min, int max, Vector4<float> controlBox)
{
	if (val == nullptr)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}
	Theme theme = systems->getThemeManager()->getActiveTheme();
	Vector2<float> size = controlBox.size();
	int numIntervals = (max - min);
	bool drawTicks = numIntervals <= 10;
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();

	ColorF notchColor = theme[ThemeColor::SLIDER_NOTCH];

	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		notchColor = theme[ThemeColor::SLIDER_NOTCH_HOVERED];
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && activeComponent == 0)
		{
			activeComponent = id;
		}
	}

	if (activeComponent == id)
	{
		float progPixel = ImGui::GetMousePos().x;
		float prog = (progPixel - controlBox.x) / size.x;
		if (progPixel >= controlBox.z)
			prog = 1.0f;
		if (progPixel <= controlBox.x)
			prog = 0.0f;
		*val = std::roundf(static_cast<float>(numIntervals) * prog) + min;

		notchColor = theme[ThemeColor::SLIDER_NOTCH_MOVING];

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			activeComponent = 0;
		}
	}

	float prog = (*val - min) / static_cast<float>(numIntervals);
	float progPixel = controlBox.x + (size.x * prog);

	Vector4<float> sliderBox = {
		controlBox.x,
		controlBox.center().y - sliderThiccness,
		controlBox.z,
		controlBox.center().y + sliderThiccness
	};
	Vector4<float> sliderSlid = {
		controlBox.x,
		controlBox.center().y - sliderThiccness,
		progPixel,
		controlBox.center().y + sliderThiccness
	};
	Vector2<float> sliderNotchPos = {
		progPixel,
		controlBox.center().y
	};

	drawList->AddRectFilled(sliderBox.min(), sliderBox.max(), theme[ThemeColor::SLIDER_COLOR].asInt(), 32.0f);
	if (!drawTicks)
	{
		drawList->AddRectFilled(sliderSlid.min(), sliderSlid.max(), theme[ThemeColor::SLIDER_COLOR_SLID].asInt(), 32.0f);
	}
	else
	{
		for (int i = 0; i <= numIntervals; i++)
		{
			float progOffset = static_cast<float>(i) / static_cast<float>(numIntervals);
			float offsetPixel = controlBox.x + (size.x * progOffset);
			Vector4<float> sliderTick = {
				offsetPixel - sliderThiccness,
				controlBox.center().y - (sliderThiccness * 2.0f),
				offsetPixel + sliderThiccness,
				controlBox.center().y + (sliderThiccness * 2.0f)
			};
			drawList->AddRectFilled(sliderTick.min(), sliderTick.max(), theme[ThemeColor::SLIDER_COLOR].asInt());
		}

	}
	drawList->AddCircleFilled(sliderNotchPos, size.y / 3.0f, notchColor.asInt());

	return true;
}
bool NuControls::drawSliderFloatControl(ImDrawList* drawList, float* val, float min, float max, Vector4<float> controlBox)
{
	if (val == nullptr)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}
	Theme theme = systems->getThemeManager()->getActiveTheme();
	Vector2<float> size = controlBox.size();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ColorF notchColor = theme[ThemeColor::SLIDER_NOTCH];

	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		notchColor = theme[ThemeColor::SLIDER_NOTCH_HOVERED];
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && activeComponent == 0)
		{
			activeComponent = id;
		}
	}

	if (activeComponent == id)
	{
		float progPixel = ImGui::GetMousePos().x;
		float prog = (progPixel - controlBox.x) / size.x;
		if (progPixel >= controlBox.z)
			prog = 1.0f;
		if (progPixel <= controlBox.x)
			prog = 0.0f;
		*val = ((max - min) * prog) + min;

		notchColor = theme[ThemeColor::SLIDER_NOTCH_MOVING];

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			activeComponent = 0;
		}
	}

	float prog = (*val - min) / (max - min);
	float progPixel = controlBox.x + (size.x * prog);

	Vector4<float> sliderBox = {
		controlBox.x,
		controlBox.center().y - sliderThiccness,
		controlBox.z,
		controlBox.center().y + sliderThiccness
	};
	Vector4<float> sliderSlid = {
		controlBox.x,
		controlBox.center().y - sliderThiccness,
		progPixel,
		controlBox.center().y + sliderThiccness
	};
	Vector2<float> sliderNotchPos = {
		progPixel,
		controlBox.center().y
	};

	drawList->AddShadowCircle(sliderNotchPos, size.y / 3.0f, theme[ThemeColor::SLIDER_NOTCH_SHADOW].asInt(), 12.0f, ImVec2(0.0f, 0.0f));
	drawList->AddRectFilled(sliderBox.min(), sliderBox.max(), theme[ThemeColor::SLIDER_COLOR].asInt(), 32.0f);
	drawList->AddRectFilled(sliderSlid.min(), sliderSlid.max(), theme[ThemeColor::SLIDER_COLOR_SLID].asInt(), 32.0f);
	drawList->AddCircleFilled(sliderNotchPos, size.y / 3.0f, notchColor.asInt());

	return activeComponent == id;
}
bool NuControls::drawToggleControl(ImDrawList* drawList, bool* val, Vector4<float> controlBox)
{
	if (val == nullptr)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}

	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		if (activeComponent == 0)
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				*val = !*val;
			}
		}
	}

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImGuiIO& io = ImGui::GetIO();

	if (*val)
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
		}
	}

	Vector2<float> size = {
		controlBox.z - controlBox.x,
		controlBox.w - controlBox.y
	};

	ColorF onColor = theme[ThemeColor::TOGGLE_ON];
	Vector4<float> notchRectOff = {
			controlBox.x,
			controlBox.y,
			controlBox.x + (size.x / 2.0f),
			controlBox.w

	};
	Vector2<float> notchPosOff = notchRectOff.center() + Vector2<float>(2, 0);
	float notchRadius = notchRectOff.size().min().x * 0.5f;

	ColorF offColor = theme[ThemeColor::TOGGLE_OFF];
	Vector4<float> notchRectOn = {
			controlBox.x + (size.x / 2.0f),
			controlBox.y,
			controlBox.z,
			controlBox.w
	};
	Vector2<float> notchPosOn = notchRectOn.center() - Vector2<float>(2, 0);

	drawList->AddRectFilled(controlBox.min(), controlBox.max(), offColor.interpolate(onColor, animProgs[id]).asInt(), 32.0f);
	Vector2<float> notchPos = notchPosOff.interpolate(notchPosOn, animProgs[id]);

	drawList->AddShadowCircle(notchPos, notchRadius, theme[ThemeColor::TOGGLE_NOTCH_SHADOW].asInt(), 12.0f, ImVec2(0.0f, 0.0f));
	drawList->AddCircleFilled(notchPos, notchRadius, theme[ThemeColor::TOGGLE_NOTCH].asInt());
	return true;
}
static bool cursorFlip = false;
bool NuControls::drawTextInputControl(ImDrawList* drawList, std::string* val, std::string hint, Vector4<float> controlBox, std::optional<std::string> prefixIcon)
{
	if (val == nullptr)
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
	ImGuiContext* ctx = ImGui::GetCurrentContext();

	ColorF boxColor = theme[ThemeColor::TEXTINPUT_COLOR];
	ColorF cursorColor = theme[ThemeColor::TEXTINPUT_CURSOR];

	if (activeComponent == id)
	{
		if (cursorFlip)
		{
			animProgs[ImGui::GetID("TEXT_CURSOR")] += io.DeltaTime * animationSpeed * 0.5f;
			if (animProgs[ImGui::GetID("TEXT_CURSOR")] >= 1.0f)
			{
				animProgs[ImGui::GetID("TEXT_CURSOR")] = 1.0f;
				cursorFlip = false;
			}
		}
		else
		{
			animProgs[ImGui::GetID("TEXT_CURSOR")] -= io.DeltaTime * animationSpeed * 0.5f;
			if (animProgs[ImGui::GetID("TEXT_CURSOR")] <= 0.0f)
			{
				animProgs[ImGui::GetID("TEXT_CURSOR")] = 0.0f;
				cursorFlip = true;
			}
		}
		cursorColor.w = animProgs[ImGui::GetID("TEXT_CURSOR")];
	}

	if (controlBox.contains(io.MousePos))
	{
		boxColor = theme[ThemeColor::TEXTINPUT_HOVERED];

		if (io.MouseClicked[ImGuiMouseButton_Left])
		{
			io.InputQueueCharacters.clear();
			activeComponent = id;
		}
	}
	else
	{
		if (io.MouseClicked[ImGuiMouseButton_Left] && activeComponent == id)
		{
			activeComponent = 0;
		}
	}

	if (activeComponent == id)
	{
		for (const unsigned short key : io.InputQueueCharacters)
		{
			if (key == VK_BACK)
			{
				animProgs[ImGui::GetID("TEXT_CURSOR")] = 1.0f;
				cursorColor.w = 1.0f;
				*val = val->substr(0, val->size() - 1);
				continue;
			}

			char c = static_cast<char>(key);
			if (isprint(c))
			{
				animProgs[ImGui::GetID("TEXT_CURSOR")] = 1.0f;
				cursorColor.w = 1.0f;
				*val += c;
			}
		}
	}

	drawList->AddRectFilled(controlBox.min(), controlBox.max(), boxColor.asInt(), 4.0f);

	ColorF textColor = (val->empty() && activeComponent != id) ? theme[ThemeColor::TEXTINPUT_HINT] : theme[ThemeColor::TEXTINPUT_TEXT];
	const std::string* toRender = (val->empty() && activeComponent != id) ? &hint : val;
	Vector2<float> textSize = uiFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, toRender->c_str());
	Vector2<float> textPos = {
		controlBox.x + 10.0f,
		controlBox.center().y - (textSize.y / 2.0f)
	};
	if (prefixIcon.has_value())
	{
		Vector2<float> iconSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, prefixIcon->c_str());
		Vector2<float> iconPos = {
			controlBox.x + 10.0f,
			controlBox.center().y - (iconSize.y / 2.0f)
		};
		textPos.x += iconSize.x + 10.0f;

		drawList->AddText(icoonFont, 16.0f, iconPos, theme[ThemeColor::TEXTINPUT_HINT].asInt(), prefixIcon->c_str());
	}

	drawList->AddText(uiFont, 16.0f, textPos, textColor.asInt(), toRender->c_str());

	if (activeComponent == id)
	{
		if (!val->empty())
		{
			Vector2<float> iconSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, theme[ThemeIcon::CLEAR].glyph.c_str());
			Vector2<float> iconPos = {
				controlBox.z - (iconSize.x + 10.0f),
				controlBox.center().y - (iconSize.y / 2.0f)
			};
			Vector4<float> iconBox = {
				iconPos,
				iconPos + iconSize
			};

			if (iconBox.contains(io.MousePos))
			{
				if (io.MouseClicked[ImGuiMouseButton_Left])
				{
					val->clear();
				}
			}

			drawList->AddText(icoonFont, 16.0f, iconPos, theme[ThemeColor::TEXTINPUT_HINT].asInt(), theme[ThemeIcon::CLEAR].glyph.c_str());
		}

		Vector4<float> cursorBox = {
			textPos.x + textSize.x + 3.0f,
			controlBox.center().y - (controlBox.size().y / 3.0f),
			textPos.x + textSize.x + 5.0f,
			controlBox.center().y + (controlBox.size().y / 3.0f)
		};
		drawList->AddRectFilled(cursorBox.min(), cursorBox.max(), cursorColor.asInt(), 16.0f);
	}

	return true;
}
bool NuControls::drawCollapsableControl(class ImDrawList* drawList, const std::string& title, const std::string& tooltip, bool* childSelect, Vector4<float> controlBox)
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

	//Render the module name text
	ImVec2 modNameSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, title.c_str());
	Vector2<float> modNamePos = {
		controlBox.x + 20.0f,
		controlBox.y + ((controlBox.size().y / 2.0f) - (modNameSize.y / 2.0f))
	};
	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::CLICKGUI_MODULECONTROL_HOVERED].asInt(), 8.0f);
		drawList->AddText(uiFont, 24.0f, modNamePos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), title.c_str());

		ImGui::SetTooltipThemed(tooltip.c_str());
	}
	else
	{
		drawList->AddRectFilled(controlBox.min(), controlBox.max(), theme[ThemeColor::CLICKGUI_MODULECONTROL_COLOR].asInt(), 8.0f);
		drawList->AddText(uiFont, 24.0f, modNamePos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), title.c_str());
	}

	//Render the expand/collapse icon
	bool expanded = collapsed[id];
	std::string expandIco = expanded ? theme[ThemeIcon::GUI_COLLAPSE].glyph : theme[ThemeIcon::GUI_EXPAND].glyph;
	ImVec2 expandIcoSize = icoonFont->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, expandIco.c_str());
	//expandIcoSize.x *= 2.0f;
	Vector2<float> expandIcoPos = {
		controlBox.z - 40.0f,
		controlBox.y + ((controlBox.size().y / 2.0f) - (expandIcoSize.y / 2.0f))
	};
	if (ImGui::IsMouseHoveringRect(expandIcoPos, expandIcoPos + expandIcoSize))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			collapsed[id] = !collapsed[id];
		}
	}
	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			collapsed[id] = !collapsed[id];
		}
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !ImGui::IsMouseHoveringRect(expandIcoPos, expandIcoPos + expandIcoSize))
		{
			*childSelect = !*childSelect;
		}
	}
	drawList->AddText(icoonFont, 16.0f, expandIcoPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), expandIco.c_str());

	return expanded;
}
bool NuControls::drawTriStateControl(ImDrawList* drawList, TriState* state, Vector4<float> controlBox)
{
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();

	bool clicked = false;
	if (ImGui::IsMouseHoveringRect(controlBox.min(), controlBox.max()))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			clicked = true;

			switch (*state)
			{
			case TriState::OFF:
				*state = TriState::ON;
				break;
			case TriState::ON:
				*state = TriState::MAYBE;
				break;
			case TriState::MAYBE:
			default:
				*state = TriState::OFF;
				break;
			}
		}
	}

	ColorF defaultCol = theme[ThemeColor::CHECKBOX_COLOR];
	ColorF activeCol = theme[ThemeColor::CHECKBOX_CHECKED];
	ColorF usedCol = *state == TriState::OFF ? defaultCol : activeCol;

	drawList->AddRectFilled(controlBox.min(), controlBox.max(), usedCol.asInt(), 4.0f);
	Internal::drawCheckmark(drawList, state, controlBox, 3.0f);

	return clicked;
}

bool NuControls::drawCheckboxControl(ImDrawList* drawList, bool* state, Vector4<float> controlBox)
{
	TriState triState = TriState::OFF;
	switch (*state)
	{
	case true:
		triState = TriState::ON;
		break;
	default:
		triState = TriState::OFF;
		break;
	};

	bool clicked = NuControls::drawTriStateControl(drawList, &triState, controlBox);

	switch (triState)
	{
	case TriState::ON:
		*state = true;
		break;
	case TriState::MAYBE:
	case TriState::OFF:
	default:
		*state = false;
		break;
	}

	return clicked;
}
