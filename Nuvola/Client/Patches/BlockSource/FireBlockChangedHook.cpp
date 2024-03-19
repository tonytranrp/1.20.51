#include "FireBlockChangedHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Events/BlockChangedEvent.h"
#include "../../Events/EventDispatcher.h"

#include <algorithm>
#include <map>
#include <mutex>

void __fastcall FireBlockChangedHook::fireBlockChangedCallback_1_20_12_1(BlockSource* self, const BlockPos& blockPos,
																		 uint32_t param_2, const Block* newBlock,
																		 const Block* oldBlock, int param_5,
																		 int32_t eventTarget,
																		 class ActorBlockSyncMessage* param_7,
																		 Actor* who)
{
	PLH::FnCast(funcOriginal, &fireBlockChangedCallback_1_20_12_1)(self, blockPos, param_2, newBlock, oldBlock,
			param_5, eventTarget, param_7, who);

	auto event = nes::make_holder<BlockChangedEvent>(blockPos, oldBlock->blockLegacy, newBlock->blockLegacy, who);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
}

FireBlockChangedHook::FireBlockChangedHook() : IPatch::IPatch("BlockSource::fireBlockChanged")
{
	this->addSignature(SigTable::BlockSource_fireBlockChanged); //1.20.12.1
}

bool FireBlockChangedHook::apply()
{
	return this->autoPatch(&fireBlockChangedCallback_1_20_12_1, &funcOriginal);
}
