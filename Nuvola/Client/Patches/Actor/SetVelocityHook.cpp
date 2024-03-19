#include  "SetVelocityHook.h"

#include <vector>

#include "../../Bridge/Actor.h"
#include "../../Events/ActorMotionEvent.h"
#include "../../Events/EventDispatcher.h"

void __fastcall SetVelocityHook::SetVelocityCallback_1_20_0(Actor* actor, Vector3<float>* vector) {
	auto event = nes::make_holder<ActorMotionEvent>(actor, vector);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
	if (event->isCancelled()) {
		return;
	}
	PLH::FnCast(funcOriginal, &SetVelocityCallback_1_20_0)(event->getActor(), event->getMotion());
}

SetVelocityHook::SetVelocityHook() : IPatch::IPatch("Actor::setVelocity") {
	this->addSignature(SigTable::Actor_setVelocity);
}

bool SetVelocityHook::apply() {
	return this->autoPatch(&SetVelocityCallback_1_20_0, &funcOriginal);
}