#pragma once

#include <Math/Math.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

class ImDrawList;
struct IconGlyph;
class Module;
typedef unsigned int ImGuiID;

enum class TriState
{
	OFF,
	MAYBE,
	ON
};

namespace NuControls
{
	namespace Internal
	{
		void drawCheckmark(ImDrawList* drawList, TriState* state, Vector4<float> controlBox, float thicknenss = 1.0f);
	}

	ImGuiID getActiveComponent();
	void setActiveComponent(ImGuiID);

	bool drawButtonControl(ImDrawList* drawList, IconGlyph icon, Vector4<float> controlBox);
	bool drawButtonControl(ImDrawList* drawList, std::string text, Vector4<float> controlBox);
	bool drawComboControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox);
	bool drawSegmentedControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox);
	bool drawOptimalEnumControl(ImDrawList* drawList, int* selection, const std::vector<std::string>& values, Vector4<float> controlBox);
	bool drawSliderIntControl(ImDrawList* drawList, int* val, int min, int max, Vector4<float> controlBox);
	bool drawSliderFloatControl(ImDrawList* drawList, float* val, float min, float max, Vector4<float> controlBox);
	bool drawToggleControl(ImDrawList* drawList, bool* val, Vector4<float> controlBox);
	bool drawTextInputControl(ImDrawList* drawList, std::string* val, std::string hint, Vector4<float> controlBox, std::optional<std::string> prefixIcon);
	bool drawCollapsableControl(ImDrawList* drawList, const std::string& title, const std::string& tooltip, bool* childSelect, Vector4<float> controlBox);
	bool drawTriStateControl(ImDrawList* drawList, TriState* state, Vector4<float> controlBox);
	bool drawCheckboxControl(ImDrawList* drawList, bool* state, Vector4<float> controlBox);
}