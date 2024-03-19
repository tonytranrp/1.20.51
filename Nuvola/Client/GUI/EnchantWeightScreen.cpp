#include "EnchantWeightScreen.h"

#include "../Bridge/EnchantUtils.h"
#include "../Features/Theme/ThemeManager.h"
#include "NuControls.h"

#include <Fonts/FontRegistry.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <magic_enum.hpp>
#include <magic_enum_utility.hpp>

#include <algorithm>

EnchantWeightScreenData::EnchantWeightScreenData(std::shared_ptr<EnchantWeightSetting> weightSetting) : weightSetting(weightSetting) {}

std::shared_ptr<EnchantWeightSetting> EnchantWeightScreenData::getWeightsAccess() const
{
	return this->weightSetting;
}

EnchantWeightScreen::EnchantWeightScreen() : WindowScreen("EnchantWeightScreen")
{
	magic_enum::enum_for_each<EnchantType>([&](auto val) {
		constexpr EnchantType enchant = val;
		enchantsSet.emplace(enchant);
	});
}

void EnchantWeightScreen::initialize(ScreenData* data)
{
	EnchantWeightScreenData* casted = dynamic_cast<EnchantWeightScreenData*>(data);
	assert(casted);

	auto weightsPtr = casted->getWeightsAccess();
	this->weightSetting = weightsPtr;

	this->guiSize = { 500, 500 };

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(Vector2<float>((io.DisplaySize.x / 2.0f) - (guiSize.x / 2.0f), 20.0f));

	std::transform(this->enchantsSet.begin(), this->enchantsSet.end(), std::back_inserter(this->enchantNames), [](auto val) {
		return std::string(magic_enum::enum_name(val));
	});
}

void EnchantWeightScreen::renderWindowContent(float deltaTime)
{
	Theme theme = systems->getThemeManager()->getActiveTheme();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImGuiIO& io = ImGui::GetIO();
	ImGuiID id = ImGui::GetCurrentWindow()->IDStack.back();
	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;

	Vector4<float> windowRect = {
		ImGui::GetWindowPos().x,
		ImGui::GetWindowPos().y,
		ImGui::GetWindowPos().x + guiSize.x,
		ImGui::GetWindowPos().y + guiSize.y
	};
	Vector4<float> titleBarRect = {
		windowRect.x,
		windowRect.y,
		windowRect.z,
		windowRect.y + 60.0f
	};
	Vector4<float> contentArea = {
		windowRect.x,
		titleBarRect.w,
		windowRect.z - 15.0f, //-15 for scrollbar
		windowRect.w
	};
	ImRect contentBehaviorBox = {
		contentArea.x,
		contentArea.y,
		contentArea.z,
		contentArea.w,
	};
	bool useless = false;
	ImGui::ButtonBehavior(contentBehaviorBox, id, &useless, &useless);

	//Draw search bar
	ImGui::PushID(&this->searchQuery);
	float searchBoxPad = 20.0f;
	Vector4<float> searchBox = {
		titleBarRect.x + searchBoxPad,
		titleBarRect.center().y - (searchBoxPad / 1.5f),
		titleBarRect.z - 60.0f,
		titleBarRect.center().y + (searchBoxPad / 1.5f)
	};
	NuControls::drawTextInputControl(drawList, &searchQuery, "Search", searchBox, theme[ThemeIcon::SEARCH].glyph);
	ImGui::PopID();

	ImGui::PushClipRect(contentArea.min(), contentArea.max() + Vector2<float>(15.0f, 0), true);
	Vector2<float> drawPos = { windowRect.x + searchBoxPad, (searchBox.w + searchBoxPad) - scrollAmount };
	for (int i = 0; i < this->enchantsSet.size(); i++)
	{
		//TODO: Make an offset for EnchantUtils::mEnchants, or use that to find the getEnchant function
		//Then, use that to get the Enchant based on the EnchantType
		//Finally, check if the sig below is for the I18n::get function
		//48 89 5c 24 ? 48 89 7c 24 ? 41 56 48 83 ec ? 4d 8b f0
		//And if so, use that with the enchant's translationKey to get the enchant name
		EnchantType enchant = *std::next(this->enchantsSet.begin(), i);
		std::string name = this->enchantNames[magic_enum::enum_index(enchant).value()]; //EnchantUtils::getEnchantNameAndLevel(enchant, 0);
		std::string nameLower;
		std::transform(name.begin(), name.end(), std::back_inserter(nameLower), [](auto c){ return std::tolower(c); });
		std::string searchLower;
		std::transform(searchQuery.begin(), searchQuery.end(), std::back_inserter(searchLower), [](auto c){ return std::tolower(c); });

		if (nameLower.find(searchLower) == std::string::npos)
		{
			continue;
		}

		//Label for enchant
		Vector2<float> textSize = uiFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, name.data());
		drawList->AddText(uiFont, 20.0f, drawPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), name.data());

		//Slider for weight
		ImGui::PushID(*reinterpret_cast<int*>(&enchant));
		Vector4<float> sliderBox = {
			contentArea.x + (guiSize.x / 2.0f),
			drawPos.y,
			contentArea.z - 60.0f,
			drawPos.y + textSize.y
		};
		float weight = this->weightSetting->getWeight(enchant);
		if (NuControls::drawSliderFloatControl(drawList, &weight, -5.0f, 5.0f, sliderBox))
		{
			this->weightSetting->removeWeight(enchant);
			this->weightSetting->addWeight(enchant, weight);
		}
		ImGui::PopID();

		Vector4<float> labelBox = {
			sliderBox.z + 25.0f,
			sliderBox.y,
			contentArea.z - 15.0f,
			sliderBox.w
		};
		std::string weightStr = std::format("{:.2f}", weight);
		Vector2<float> labelSize = uiFont->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, weightStr.c_str());
		Vector2<float> labelPos = {
			labelBox.center().x - (labelSize.x / 2.0f),
			labelBox.center().y - (labelSize.y / 2.0f)
		};
		drawList->AddText(uiFont, 20.0f, labelPos, theme[ThemeColor::CLICKGUI_TEXT_COLOR].asInt(), weightStr.c_str());

		drawPos += { 0.0f, textSize.y + 4.0f };
	}
	Vector4<float> resetBox = {
		drawPos.x,
		drawPos.y,
		contentArea.z - (drawPos.x - contentArea.x),
		drawPos.y + 24.0f
	};
	if (NuControls::drawButtonControl(drawList, "Reset", resetBox))
	{
		this->weightSetting->resetToDefault();
	}
	drawPos += { 0.0f, 35.0f };

	this->guiSize.y = drawPos.y - windowRect.y;
	if (this->guiSize.y > (io.DisplaySize.y * 0.75f))
	{
		this->guiSize.y = io.DisplaySize.y * 0.75f;
	}
	drawPos.y -= contentArea.y;
	drawPos.y += scrollAmount;

	if (contentArea.contains(io.MousePos))
	{
		scrollAmount += lastEventScroll;
		lastEventScroll = 0;
	}

	if (scrollAmount <= 0)
		scrollAmount = 0;
	const int64_t scrollMax = std::max(1.0f, drawPos.y - (contentArea.w - contentArea.y));
	if (scrollMax <= scrollAmount)
		scrollAmount = scrollMax;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiID scrollid = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
	ImRect scrollRect = {
		contentArea.z,
		contentArea.y,
		contentArea.z + 15.0f,
		contentArea.w
	};
	ImGui::GetStyle().WindowRounding = 8.0f;
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, theme[ThemeColor::SCROLLBAR_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme[ThemeColor::SCROLLBAR_SCROLLER_COLOR]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, theme[ThemeColor::SCROLLBAR_SCROLLER_HOVERED]);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, theme[ThemeColor::SCROLLBAR_SCROLLER_MOVING]);
	if (ImGui::ScrollbarEx(scrollRect, scrollid, ImGuiAxis_Y, &scrollAmount, contentArea.w - contentArea.y, drawPos.y, ImDrawFlags_RoundCornersBottomRight))
	{
		NuControls::setActiveComponent(0);
	}
	ImGui::PopStyleColor(4);
	ImGui::PopClipRect();
}

void EnchantWeightScreen::handleInput(MouseEvent& event)
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