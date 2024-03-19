#include "ActorOffsetProvider.h"

ActorOffsetProvider::ActorOffsetProvider() : OffsetProvider("Actor")
{
	this->addResult<int32_t>(OffsetTable::Actor_vft_swing, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 85 ff 74", 3);

    this->addResult(OffsetTable::Actor_entityData, 0x188);
    this->addResult(OffsetTable::Actor_renderId, 0x268);
    this->addResult(OffsetTable::Actor_isDestroying, 0x1988);
    this->addResult(OffsetTable::Actor_entityContext, 0x8);
    this->addResult<int32_t>(OffsetTable::Actor_ticksAlive, "FF 83 ? ? ? ? 0F 2F DC F3", 2);
    this->addResult<int32_t>(OffsetTable::Actor_dimension, "49 83 b8 ? ? ? ? ? 49 8b 88", 3);
    this->addResult<int32_t>(OffsetTable::Actor_level, "48 39 98 ? ? ? ? 74 ? 49 8b 06", 3);
    this->addResult<int32_t>(OffsetTable::Actor_stateVectorComponent, "48 8b 90 ? ? ? ? 48 85 d2 74 ? 85 c9", 3);
    this->addResult<int32_t>(OffsetTable::Actor_aabbComponent, "4D 8B B4 24 ? ? ? ? 4C 89 75 88 4D 85 F6", 4);
    this->addResult<int32_t>(OffsetTable::Actor_rotationComponent, "48 8B 83 ? ? ? ? 0F 28 C7", 3);
    this->addResult<int32_t>(OffsetTable::Actor_distanceTraveledComponent, "48 8b 80 ? ? ? ? 48 85 c0 74 ? f3 0f 10 48", 3);
    this->addResult<int32_t>(OffsetTable::Actor_dead, "40 38 aa ? ? ? ? 74 ? 33 d2", 3);
    this->addResult<int32_t>(OffsetTable::Actor_armorContainer, "48 8b 89 ? ? ? ? 48 8b 01 33 d2 48 8b 40", 3);
    this->addResult<int32_t>(OffsetTable::Actor_handContainer, "48 8b 88 ? ? ? ? 48 8b 01 48 8b 40 ? ff 15 ? ? ? ? 48 8b 8b", 3);

    // TODO: Move to GamemodeOffsetProvider
    this->addResult(OffsetTable::GameMode_breakProgress, 0x24);
}