#pragma once

#include "Abilities.h"
#include "ActorType.h"
#include "BlockSource.h"
#include "Macro.h"

#include <Math/Math.h>
#include <vector>

struct IActorMovementProxy
{
	virtual ~IActorMovementProxy() {};
	bool isFlying()
	{
		return vftCall<38, bool, IActorMovementProxy*>(this);
	}
	auto setFlying(bool b)
	{
		return this->setAbilityBool(AbilitiesIndex::Flying, b);
	}
	auto setFlightSpeed(float f)
	{
		return this->setAbilityFloat(AbilitiesIndex::FlySpeed, f);
	}
	bool isOnGround()
	{
		return vftCall<39, bool, IActorMovementProxy*>(this);
	}
	void setOnGround(bool value)
	{
		return vftCall<40, void, IActorMovementProxy*, bool>(this, value);
	}

	bool isInWater()
	{
		return vftCall<5, bool, IActorMovementProxy*>(this);
	}

	void setPosition(const Vector3<float>& position)
	{
		return vftCall<66, void, IActorMovementProxy*, const Vector3<float>&>(this, position);
	}
	void setRotation(const Vector2<float>& rotation)
	{
		return vftCall<74, void, IActorMovementProxy*, const Vector2<float>&>(this, rotation);
	}
	float getFallDistance()
	{
		return vftCall<76, float, IActorMovementProxy*>(this);
	}
	float getJumpPower() const
	{
		return vftCall<120, float, const IActorMovementProxy*>(this);
	}
	void setSprinting(bool value)
	{
		return vftCall<128, void, IActorMovementProxy*, bool>(this, value);
	}
	void jumpFromGround(const BlockSource* blockSource)
	{
		return vftCall<136, void, IActorMovementProxy*, const BlockSource*>(this, blockSource);
	}
	void setYHeadRot(float yaw)
	{
		return vftCall<150, void, IActorMovementProxy*, float>(this, yaw);
	}
	bool getAbilityBool(AbilitiesIndex index)
	{
		return vftCall<192, bool, IActorMovementProxy*>(this, index);
	}
	void setAbilityBool(AbilitiesIndex index, bool b)
	{
		return vftCall<193, void, IActorMovementProxy*>(this, index, b);
	}
	float getAbilityFloat(AbilitiesIndex index)
	{
		return vftCall<194, float, IActorMovementProxy*>(this, index);
	}
	void setAbilityFloat(AbilitiesIndex index, float f)
	{
		return vftCall<195, void, IActorMovementProxy*>(this, index, f);
	}
};