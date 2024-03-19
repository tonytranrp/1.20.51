#include "NuvolaLight.h"

NuvolaLight::NuvolaLight() : Theme()
{
    this->icons[ThemeIcon::COMBAT] = IconGlyph::fromString("f255");
    this->icons[ThemeIcon::RENDER] = IconGlyph::fromString("f1b2");
    this->icons[ThemeIcon::MOTION] = IconGlyph::fromString("f554");
    this->icons[ThemeIcon::PLAYER] = IconGlyph::fromString("f007");
    this->icons[ThemeIcon::MISC] = IconGlyph::fromString("f0ad");

    this->icons[ThemeIcon::ADD] = IconGlyph::fromString("f067", 14.0);
    this->icons[ThemeIcon::CHANGING_KEY] = IconGlyph::fromString("f11c");
    this->icons[ThemeIcon::CLEAR] = IconGlyph::fromString("f057");
    this->icons[ThemeIcon::COMBO_OPEN] = IconGlyph::fromString("f078");
    this->icons[ThemeIcon::CONFIG] = IconGlyph::fromString("f085", 0, 4);
    this->icons[ThemeIcon::DELETE] = IconGlyph::fromString("f2ed");
    this->icons[ThemeIcon::GUI_COLLAPSE] = IconGlyph::fromString("f053");
    this->icons[ThemeIcon::GUI_EXPAND] = IconGlyph::fromString("f078");
    this->icons[ThemeIcon::NEXT] = IconGlyph::fromString("f105", 0, 0, 16.0f);
    this->icons[ThemeIcon::REFRESH] = IconGlyph::fromString("f2f9");
    this->icons[ThemeIcon::SEARCH] = IconGlyph::fromString("f002");
    this->icons[ThemeIcon::SETTINGS] = IconGlyph::fromString("f085", 0, 4);
    this->icons[ThemeIcon::THEME] = IconGlyph::fromString("f1fc", 4, 3);

    this->colors[ThemeColor::AIRPLACE_OUTLINE] = ColorF::parse("FFFFFFFF");
    this->colors[ThemeColor::BUTTON_COLOR] = ColorF::parse("#D4D4D6FF");
    this->colors[ThemeColor::BUTTON_ACTIVE] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::BUTTON_HOVERED] = ColorF::parse("#E5E5EAFF");
    this->colors[ThemeColor::CHECKBOX_COLOR] = ColorF::parse("#0000002F");
    this->colors[ThemeColor::CHECKBOX_CHECKED] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::CHECKBOX_CHECK] = ColorF::parse("#FFFFFFFF");
    this->colors[ThemeColor::CONTAINERESP_FURNACE] = ColorF::parse("#8080807F");
    this->colors[ThemeColor::CONTAINERESP_CHEST] = ColorF::parse("#C4A4847F");
    this->colors[ThemeColor::CONTAINERESP_BARREL] = ColorF::parse("#4E35247F");
    this->colors[ThemeColor::CONTAINERESP_SHULKER] = ColorF::parse("#46ADCF7F");
    this->colors[ThemeColor::COMBO_BACKGROUND] = ColorF::parse("#E5E5EAFF");
    this->colors[ThemeColor::COMBO_COLOR] = ColorF::parse("#F2F2F6FF");
    this->colors[ThemeColor::COMBO_DROPDOWN_COLOR] = ColorF::parse("#F2F2F6FF");
    this->colors[ThemeColor::COMBO_HOVERED] = ColorF::parse("#E5E5EAFF");
    this->colors[ThemeColor::COMBO_ITEM_HOVERED] = ColorF::parse("#D7D7D9FF");
    this->colors[ThemeColor::COMBO_ITEM_SELECTED] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::COMBO_TEXT] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_BRAND_COLOR] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_CLOSE_COLOR] = ColorF::parse("#FF3F3FFF");
    this->colors[ThemeColor::CLICKGUI_CLOSE_HOVERED] = ColorF::parse("#DF3F3FDF");
    this->colors[ThemeColor::CLICKGUI_COLOR] = ColorF::parse("#E5E5EABF");
    this->colors[ThemeColor::CLICKGUI_CONFIG_PANEL] = ColorF::parse("#F2F2F600");
    this->colors[ThemeColor::CLICKGUI_CONTENTPANEL_COLOR] = ColorF::parse("#00000000");
    this->colors[ThemeColor::CLICKGUI_DEBUG_CLOSE_HITBOX] = ColorF::parse("#00000000");
    this->colors[ThemeColor::CLICKGUI_MODULECONTROL_COLOR] = ColorF::parse("#F2F2F67F");
    this->colors[ThemeColor::CLICKGUI_MODULECONTROL_HOVERED] = ColorF::parse("#F2F2F67F");
    this->colors[ThemeColor::CLICKGUI_SETTING_LABEL] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_SHADOW] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::CLICKGUI_SIDEBAR_COLOR] = ColorF::parse("#D4D4D6FF");
    this->colors[ThemeColor::CLICKGUI_TEXT_ACTIVE] = ColorF::parse("#F2F2F6FF");
    this->colors[ThemeColor::CLICKGUI_TEXT_COLOR] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_TEXT_HOVERED] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_TILE_COLOR] = ColorF::parse("#E5E5EA7F");
    this->colors[ThemeColor::CLICKGUI_TILE_HOVER] = ColorF::parse("#0101013F");
    this->colors[ThemeColor::CLICKGUI_TILE_SUBTITLE] = ColorF::parse("#8F8F8FFF");
    this->colors[ThemeColor::CLICKGUI_TILE_TITLE] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::CLICKGUI_TITLEBAR_COLOR] = ColorF::parse("#D4D4D6FF");
    this->colors[ThemeColor::ESP_BOTS] = ColorF::parse("#00ff445F");
    this->colors[ThemeColor::ESP_DEFAULT] = ColorF::parse("#FFFFFF5F");
    this->colors[ThemeColor::ESP_HURT] = ColorF::parse("#FF00005F");
    this->colors[ThemeColor::ESP_TEAMMATES] = ColorF::parse("#00ff445F");
    this->colors[ThemeColor::KILLAURA_DEBUG_LOOK] = ColorF::parse("#00FF007F");
    this->colors[ThemeColor::KILLAURA_DEBUG_OPTIMAL] = ColorF::parse("#00FFFF7F");
    this->colors[ThemeColor::KILLAURA_DEBUG_REAL_LOOK] = ColorF::parse("#002FFF7F");
    this->colors[ThemeColor::KILLAURA_TARGET] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::MODLIST_COLOR_BEGIN] = ColorF::parse("#FFFFFF7F");
    this->colors[ThemeColor::MODLIST_SHADOW_BEGIN] = ColorF::parse("#00000000");
    this->colors[ThemeColor::MODLIST_TEXT_BEGIN] = ColorF::parse("#1C1C1EFF");
    this->colors[ThemeColor::MODLIST_COLOR_END] = ColorF::parse("#FFFFFF7F");
    this->colors[ThemeColor::MODLIST_SHADOW_END] = ColorF::parse("#00000000");
    this->colors[ThemeColor::MODLIST_TEXT_END] = ColorF::parse("#1C1C1EFF");
    this->colors[ThemeColor::NOTIFICATION_COLOR] = ColorF::parse("#E5E5EAAF");
    this->colors[ThemeColor::NOTIFICATION_SHADOW] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::NOTIFICATION_TEXT] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::NOTIFICATION_TEXT_BOLD] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::OREESP_COAL] = ColorF::parse("#0000004F");
    this->colors[ThemeColor::OREESP_DIAMOND] = ColorF::parse("#00FFFF4F");
    this->colors[ThemeColor::OREESP_EMERALD] = ColorF::parse("#00FF004F");
    this->colors[ThemeColor::OREESP_GOLD] = ColorF::parse("#FFD7004F");
    this->colors[ThemeColor::OREESP_IRON] = ColorF::parse("#FFFFFF4F");
    this->colors[ThemeColor::OREESP_LAPIZ] = ColorF::parse("#0000FF4F");
    this->colors[ThemeColor::OREESP_REDSTONE] = ColorF::parse("#FF00004F");
    this->colors[ThemeColor::SCROLLBAR_COLOR] = ColorF::parse("#F2F2F67F");
    this->colors[ThemeColor::SCROLLBAR_SCROLLER_COLOR] = ColorF::parse("#FEFFFE7F");
    this->colors[ThemeColor::SCROLLBAR_SCROLLER_HOVERED] = ColorF::parse("#C4C0C17F");
    this->colors[ThemeColor::SCROLLBAR_SCROLLER_MOVING] = ColorF::parse("#C4C0C17F");
    this->colors[ThemeColor::SEGMENTED_COLOR] = ColorF::parse("#F5F7F87F");
    this->colors[ThemeColor::SEGMENTED_SECONDARY] = ColorF::parse("#C1C1C1FF");
    this->colors[ThemeColor::SEGMENTED_SELECTED] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::SEGMENTED_SHADOW] = ColorF::parse("#0000004F");
    this->colors[ThemeColor::SEGMENTED_TEXT] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::SEGMENTED_TEXT_SELECTED] = ColorF::parse("#F2F2F6FF");
    this->colors[ThemeColor::SLIDER_COLOR] = ColorF::parse("#F2F2F6FF");
    this->colors[ThemeColor::SLIDER_COLOR_SLID] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::SLIDER_NOTCH] = ColorF::parse("#FFFFFFFF");
    this->colors[ThemeColor::SLIDER_NOTCH_HOVERED] = ColorF::parse("#DFDFDFFF");
    this->colors[ThemeColor::SLIDER_NOTCH_MOVING] = ColorF::parse("#AFAFAFFF");
    this->colors[ThemeColor::SLIDER_NOTCH_SHADOW] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::TEXTINPUT_BORDER] = ColorF::parse("#00000000");
    this->colors[ThemeColor::TEXTINPUT_COLOR] = ColorF::parse("#E7E7E9FF");
    this->colors[ThemeColor::TEXTINPUT_HOVERED] = ColorF::parse("#D7D7D9FF");
    this->colors[ThemeColor::TEXTINPUT_CURSOR] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::TEXTINPUT_HINT] = ColorF::parse("#8E8E93");
    this->colors[ThemeColor::TEXTINPUT_TEXT] = ColorF::parse("#1F1F1FFF");
    this->colors[ThemeColor::TOGGLE_ON] = ColorF::parse("#1F8AFFFF");
    this->colors[ThemeColor::TOGGLE_OFF] = ColorF::parse("#E5E5EAFF");
    this->colors[ThemeColor::TOGGLE_NOTCH] = ColorF::parse("#FFFFFFFF");
    this->colors[ThemeColor::TOGGLE_NOTCH_SHADOW] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::TOOLTIP_COLOR] = ColorF::parse("#0000007F");
    this->colors[ThemeColor::TOOLTIP_TEXT] = ColorF::parse("#DFDFDFFF");
    this->colors[ThemeColor::TRACERS_BOTS] = ColorF::parse("#00ff445F");
    this->colors[ThemeColor::TRACERS_DEFAULT] = ColorF::parse("#FFFFFF5F");
    this->colors[ThemeColor::TRACERS_HURT] = ColorF::parse("#FF00005F");
    this->colors[ThemeColor::TRACERS_TEAMMATES] = ColorF::parse("#00ff445F");
}

std::string NuvolaLight::getName() const
{
	return "Nuvola Light";
}

void NuvolaLight::setName(std::string name)
{
	return;
}

std::string NuvolaLight::getAuthor() const
{
	return "DisabledMallis";
}

void NuvolaLight::setAuthor(std::string author)
{
	return;
}

