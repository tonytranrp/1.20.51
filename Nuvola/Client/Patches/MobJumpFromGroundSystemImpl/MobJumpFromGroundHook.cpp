#include "MobJumpFromGroundHook.h"

#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/LocalPlayer.h"
#include "../../Bridge/StrictEntityContext.h"

#include "../../Events/EventDispatcher.h"
#include "../../Events/MobQueryJumpPowerEvent.h"

void __cdecl MobJumpFromGroundHook::MobJumpFromGroundCallback_1_20_12_1(StrictEntityContext* context,
	AABBShapeComponent* aabbShapeComponent, class SynchedActorDataComponent* param_3,
	ActorRotationComponent* rotationComponent, class MobEffectsComponent* param_5, class JumpControlComponent* param_6,
	class MobJumpComponent* param_7, StateVectorComponent* stateVectorComponent, class EntityModifierT* param_9,
	class IConstBlockSource* param_10, bool param_11)
{
	PLH::FnCast(funcOriginal, &MobJumpFromGroundCallback_1_20_12_1)(context, aabbShapeComponent, param_3,
			rotationComponent, param_5, param_6, param_7, stateVectorComponent, param_9, param_10, param_11);

	auto ci = Bridge::getClientInstance();
	if(ci == nullptr) return;
	auto lp = ci->getClientPlayer();
	if(lp == nullptr) return;
	auto& lpContext = lp->entityContext;
	EntityContext ectx;
	ectx.id = context->entity;
	ectx.mRegistry = lpContext.mRegistry;
	ectx.mEnttRegistry = lpContext.mEnttRegistry;
	auto mob = reinterpret_cast<Mob*>(Actor::tryGetFromEntity(ectx));

	auto event = nes::make_holder<MobQueryJumpPowerEvent>(mob, stateVectorComponent->velocity.y);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
	stateVectorComponent->velocity.y = event->getPower();
}

MobJumpFromGroundHook::MobJumpFromGroundHook() : IPatch::IPatch("SystemImpl::mobJumpFromGround") {
	this->addSignature(SigTable::MobJumpFromGroundSystemImpl_mobJumpFromGround);
}

bool MobJumpFromGroundHook::apply() {
	return this->autoPatch(&MobJumpFromGroundCallback_1_20_12_1, &funcOriginal);
}