#pragma once

//https://github.com/ambiennt/ElementZero/blob/44c0ece220c0f9295531238c9e8aac60874b4919/MinecraftHeaders/Actor/Attribute.h

#include "Bedrock/mce.h"
#include "ActorUniqueID.h"
#include "HashedString.h"

#include <cstdint>
#include <memory>
#include <vector>

class Player;
class Mob;
class BaseAttributeMap;

enum class AttributeID : uint32_t {
	PlayerHunger                    = 1,
	PlayerSaturation                = 2,
	PlayerExhaustion                = 3,
	PlayerLevel                     = 4,
	PlayerExperience                = 5,
	ZombieSpawnReinforcementsChange = 6,
	Health                          = 7,
	FollowRange                     = 8,
	KnockbackResistance             = 9,
	MovementSpeed                   = 10,
	UnderwaterMovementSpeed         = 11,
	LavaMovementSpeed               = 12,
	AttackDamage                    = 13,
	Absorption                      = 14,
	Luck                            = 15,
	JumpStrength                    = 16,
};

enum class AttributeBuffType : int32_t {
	Hunger = 0x0,
	Saturation = 0x1,
	Regeneration = 0x2,
	Heal = 0x3,
	Harm = 0x4,
	Magic = 0x5,
	Wither = 0x6,
	Poison = 0x7,
	FatalPoison = 0x8,
	SelfHeal = 0x9,
	Unknown = 0xA,
	None = 0xB,
};

enum class RedefinitionMode : int8_t {
	KeepCurrent = 0x0,
	UpdateToNewDefault = 0x1,
};

enum class AttributeModifierOperation : int32_t {
	OPERATION_ADDITION = 0x0,
	OPERATION_MULTIPLY_BASE = 0x1,
	OPERATION_MULTIPLY_TOTAL = 0x2,
	OPERATION_CAP = 0x3,
	TOTAL_OPERATIONS = 0x4,
	OPERATION_INVALID = 0x4,
};

enum class AttributeOperands : int32_t {
	OPERAND_MIN = 0x0,
	OPERAND_MAX = 0x1,
	OPERAND_CURRENT = 0x2,
	TOTAL_OPERANDS = 0x3,
	OPERAND_INVALID = 0x3,
};

class Amplifier {
public:

	virtual ~Amplifier() = default;
	virtual float getAmount(int32_t amplification, float scale) = 0;
	virtual bool shouldBuff(int32_t remainingDuration, int32_t amplification) = 0;
	virtual int32_t getTickInterval(int32_t amplification) = 0;
};
static_assert(sizeof(Amplifier) == 0x8);

class Attribute {
public:
	RedefinitionMode mRedefinitionMode; // 0x0
	bool mSyncable; // 0x1
	uint32_t mIDValue; // 0x4
	HashedString mName; // 0x8
};
static_assert(sizeof(Attribute) == 0x38);

struct AttributeBuffInfo {
	AttributeBuffType mType; // 0x0
	ActorUniqueID mSource; // 0x8
};
static_assert(sizeof(AttributeBuffInfo) == 0x10);

class AttributeBuff {
public:
	float mAmount; // 0x8
	AttributeBuffInfo mInfo; // 0x10
	std::shared_ptr<Amplifier> mValueAmplifier; // 0x20
	std::shared_ptr<Amplifier> mDurationAmplifier; // 0x30
	float mScale; // 0x40
	int32_t mAmplification; // 0x44
	uint64_t mId; // 0x48
	int32_t mOperand; // 0x50

	virtual ~AttributeBuff() = default;
	virtual bool isInstantaneous() const = 0;
	virtual bool isSerializable() const = 0;
	virtual void setDurationAmplifier(std::shared_ptr<Amplifier> amplifier) = 0;
};
static_assert(sizeof(AttributeBuff) == 0x58);

class InstantaneousAttributeBuff : public AttributeBuff {
public:

	/*InstantaneousAttributeBuff(float amount, AttributeBuffType type) {
		this->mAmount = amount;
		this->mInfo.mType = type;
		this->mInfo.mSource = ActorUniqueID(-1);
		this->mScale = 1.f;
		this->mId = 0;
		this->mOperand = 2;
	}*/

	virtual ~InstantaneousAttributeBuff() = default;
	virtual bool isInstantaneous() const = 0;
	virtual bool isSerializable() const = 0;
};
static_assert(sizeof(InstantaneousAttributeBuff) == 0x58);

class TemporalAttributeBuff : public AttributeBuff {
public:
	int32_t mDuration; // 0x58
	int32_t mLifeTimer; // 0x5C
	float mBaseAmount; // 0x60
	bool mIsSerializable; // 0x64

	virtual ~TemporalAttributeBuff() = default;
	virtual bool isInstantaneous() const = 0;
	virtual bool isSerializable() const = 0;
	virtual void setDurationAmplifier(std::shared_ptr<Amplifier> amplifier) = 0;
};
static_assert(sizeof(TemporalAttributeBuff) == 0x68);

class AttributeModifier {
public:
	float mAmount; // 0x8
	AttributeModifierOperation mOperation; // 0xC
	AttributeOperands mOperand; // 0x10
	std::string mName; // 0x18
	mce::UUID mId; // 0x38
	bool mSerialize; // 0x48

	virtual ~AttributeModifier() = default;
	virtual bool isInstantaneous() = 0;
};
static_assert(sizeof(AttributeModifier) == 0x50);

class AttributeInstanceHandle {
public:
	uint32_t mAttributeID; // 0x0
	BaseAttributeMap* mAttributeMap; // 0x8
};
static_assert(sizeof(AttributeInstanceHandle) == 0x10);

class AttributeInstanceDelegate {
public:
	AttributeInstanceHandle mAttributeHandle; // 0x8

	virtual ~AttributeInstanceDelegate() = default;
	virtual void tick() = 0;
	virtual void notify(int64_t type) = 0;
	virtual void change(float oldValue, float newValue, AttributeBuffInfo buffInfo) = 0;
	virtual void getBuffValue(const AttributeBuff &buff) = 0;
};
static_assert(sizeof(AttributeInstanceDelegate) == 0x18);

class AttributeInstance {
public:
	BaseAttributeMap *mAttributeMap; // 0x8
	const Attribute *mAttribute; // 0x10
	std::vector<AttributeModifier> mModifierList; // 0x18
	std::vector<TemporalAttributeBuff> mTemporalBuffs; // 0x30
	std::vector<AttributeInstanceHandle> mListeners; // 0x48
	std::shared_ptr<AttributeInstanceDelegate> mDelegate; // 0x60
	union {
		struct {
			float mDefaultMinValue;
			float mDefaultMaxValue;
			float mDefaultValue;
		};
		float mDefaultValues[3]; // 0x70
	};
	union {
		struct {
			float mCurrentMinValue;
			float mCurrentMaxValue;
			float mCurrentValue;
		};
		float mCurrentValues[3]; // 0x7C
	};

	virtual ~AttributeInstance() = default;
	//virtual void tick() = 0;

	//void addBuff(const AttributeBuff &buff) = 0;
};
static_assert(sizeof(AttributeInstance) == 0x88);