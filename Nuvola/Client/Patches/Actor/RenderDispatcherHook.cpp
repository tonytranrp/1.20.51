#include "RenderDispatcherHook.h"

#include "../../Bridge/Components/ActorHeadRotationComponent.h"
#include "../../Bridge/Components/MobBodyRotationComponent.h"

#include "../../Events/ActorMoveEvent.h"
#include "../../Events/ActorRotateEvent.h"
#include "../../Events/EventDispatcher.h"

#include <xorstr.hpp>

void __fastcall RenderDispatcherHook::renderDispatcherCallback_1_20_31(class ActorRenderDispatcher* self,
	class BaseActorRenderContext* context, Actor* actor, Vector3<float>* param_3,
	Vector3<float>* param_4, Vector2<float>* param_5, bool param_6)
{
	std::array<Vector2<float>, 4> restoreRotation;
	restoreRotation[0] = actor->rotationComponent->rotation;
	restoreRotation[1] = actor->rotationComponent->oldRotation;
	auto* headRotComponent = actor->entityContext.tryGetComponent<ActorHeadRotationComponent>();
	if(headRotComponent)
	{
		restoreRotation[2] = {headRotComponent->headRot, headRotComponent->oldHeadRot};
	}

	auto* bodyRotComponent = actor->entityContext.tryGetComponent<MobBodyRotationComponent>();
	if(bodyRotComponent)
	{
		restoreRotation[3] = {bodyRotComponent->yBodyRot, bodyRotComponent->yOldBodyRot};
	}

	auto event = nes::make_holder<ActorRotateEvent>(actor, actor->rotationComponent->rotation, actor->rotationComponent->oldRotation, RotationType::RENDER);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	actor->rotationComponent->rotation = event->getRotation();
	actor->rotationComponent->oldRotation = event->getOldRotation();
	if(headRotComponent)
	{
		headRotComponent->headRot = event->getYaw();
		headRotComponent->oldHeadRot = event->getOldYaw();
	}
	if(bodyRotComponent)
	{
		bodyRotComponent->yBodyRot = event->getYaw();
		bodyRotComponent->yOldBodyRot = event->getOldYaw();
	}

	PLH::FnCast(funcOriginal, &renderDispatcherCallback_1_20_31)(self, context, actor, param_3, param_4, param_5, param_6);

	actor->rotationComponent->rotation = restoreRotation[0];
	actor->rotationComponent->oldRotation = restoreRotation[1];
	if(headRotComponent)
	{
		headRotComponent->headRot = restoreRotation[2].x;
		headRotComponent->oldHeadRot = restoreRotation[2].y;
	}
	if(bodyRotComponent)
	{
		bodyRotComponent->yBodyRot = restoreRotation[3].x;
		bodyRotComponent->yOldBodyRot = restoreRotation[3].y;
	}
}

RenderDispatcherHook::RenderDispatcherHook() : IPatch(xorstr_("ActorRenderDispatcher::render"))
{
	this->addSignature(SigTable::ActorRenderDispatcher_render); //1.20.31
}

bool RenderDispatcherHook::apply()
{
	return this->autoPatch(&renderDispatcherCallback_1_20_31, &funcOriginal);
}
