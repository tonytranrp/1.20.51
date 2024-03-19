#pragma once

#include "ActorFactory.h"
#include "ActorManager.h"
#include "BlockPalette.h"
#include "BlockTypeRegistry.h"
#include "Player.h"
#include "WeakEntityRef.h"

#include <Math/Math.h>

enum class HitResultType : int32_t
{
	BLOCK,
	ENTITY,
	ENTITY_OUT_OF_RANGE,
	AIR
};

class SRaycastResult
{
public:
	Vector3<float> liquidHitLocation; //0x0000
	Vector3<float> raycastDirection; //0x0000
	HitResultType type; //0x0018
	int32_t selectedFace; //0x001C
	Vector3<int> blockPos;
	Vector3<float> endPos; //0x002C
	WeakEntityRef entity; //0x0038
	bool isLiquid; //0x004C
	char pad_004D[3]; //0x004D
	int32_t liquidFace; //0x0050
	Vector3<int> liquidBlockPos; //0x0054
	Vector3<float> liquidPos; //0x0060
	bool indirectHit; //0x006C
	char pad_006D[3]; //0x006D
}; //Size: 0x0070
static_assert(sizeof(SRaycastResult) == 0x70);


struct Level {
	DEFINE_MEMBER(BlockPalette*, mBlockPalette, OffsetTable::Level_mBlockPalette);
	DEFINE_MEMBER(SRaycastResult, mHitResult, OffsetTable::Level_mHitResult);
	DEFINE_MEMBER(ActorFactory*, mActorFactory, OffsetTable::Level_mActorFactory);
	DEFINE_MEMBER(ActorManager*, mActorManager, OffsetTable::Level_mActorManager);

	/* Virtuals */
	virtual ~Level() { };
	void forEachPlayer(std::function<bool(__cdecl)(Player&)> callback) const
	{
		return vftCall<204, void, const Level*, std::function<bool(__cdecl)(Player&)>>(this, callback);
	}
	
	std::weak_ptr<BlockTypeRegistry> getBlockRegistry() const
	{
		return vftCall<349, std::weak_ptr<BlockTypeRegistry>, Level>(this);
	}

	/* Functions */
};