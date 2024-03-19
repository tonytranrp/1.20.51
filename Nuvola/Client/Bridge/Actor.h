#pragma once

#undef max
#include "Macro.h"

#include "Components/Flags/HorizontalCollisionFlag.h"
#include "Components/Flags/JumpFromGroundRequestFlag.h"
#include "Components/Flags/OnGroundFlag.h"

#include "Components/AABBShapeComponent.h"
#include "Components/ActorDefinitionIdentifierComponent.h"
#include "Components/ActorOwnerComponent.h"
#include "Components/ActorRotationComponent.h"
#include "Components/ActorTypeComponent.h"
#include "Components/AttributesComponent.h"
#include "Components/FallDistanceComponent.h"
#include "Components/FlagComponent.h"
#include "Components/RenderPositionComponent.h"
#include "Components/StateVectorComponent.h"

#include "Nuvola/Bridge.h"

#include "ActorType.h"
#include "ArmorItem.h"
#include "Dimension.h"
#include "EntityContext.h"
#include "HashedString.h"
#include "SimpleContainer.h"
#include "SynchedActorDataEntityWrapper.h"
#include "Types.h"

#include <Math/Math.h>
#include <gsl/gsl>

#include <memory>
#include <string>

class Actor {
	Sizer<0x4B4> classSize;

public:
	/* Fields */
	DEFINE_MEMBER(EntityContext, entityContext, OffsetTable::Actor_entityContext);
	DEFINE_MEMBER(SynchedActorDataEntityWrapper, entityData, OffsetTable::Actor_entityData);
	DEFINE_MEMBER(int64_t, ticksAlive, OffsetTable::Actor_ticksAlive);
	DEFINE_MEMBER(std::shared_ptr<Dimension>, dimension, OffsetTable::Actor_dimension);
	DEFINE_MEMBER(class Level*, level, OffsetTable::Actor_level);
	DEFINE_MEMBER(HashedString, renderId, OffsetTable::Actor_renderId);
	DEFINE_MEMBER(StateVectorComponent*, stateVectorComponent, OffsetTable::Actor_stateVectorComponent);
	DEFINE_MEMBER(AABBShapeComponent*, aabbComponent, OffsetTable::Actor_aabbComponent);
	DEFINE_MEMBER(ActorRotationComponent*, rotationComponent, OffsetTable::Actor_rotationComponent);
	DEFINE_MEMBER(bool, dead, OffsetTable::Actor_dead);
	DEFINE_MEMBER(class SimpleContainer*, armorContainer, OffsetTable::Actor_armorContainer);
    DEFINE_MEMBER(class SimpleContainer*, handContainer, OffsetTable::Actor_handContainer);
    DEFINE_MEMBER(bool, isDestroying, OffsetTable::Actor_isDestroying);


    /* Virtuals */
	//virtual ~Actor();
	const Vector3<float>& getPosition()
	{
		return stateVectorComponent->currentPos;
	}
	[[nodiscard]] const std::string& getNameTag() const
	{
		auto& entityData = this->getEntityData();
		auto* synchedData = entityData.getData();
		return synchedData->getString(4); //4 is the name tag data id
	}
	bool isImmobile()
	{
		return vftCall<49, bool, Actor*>(this);
	}
	bool canAttack(Actor* target, bool param_2)
	{
		return vftCall<64, bool, Actor*, Actor*, bool>(this, target, param_2);
	}
	ItemStack& getArmor(ArmorSlot slot)
	{
		return this->armorContainer->stacks[static_cast<int32_t>(slot)];
	}

	void swing()
	{
		return vftCall<OffsetTable::Actor_vft_swing, void, Actor*>(this);
	}

	bool isInWater() {
		//return vftCall<64, bool, Actor*>(this); not a vtable func anymore
		return false;
	}

	/* Functions */
	[[nodiscard]] auto getEntityId() const
	{
		return this->entityContext.id;
	}

	static Actor* tryGetFromEntity(EntityContext& entityContext) {
		auto component = entityContext.tryGetComponent<ActorOwnerComponent>();
		if(component)
		{
			return component->actor;
		}
		return nullptr;
	}

	[[nodiscard]] SynchedActorDataEntityWrapper& getEntityData() const {
		return this->entityData;
	}

	bool isMob()
	{
		const auto component = entityContext.tryGetComponent<ActorTypeComponent>();
		if (component != nullptr)
		{
			const ActorType type = component->type;
			return (type & ActorType::Mob) == ActorType::Mob;
		}
		return false;
	}

	std::string getCanonicalName()
	{
		auto component = entityContext.tryGetComponent<ActorDefinitionIdentifierComponent>();
		if (component != nullptr)
		{
			return component->identifier.canonicalName.get();
		}
		return "";
	}

	bool isCollidingHorizontal()
	{
		return entityContext.hasComponent<FlagComponent<HorizontalCollisionFlag>>();
	}

	bool isOnGround()
	{
		return entityContext.hasComponent<FlagComponent<OnGroundFlag>>();
	}

	void jumpFromGround(void* legacyBlocksource = nullptr)
	{
		entityContext.getOrAddComponent<FlagComponent<JumpFromGroundRequestFlag>>();
	}

	BlockPos getBlockPos()
	{
		BlockPos blockPos;
		const Vector3<float>& currentPos = this->getPosition();
		blockPos.x = (int)floor(currentPos.x);
		blockPos.y = (int)floor(currentPos.y - (float)2.0f);
		blockPos.z = (int)floor(currentPos.z);
		return blockPos;
	}
	ChunkPos getChunkPos()
	{
		return {this->getBlockPos()};
	}

	Vector3<float> getRenderPos()
	{
		auto component = entityContext.tryGetComponent<RenderPositionComponent>();
		if (component != nullptr)
		{
			return component->pos;
		}
		return {0, 0, 0};
	}

	AABB<float> getRenderAABB()
	{
		AABB<float> result;
		auto* aabbComponent = this->aabbComponent;
		if (aabbComponent == nullptr)
		{
			return result;
		}
		
		//Get the width
		float width = aabbComponent->width;
		float halfWidth = width * 0.5f;

		//Get the render pos
		Vector3<float> renderPos = this->getRenderPos();
		Vector3<float> realPos = this->getPosition();
		float yDiff = realPos.y - renderPos.y;

		//Make the width of the AABB
		result.firstCorner = renderPos - Vector3<float>(halfWidth, 0, halfWidth);
		result.secondCorner = renderPos + Vector3<float>(halfWidth, 0, halfWidth);

		//Adjust the height, the render pos for players is eye level
		result.firstCorner.y = aabbComponent->aabb.firstCorner.y - yDiff;
		result.secondCorner.y = aabbComponent->aabb.secondCorner.y - yDiff;

		return result;
	}

	[[nodiscard]] bool isAlive() const
	{
		return !this->dead;
	}

	[[nodiscard]] float getFallDistance()
	{
		auto component = entityContext.tryGetComponent<FallDistanceComponent>();
		if(component)
		{
			return component->fallDistance;
		}
		return 0.0f;
	}

	gsl::not_null<BaseAttributeMap*> getAttributes() {
		auto component = this->entityContext.getOrAddComponent<AttributesComponent>();
		return component.map;
	}

	void setOnGround(bool value)
	{
		if(value)
		{
			entityContext.getOrAddComponent<FlagComponent<OnGroundFlag>>();
			return;
		}
		entityContext.removeComponent<FlagComponent<OnGroundFlag>>();
	}

	void setPosition(const Vector3<float>& pos)
	{
		this->stateVectorComponent->currentPos = pos;
	}
};

static_assert(sizeof(Actor) == 0x4B4);