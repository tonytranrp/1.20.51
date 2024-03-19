#include  "RunUpdateCycleHook.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RakPeerRunUpdateCycleEvent.h"
#include "../../../Signatures/SigManager.h"

bool __fastcall RunUpdateCycleHook::RunUpdateCycleHookCallback_1_20_0(RakNet::RakPeer* peer, void* bitStream) {
	auto event = nes::make_holder<RakPeerRunUpdateCycleEvent>(peer);

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	if (event->isCancelled())
		return false;
	return PLH::FnCast(funcOriginal, &RunUpdateCycleHookCallback_1_20_0)(peer, bitStream);
}

RunUpdateCycleHook::RunUpdateCycleHook() : IPatch::IPatch("RakNet::RakPeer::RunUpdateCycle") {
	this->addSignature(SigTable::RakNet_RakPeer_RunUpdateCycle);
}

bool RunUpdateCycleHook::apply() {
	return this->autoPatch(&RunUpdateCycleHookCallback_1_20_0, &funcOriginal);
}