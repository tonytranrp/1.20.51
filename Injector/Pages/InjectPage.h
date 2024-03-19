#pragma once
#include <Menu/Page.h>
#include "../Discord/DiscordRPCIntegration.h"

class InjectPage : public Page {
public:
	InjectPage(size_t idx);

	virtual void DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) override;
};