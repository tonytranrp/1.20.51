#include "BlockESP.h"

#include "../../../Bridge/Nuvola/Bridge.h"
#include "../../../Bridge/BlockPalette.h"
#include "../../../Bridge/ChunkSource.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/BlockChangedEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/RenderEvent.h"
#include "../../../Events/ReceivePacketEvent.h"

#include "../../../Rendering/BlockRenderer.h"
#include "../../../Rendering/GeometryBuilder.h"
#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/WorldRenderer.h"

#include "../../../../Utils/RenderUtils.h"

#include "../../Theme/ThemeManager.h"

#include "../ModuleManager.h"

#include "Notifications.h"

#include <ranges>

extern std::unique_ptr<BlockRenderer> nuvolaBlockRenderer;

BlockESP::BlockESP() : Module("BlockESP", "Displays ores around the player.")
{
	this->range = this->addItem<Setting>("Range", "The distance (in chunks) from the player that should be checked for ores each frame. This highly impacts framerate.", SettingType::SLIDER_INT, 1, 1, 32);
	this->selectedBlocks = this->addItem<BlockTypeListSetting>("Blocks", "Select which blocks should be rendered with Block ESP");
}

void BlockESP::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<BlockChangedEvent, &BlockESP::onBlockChangedEvent>(this);
	dispatcher->listen<LevelTickEvent, &BlockESP::onLevelTickEvent>(this);
	dispatcher->listen<RenderEvent, &BlockESP::onRenderEvent>(this);
	dispatcher->listen<ReceivePacketEvent, &BlockESP::onReceivePacketEvent>(this);
	this->resetService();
}

void BlockESP::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<BlockChangedEvent, &BlockESP::onBlockChangedEvent>(this);
	dispatcher->deafen<LevelTickEvent, &BlockESP::onLevelTickEvent>(this);
	dispatcher->deafen<RenderEvent, &BlockESP::onRenderEvent>(this);
	dispatcher->deafen<ReceivePacketEvent, &BlockESP::onReceivePacketEvent>(this);

	std::lock_guard<std::recursive_mutex> disableGuard(this->disableSemaphore);
	foundBlocks.clear();
	this->resetService();
	nuvolaBlockRenderer->reset();
}

void BlockESP::desyncCritical() {
	std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
	notifs->notify("BlockESP's world data was desynced from Minecraft's world data. This may lead to a crash, so BlockESP was disabled.", "BlockESP Error", 10.0f);
	this->setEnabled(false);
}

void BlockESP::resetService() {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr)
	{
		std::lock_guard<std::recursive_mutex> disableGuard(this->disableSemaphore);
		foundBlocks.clear();
		nuvolaBlockRenderer->reset();

		this->state = ServiceState::IDLE;
		return;
	}
	std::shared_ptr<Dimension> dim = player->dimension;
	if (dim == nullptr) return;
	BlockSource* blockSource = dim->blockSource;
	if (blockSource == nullptr) return;

	ChunkPos playerPos = player->getChunkPos();
	this->searchCenter = playerPos;
	this->currentChunkPos = playerPos;
	this->subChunkIndex = 0;//(playerPos.y - blockSource->buildDepth) >> 4;
	this->state = ServiceState::SCAN;
	this->steps = 1;
}

void BlockESP::moveNextSubChunk() {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	std::shared_ptr<Dimension> dim = player->dimension;
	if (dim == nullptr) return;
	BlockSource* blockSource = dim->blockSource;
	if (blockSource == nullptr) return;

	// Spiral search pattern
	static const std::vector<std::pair<int, int>> directions = {
			{1, 0}, {0, 1}, {-1, 0}, {0, -1}
	};

	size_t numSubchunks = (blockSource->buildHeight - blockSource->buildDepth) / 16;
	if(numSubchunks-1 > subChunkIndex)
	{
		subChunkIndex++;
		return;
	}

	// Move to the next chunk in the spiral pattern
	currentChunkPos.x += directions[directionIndex].first;
	currentChunkPos.y += directions[directionIndex].second;

	// Update steps and stepsCount
	stepsCount++;
	if (stepsCount >= steps) {
		stepsCount = 0;
		directionIndex = (directionIndex + 1) % directions.size();
		if (directionIndex % 2 == 0) {
			steps++;
		}
	}
	subChunkIndex = 0;
}

bool BlockESP::processSubChunk(ChunkPos processChunk, int scIndex)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return false;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return false;
	Level* level = player->level;
	if (level == nullptr) return false;
	BlockPalette* levelPalette = level->mBlockPalette;
	if(levelPalette == nullptr) return false;
	std::shared_ptr<Dimension> dim = player->dimension;
	if (dim == nullptr) return false;
	BlockSource* blockSource = dim->blockSource;
	if (blockSource == nullptr) return false;
	ChunkSource* chunkSource = dim->chunkSource;
	if (chunkSource == nullptr) return false;
	auto storage = chunkSource->getStorage();

	ChunkPos playerPos = player->getChunkPos();
	int searchRadius = range->getValueAs<int>();

	if(playerPos.distanceTo((Vector2<int> &) processChunk) > searchRadius) return false;
	auto chunkRef = storage.at(processChunk);
	auto pChunk = chunkRef.lock();
	if(chunkRef.expired()) return false;
	if(!pChunk->isFullyLoaded()) return false;
	auto subChunk = pChunk->subChunks.at(scIndex);
	auto blockReader = subChunk.blockReadPtr;
	if(blockReader->getSize() == 0) return false;
	bool isValuable = false;
	const auto& selection = selectedBlocks->getSelectedBlocks();
	for(auto selected : selection)
	{
		if(blockReader->isUniform(selected))
		{
			isValuable = true;
			break;
		}
		else
		{
			if(subChunk.hasBlock(selected->blockLegacy->blockId))
			{
				isValuable = true;
				break;
			}
		}
	}
	if(!isValuable)
	{
		return false;
	}

	if(isValuable)
	{
		uint16_t searchRangeXZ = 16;
		uint16_t searchRangeY = (blockSource->buildHeight - blockSource->buildDepth) / pChunk->subChunks.size();

		//Do search logic
		for(uint16_t x = 0; x < searchRangeXZ; x++)
		{
			for(uint16_t z = 0; z < searchRangeXZ; z++)
			{
				for(uint16_t y = 0; y < searchRangeY; y++)
				{
					uint16_t elementId = (x * 0x10 + z) * 0x10 + (y & 0xf);
					const Block* found = blockReader->getElement(elementId);
					bool contains = std::ranges::any_of(selection.begin(), selection.end(), [&](auto block){
						return block->blockLegacy->blockId == found->blockLegacy->blockId;
					});
					if(!contains)
						continue;

					BlockPos pos;
					pos.x = (processChunk.x * 16) + x;
					pos.z = (processChunk.y * 16) + z;
					pos.y = y + (subChunk.subchunkIndex * 16);
					this->foundBlocks[pos] = found;
					this->needsRender = true;
				}
			}
		}
	}
	return isValuable;
}

void BlockESP::onBlockChangedEvent(BlockChangedEvent &event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	if (player->ticksAlive < 10) return;
	Level* level = player->level;
	if (level == nullptr) return;
	std::shared_ptr<Dimension> dim = player->dimension;
	if (dim == nullptr) return;
	BlockSource* blockSource = dim->blockSource;
	if (blockSource == nullptr) return;
	ChunkSource* chunkSource = dim->chunkSource;
	if (chunkSource == nullptr) return;

	auto storage = chunkSource->getStorage();
	ChunkPos updateChunk(event.getWhere());
	int subChunk = (event.getWhere().y - blockSource->buildDepth) >> 4;
	auto chunkRef = storage.at(updateChunk);
	if(chunkRef.expired()) return;
	auto levelChunk = chunkRef.lock();
	processSubChunk(updateChunk, subChunk);

	const BlockLegacy* newBlock = event.getBlock();
	if(newBlock == nullptr || newBlock->renderBlock == nullptr || newBlock->isAir())
	{
		this->foundBlocks.erase(event.getWhere());
		this->needsRender = true;
	}
	else
	{
		if(this->selectedBlocks->getSelectedBlocks().contains(newBlock->renderBlock))
		{
			this->foundBlocks[event.getWhere()] = newBlock->renderBlock;
			this->needsRender = true;
		}
	}
}

void BlockESP::onLevelTickEvent(LevelTickEvent& event)
{
	try
	{
		ClientInstance* ci = Bridge::getClientInstance();
		if (ci == nullptr) return;
		LocalPlayer* player = ci->getClientPlayer();
		if (player == nullptr) return;
		if (player->ticksAlive < 10) return; //There is some datarace to be careful of when accessing blocksource too early
		Level* level = player->level;
		if (level == nullptr) return;
		std::shared_ptr<Dimension> dim = player->dimension;
		if (dim == nullptr) return;
		BlockSource* blockSource = dim->blockSource;
		if (blockSource == nullptr) return;
		ChunkSource* chunkSource = dim->chunkSource;
		if (chunkSource == nullptr) return;

		ChunkPos playerPos = player->getChunkPos();
		int searchRadius = range->getValueAs<int>();

		if ((float)this->searchCenter.distanceTo(playerPos) > (float)searchRadius * 0.5f) {
			this->resetService();
		}

		switch(this->state)
		{
			case ServiceState::SCAN: {
				if (currentChunkPos.distanceTo(searchCenter) > searchRadius) {
					this->state = ServiceState::IDLE;
				}

				int tries = 4;
				while (tries > 0) {
					if (processSubChunk(currentChunkPos, subChunkIndex)) {
						tries = 0;
					}
					moveNextSubChunk();
					tries--;
				}
				break;
			}
			//We are finished scanning and now idle only listening for block updated
			case ServiceState::IDLE: {
				break;
			}
			default: {
				this->resetService();
				break;
			}
		}
	}
	catch(const std::exception& ex)
	{
		std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
		notifs->notify(ex.what(), "BlockESP Error", 10.0f);
		this->resetService();
	}
}

void BlockESP::onRenderEvent(RenderEvent& event)
{
	std::lock_guard<std::recursive_mutex> disableGuard(this->disableSemaphore);
	if(this->needsRender)
	{
		nuvolaBlockRenderer->reset();
		ColorF color(1.0f,1.0f,1.0f,0.5f);
		for(auto [blockPos, block] : this->foundBlocks)
		{
			const auto& selection = this->selectedBlocks->getSelectedTypes();
			nuvolaBlockRenderer->computeAndCache(blockPos, color);
		}
		this->needsRender = false;
	}
}

void BlockESP::onReceivePacketEvent(ReceivePacketEvent &event) {
	switch(event.getPacket()->getId())
	{
		case PacketID::ChangeDimensionPacket:
		case PacketID::TransferPacket:
			this->desyncCritical();
			break;
		default:
			break;
	}
}
