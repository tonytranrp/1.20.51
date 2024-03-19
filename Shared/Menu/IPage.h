#pragma once

#include <Math/Math.h>

struct IPage {
	virtual const std::string& GetPageName();
	virtual bool DrawSelection(Vector2<float> guiSize, float maxWid, float fontScale, float darkTheme, int64_t& curPage) { return false; };
	virtual float GetSelectionHeight(float fontScale) { return 0.0f; };
	virtual void DrawPage(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {};
	virtual void DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {};
};