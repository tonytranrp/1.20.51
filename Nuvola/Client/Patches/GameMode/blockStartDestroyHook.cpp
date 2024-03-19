#include "blockStartDestroyHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Events/BlockStartDestroyEvent.h"
#include "../../Events/EventDispatcher.h"


bool __fastcall blockStartDestroyHook::blockStartDestroyCallback_1_20_15(GameMode* _this, BlockPos* blockPos, unsigned char a3, bool* a4)
{
	auto event = nes::make_holder<BlockStartDestroyEvent>(blockPos);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	return PLH::FnCast(funcOriginal, &blockStartDestroyCallback_1_20_15)(_this, blockPos, a3, a4);
}

blockStartDestroyHook::blockStartDestroyHook() : IPatch::IPatch("GameMode::startDestroyBlock")
{
	this->addSignature(SigTable::GameMode_startDestroyBlock); //1.20.12.1
}

bool blockStartDestroyHook::apply()
{
	return this->autoPatch(&blockStartDestroyCallback_1_20_15, &funcOriginal);
}