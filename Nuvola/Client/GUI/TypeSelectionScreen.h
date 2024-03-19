#pragma once

#include "../../Utils/GroupedType.h"
#include "../Features/Theme/ThemeManager.h"

#include "NuControls.h"
#include "WindowScreen.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Fonts/FontRegistry.h>

#include <magic_enum.hpp>

#include <functional>
#include <set>

#undef max
#undef min

template<typename groups_t, typename type_t>
struct TypeSelectionScreenData : public ScreenData
{
	std::function<void(GroupedType<groups_t, type_t>* groups)> initializer;
};

template<typename groups_t, typename type_t>
class TypeSelectionScreen : public WindowScreen
{
protected:
	GroupedType<groups_t, type_t> types;
	std::set<type_t> selected;
	int64_t scrollAmount = 0.0f;
	float lastEventScroll = 0.0f;
	float labelAnimationProg = 0.0f;
	std::string searchQuery;
public:
	TypeSelectionScreen(std::string name) : WindowScreen(name)
	{

	}

	virtual void initialize(ScreenData* data) override
	{
		auto* castedData = dynamic_cast<TypeSelectionScreenData<groups_t, type_t>*>(data);
		assert(castedData != nullptr);
		assert(castedData->initializer != nullptr);

		castedData->initializer(&this->types);

		this->guiSize = { 300, 500 };

		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(Vector2<float>((io.DisplaySize.x / 2.0f) - (guiSize.x / 2.0f), 20.0f));
	}

	virtual void renderWindowContent(float deltaTime) override
	{
		if (labelAnimationProg >= 1.0f)
		{
			labelAnimationProg = 0.0f;
		}
		labelAnimationProg += deltaTime * 0.2f;
		if (labelAnimationProg >= 1.0f)
		{
			labelAnimationProg = 1.0f;
		}

		ImGuiIO& io = ImGui::GetIO();
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

		ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
		if (uiFont == nullptr)
			return;

		Theme theme = systems->getThemeManager()->getActiveTheme();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		Vector2<float> drawPos = { contentArea.x, contentArea.y - scrollAmount };
		ColorF textColor = theme[ThemeColor::CLICKGUI_TEXT_COLOR];

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

		//Draw entity selecting in content area (+15 again for scrollbar)
		ImGui::PushClipRect(contentArea.min(), contentArea.max() + Vector2<float>(15.0f, 0), true);
		for (auto group : magic_enum::enum_values<groups_t>())
		{
			//If types arent loaded, dont render
			std::vector<type_t> typeInGroup = types.getTypesFor(group);
			if (typeInGroup.empty())
			{
				continue;
			}
			//Check if this group has a match for the search query
			if (!searchQuery.empty())
			{
				bool hasMatch = false;
				for (type_t& type : typeInGroup)
				{
					std::string typeName(type);

					if (typeName.find(searchQuery) != std::string::npos)
					{
						hasMatch = true;
						break;
					}
				}

				if (!hasMatch)
				{
					continue;
				}
			}

			ImGui::PushID(magic_enum::enum_name(group).data());
			std::string groupName(magic_enum::enum_name(group));
			bool selectAll = false;
			
			Vector4<float> controlBox = {
				drawPos.x + 15.0f,
				drawPos.y,
				contentArea.z - 10.0f,
				drawPos.y + 40.0f
			};

			bool expanded = NuControls::drawCollapsableControl(drawList, groupName, "Select all entities for this group.", &selectAll, controlBox);
			//Render the enabled toggle
			float tristateSize = 25.0f;
			Vector2<float> center = controlBox.center();
			Vector4<float> tristateBox = {
				controlBox.z - (60.0f + tristateSize),
				center.y - (tristateSize / 2.0f),
				controlBox.z - 60.0f,
				center.y + (tristateSize / 2.0f)
			};

			TriState childSelect = TriState::OFF;
			bool hasAll = true;
			bool hasOne = false;
			for (type_t& type : typeInGroup)
			{
				if (this->selected.contains(type))
				{
					if (!hasOne)
					{
						hasOne = true;
					}
				}
				else {
					hasAll = false;
				}
			}
			if (hasAll)
			{
				childSelect = TriState::ON;
			}
			else if (hasOne)
			{
				childSelect = TriState::MAYBE;
			}
			else
			{
				childSelect = TriState::OFF;
			}

			bool clicked = NuControls::drawTriStateControl(drawList, &childSelect, tristateBox);
			drawPos.y += 50.0f;

			if (clicked)
			{
				switch (childSelect)
				{
				case TriState::OFF:
				case TriState::MAYBE:
					for (type_t& type : typeInGroup)
					{
						if (this->selected.contains(type))
							this->selected.erase(type);
					}
					break;
				case TriState::ON:
					for (type_t& type : typeInGroup)
					{
						if(searchQuery.empty())
						{
							this->selected.insert(type);
						}
						else
						{
							std::string typeName(type);
							if (typeName.find(searchQuery) == std::string::npos)
							{
								continue;
							}
							this->selected.insert(type);
						}
					}
					break;
				}
			}

			if (expanded || !searchQuery.empty())
			{
				for (type_t& type : typeInGroup)
				{
					std::string typeName(type);
					if (typeName.find(searchQuery) == std::string::npos)
					{
						continue;
					}

					bool selected = this->selected.contains(type);
					float boxPadding = 10.0f;

					Vector4<float> checkboxArea = {
						(contentArea.z - tristateSize) - boxPadding,
						drawPos.y,
						contentArea.z - boxPadding,
						drawPos.y + tristateSize
					};

					Vector2<float> textSize = uiFont->CalcTextSizeA(24.0f, FLT_MAX, 0.0f, typeName.c_str());
					Vector4<float> fullLabel = {
						contentArea.x,
						drawPos.y,
						contentArea.x + textSize.x,
						drawPos.y + textSize.y
					};
					Vector4<float> labelBox = {
						contentArea.x + (boxPadding * 2.0f),
						drawPos.y,
						checkboxArea.x - boxPadding,
						drawPos.y + textSize.y
					};
					bool needsAnimate = fullLabel.size().x > labelBox.size().x;
					Vector2<float> textPos = drawPos + Vector2<float>(boxPadding * 2.0f, 0);;
					if (needsAnimate)
					{
						float width = fullLabel.size().x;
						textPos.x -= std::lerp(-width, width, labelAnimationProg);
					}

					ImGui::PushID(typeName.c_str());
					ImGui::PushClipRect(labelBox.min(), labelBox.max(), true);
					drawList->AddText(uiFont, 24.0f, textPos, textColor.asInt(), typeName.c_str(), 0);
					ImGui::PopClipRect();
					ImGui::PopID();

					if (NuControls::drawCheckboxControl(drawList, &selected, checkboxArea))
					{
						if (selected)
						{
							this->selected.insert(type);
						}
						else
						{
							this->selected.erase(type);
						}
					}

					drawPos.y += 40.0f;
				}
			}
			ImGui::PopID();
		}

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
		ImGuiID id = ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y);
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
		if (ImGui::ScrollbarEx(scrollRect, id, ImGuiAxis_Y, &scrollAmount, contentArea.w - contentArea.y, drawPos.y, ImDrawFlags_RoundCornersBottomRight))
		{
			NuControls::setActiveComponent(0);
		}
		ImGui::PopStyleColor(4);

		ImGui::PopClipRect();
	}

	virtual void handleInput(MouseEvent& event) override
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
};