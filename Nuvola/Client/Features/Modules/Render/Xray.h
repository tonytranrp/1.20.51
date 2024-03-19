#pragma once

#include "../Module.h"

class BlockLegacyLayerEvent;
class Xray : public Module
{
#ifdef _DEBUG
	std::shared_ptr<Setting> layerVal = nullptr;
#endif
public:
	Xray();

	virtual void onEnable() override;
	virtual void onDisable() override;

	void onBlockLegacyLayerEvent(BlockLegacyLayerEvent& event);
};