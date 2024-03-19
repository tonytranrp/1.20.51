#include "attackActorHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Events/AttackActorEvent.h"
#include "../../Events/EventDispatcher.h"


bool __fastcall attackActorHook::attackActorCallback_1_20_31(GameMode* _this, Actor* actor)
{
	auto event = nes::make_holder<AttackActorEvent>(_this, actor);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
	
	return PLH::FnCast(funcOriginal, &attackActorCallback_1_20_31)(_this, actor);
}

attackActorHook::attackActorHook() : IPatch::IPatch("GameMode::startDestroyBlock")
{
	this->addSignature(SigTable::GameMode_attack); //1.20.31
}

bool attackActorHook::apply()
{
	return this->autoPatch(&attackActorCallback_1_20_31, &funcOriginal);
}