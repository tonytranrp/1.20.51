#pragma once

#include "../IPatch.h"

#include "../../Bridge/Actor.h"

#include <Math/Math.h>

class RenderDispatcherHook : public IPatch {
	static void __fastcall renderDispatcherCallback_1_20_31(class ActorRenderDispatcher* self,
		class BaseActorRenderContext* context, Actor* actor, Vector3<float>* param_3,
		Vector3<float>* param_4, Vector2<float>* param_5, bool param_6);
public:
	static inline uintptr_t funcOriginal = 0;
	RenderDispatcherHook();
	bool apply() override;
};