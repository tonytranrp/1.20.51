#include "SystemImplSigProvider.h"

SystemImplSigProvider::SystemImplSigProvider() : SigProvider("SystemImpl")
{
	this->addResult(SigTable::ItemUseSlowdownSystemImpl_doItemUseSlowdownSystem, "48 89 5C 24 ? 57 48 83 EC 30 49 8B D8 4D 85 C9");
	this->addResult(SigTable::MobJumpFromGroundSystemImpl_mobJumpFromGround, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B BC 24");
}