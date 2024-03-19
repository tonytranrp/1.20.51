#pragma once

#include "ActorDefinitionIdentifier.h"

#include <unordered_map>

class ActorFactory
{
public:
	char pad_0000[64-8]; //0x0000
	std::unordered_map<std::string, ActorDefinitionIdentifier> actorIdentifiers; //0x0038
	char pad_0050[96];
}; //Size: 0x00D8

//char(*__kaboom)[sizeof(std::unordered_map<std::string, ActorDefinitionIdentifier>)] = 1;

static_assert(sizeof(ActorFactory) == 0xD8);