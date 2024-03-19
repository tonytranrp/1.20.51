#include "FontRegistry.h"

#include "Fonts.h"
#include "IconsFontAwesome5.h"

#include <map>
#include <vector>

static std::map<FontRegistry::FontID, ImFont*> fonts;

void FontRegistry::initAll()
{
	fonts[FontID::REGULAR] = ImGui::FontFromArray(sf_pro_text_regular, 24.0f);
	fonts[FontID::THICC] = ImGui::FontFromArray(sf_pro_text_medium, 24.0f);

	ImFontConfig config;
	config.MergeMode = false;
	config.GlyphMinAdvanceX = 24.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	fonts[FontID::HOLLOW_ICOONS] = ImGui::FontFromArray(fa_regular_400, 24.0f, &config, icon_ranges);
	fonts[FontID::SOLID_ICOONS] = ImGui::FontFromArray(fa_solid_900, 24.0f, &config, icon_ranges);
}

ImFont* FontRegistry::getFont(FontID font)
{
	return fonts[font];
}
