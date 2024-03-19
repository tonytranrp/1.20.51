#pragma once

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_custom.h"

namespace FontRegistry
{
	enum class FontID
	{
		REGULAR,
		THICC,
		HOLLOW_ICOONS,
		SOLID_ICOONS
	};

	void initAll();
	ImFont* getFont(FontID font);
}