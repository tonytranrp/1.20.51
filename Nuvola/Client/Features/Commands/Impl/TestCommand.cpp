#include "TestCommand.h"

#include "../../../Bridge/ChunkSource.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/GuiData.h"

#include "../../IRC/KairunVM.h"

TestCommand::TestCommand() : Command("test") {

}

void TestCommand::initialize(bcs::command_dispatcher& dispatcher)
{
}

/*
bool TestCommand::execute(const std::vector<std::string>& params) {
	ClientInstance* client = Bridge::getClientInstance();
	//Camera& camera = client->camera;
	LocalPlayer* player = client->getClientPlayer();
	ChunkPos playerPos = player->getBlockPos();
	std::shared_ptr<Dimension> dim = player->dimension;
	ChunkSource* csource = dim->chunkSource;
	auto chunkStorage = csource->getStorage();
	BlockSource* bsource = dim->blockSource;

	if(params.empty())
		return true;

	if(params.size() > 0)
	{
		this->displayMessage("Chunk 100,100 is loaded");
        auto weakLevelChunk = chunkStorage.at(ChunkPos(100,100));
        auto levelChunk = weakLevelChunk.lock();
        auto& subChunks = levelChunk->subChunks;
        for(const auto& subChunk : subChunks)
        {
            auto& readPtr = subChunk.blockReadPtr;
        }
	}

	return true;
};
*/

std::string TestCommand::getDescription() const {
	return "A test command for developers to test things with";
}