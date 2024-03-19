#include "ActorSigProvider.h"

#include <xorstr.hpp>

ActorSigProvider::ActorSigProvider() : SigProvider("Actor")
{
	this->addResult(SigTable::ActorRenderDispatcher_render, "48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d 6c 24 ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 45 ? 4c 89 4c 24 ? 49 8b d8");

	this->addResult(SigTable::Actor_move, "48 89 5C 24 ? 57 48 83 EC ? 48 83 B9 ? ? ? ? ? 48 8B FA 48 8B D9 74 59");
	//this->AddResult(SigTable::Actor_normalTick, "48 8b c4 48 89 58 ?? 48 89 70 ?? 48 89 78 ?? 55 41 56 41 57 48 8d 68 ?? 48 81 ec ?? ?? ?? ?? 0f 29 70 ?? 0f 29 78 ?? 44 0f 29 40 ?? 48 8b 05 ?? ?? ?? ?? 48 33 c4 48 89 45 ?? 48 8b f1");
	this->addResult(SigTable::Actor_setVelocity, std::string("48 83 EC 28 4C 8B 81 ? ? ? ? 4D 85 ? 74 ? 8B 02 41 89 ? ? 8B 42 ? 41 89 ? ? 8B 42 ? 41 89 40 20 48 83 C4 ? C3 E8"));
	//this->AddResult(SigTable::Actor_normalTick, std::string("48 89 5c 24 ? 48 89 74 24 ? 48 89 7c 24 ? 55 48 8d 6c 24 ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 45 ? 48 8b f9 48 8b 89 ? ? ? ? 48 8b 01"));
	//this->AddResult(SigTable::Actor_tryGetFromEntity, std::string("48 83 ec ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 44 24 ? 48 8b d1 48 8d 4c 24 ? e8 ? ? ? ? 80 7c 24 ? ? 74 ? 48 8d 4c 24 ? e8 ? ? ? ? 84 c0 74 ? 48 8b 44 24 ? 48 8d 54 24 ? 48 8b 08 8b 44 24 ? 89 44 24 ? e8 ? ? ? ? 48 85 c0"));
}
