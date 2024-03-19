#pragma once

#include <Menu/Page.h>

class AccountPage : public Page {
public:
	AccountPage(size_t idx);

	virtual void DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) override;
};