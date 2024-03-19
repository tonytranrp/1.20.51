#pragma once

#include "../IPatch.h"

#include "../../Bridge/GameMode.h"
#include "../../Bridge/Actor.h"

class attackActorHook : public IPatch
{
	static bool __fastcall attackActorCallback_1_20_31(GameMode* _this, Actor* actor);
public:
	static inline uintptr_t funcOriginal = 0;
	attackActorHook();
	auto apply() -> bool override;
};