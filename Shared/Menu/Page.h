#pragma once

#include "IPage.h"

class Page : public IPage {
	std::string pageName;
	size_t idx;

public:
	Page(std::string pageName, size_t idx);

	virtual const std::string& GetPageName() override;
	virtual bool DrawSelection(Vector2<float> guiSize, float maxWid, float fontScale, float darkTheme, int64_t& curPage) override;
	virtual float GetSelectionHeight(float fontScale) override;
	virtual void DrawPage(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) override;
};