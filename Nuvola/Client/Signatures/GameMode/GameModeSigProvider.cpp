#include "GameModeSigProvider.h"

#include "../../../Mem/Mem.h"

GameModeSigProvider::GameModeSigProvider() : SigProvider("GameMode")
{
	this->addResult(SigTable::GameMode_getDestroyRate, std::string("48 89 ? ? ? 57 48 83 EC ? 48 8B ? 0F 29 ? ? ? 48 8B ? ? E8"));
	this->addResult(SigTable::GameMode_startDestroyBlock, std::string("40 55 41 54 41 55 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05"));
	this->addResult(SigTable::GameMode_attack, std::string("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 48 8B F2 48 8B F9 45 33 F6 4C 89 75 08 48 8D 4D 08 E8 ? ? ? ? 90 C6 45"));
}
