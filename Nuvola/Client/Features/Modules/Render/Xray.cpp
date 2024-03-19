#include "Xray.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/BlockLegacyLayerEvent.h"

Xray::Xray() : Module("Xray", "Makes certain blocks render on top of all others.")
{
#ifdef _DEBUG
	this->layerVal = this->addItem<Setting>("Layerval", "Testing value", SettingType::SLIDER_INT, 0, 0, 50);
#endif
}

void Xray::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<BlockLegacyLayerEvent, &Xray::onBlockLegacyLayerEvent>(this);
}

void Xray::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<BlockLegacyLayerEvent, &Xray::onBlockLegacyLayerEvent>(this);
}

void Xray::onBlockLegacyLayerEvent(BlockLegacyLayerEvent& event)
{
	const BlockLegacy* legacy = event.getBlock();
	if (legacy->blockId != BlockId::IRON_ORE)
	{
		event.setLayer(BlockRenderLayer::RENDERLAYER_BARRIER);
	}
	else
	{
		event.setLayer(BlockRenderLayer::RENDERLAYER_DOUBLE_SIDED);
	}
}
