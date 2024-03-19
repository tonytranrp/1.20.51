#pragma once

#include <Menu/Page.h>

class SettingsPage : public Page {
public:
	SettingsPage(size_t idx);

	virtual void DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) override;
};