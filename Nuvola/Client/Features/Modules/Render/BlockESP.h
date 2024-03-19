#pragma once

#include "../../../Bridge/Types.h"

#include "../BlockTypeListSetting.h"
#include "../Module.h"

#include <Math/Math.h>

#include <future>
#include <map>
#include <memory>
#include <mutex>

class BlockChangedEvent;
class LevelTickEvent;
class RenderEvent;
class ReceivePacketEvent;
class BlockESP : public Module
{
	enum class ServiceState
	{
		SCAN,
		IDLE
	};
	std::shared_ptr<Setting> range;
	std::shared_ptr<BlockTypeListSetting> selectedBlocks;

	std::map<Vector3<int>, const Block*> foundBlocks;
	bool needsRender = true;
	std::recursive_mutex disableSemaphore;

	//Service state management
	ServiceState state = ServiceState::IDLE;

	//Block search data
	ChunkPos searchCenter;
	ChunkPos currentChunkPos;
	int subChunkIndex = 0;
	int directionIndex = 0;
	int steps = 1;
	int stepsCount = 0;
public:
	BlockESP();
	void onEnable() override;
	void onDisable() override;

	void desyncCritical();
	void resetService();
	void moveNextSubChunk();
	bool processSubChunk(ChunkPos processChunk, int subChunk);

	void onBlockChangedEvent(BlockChangedEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
	void onRenderEvent(RenderEvent& event);
	void onReceivePacketEvent(ReceivePacketEvent& event);
};