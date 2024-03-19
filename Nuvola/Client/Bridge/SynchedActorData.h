#pragma once

#include "Bedrock/Utils.h"
#include "ActorFlags.h"

#include <Math/Math.h>

#include <cstdint>
#include <vector>

#pragma pack(push, 1)
class DataItem
{
public:
	enum class Type : int8_t
	{
		NETWORK_ID = -1,
		BYTE = 0,
		INT = 2,
		FLOAT = 3,
		STRING = 4,
		NBT = 5,
		BLOCKPOS = 6,
		INT64 = 7,
		VEC3 = 8
	};
	// Default value
	template<typename T> struct TypeMapper { static constexpr Type value = Type::BYTE; };

	template<> struct TypeMapper<int8_t> { static constexpr Type value = Type::BYTE; };
	template<> struct TypeMapper<int32_t> { static constexpr Type value = Type::INT; };
	template<> struct TypeMapper<float> { static constexpr Type value = Type::FLOAT; };
	template<> struct TypeMapper<std::string> { static constexpr Type value = Type::STRING; };
	template<> struct TypeMapper<Vector3<int>> { static constexpr Type value = Type::BLOCKPOS; };
	template<> struct TypeMapper<int64_t> { static constexpr Type value = Type::INT64; };
	template<> struct TypeMapper<Vector3<float>> { static constexpr Type value = Type::VEC3; };

	virtual ~DataItem() = delete;
	virtual bool isDataEqual(const DataItem& other)
	{
		return this->getType() == other.getType();
	}

	[[nodiscard]] inline Type getType() const
	{
		return this->type;
	}

	[[nodiscard]] inline int8_t getInt8() const
	{
		return this->value.valueInt8;
	}
	[[nodiscard]] inline int32_t getInt() const
	{
		return this->value.valueInt;
	}
	[[nodiscard]] inline float getFloat() const
	{
		return this->value.valueFloat;
	}
	[[nodiscard]] inline const std::string& getString() const
	{
		return this->value.valueString;
	}
	[[nodiscard]] inline const BlockPos& getBlockPos() const
	{
		return this->value.valueBlockPos;
	}
	[[nodiscard]] inline int64_t getInt64() const
	{
		return this->value.valueInt64;
	}
	[[nodiscard]] inline ActorFlags getStatusFlags() const
	{
		return this->value.valueStatusFlag;
	}
	[[nodiscard]] inline Vector3<float> getVec3() const
	{
		return this->value.valueVec3;
	}
//protected:
	Type type = Type::BYTE;
	uint8_t unknown;
	uint16_t field_a;
	bool dirty = false;
	char pad[3];

	union
	{
		int8_t valueInt8;
		int32_t valueInt;
		float valueFloat;
		std::string valueString;
		BlockPos valueBlockPos;
		int64_t valueInt64;
		ActorFlags valueStatusFlag;
		Vector3<float> valueVec3 = {0,0,0};
	} value;
};
static_assert(offsetof(DataItem, value.valueString) == 0x10);

class SynchedActorData
{
	std::vector<DataItem*> data;
	uint16_t field_18 = 0;
	uint16_t field_1a = 0;

	/*
	 * Creates a stack clone of the instance
	 */
	SynchedActorData _clone()
	{
		auto clone = *this;
		return clone;
	}

	/*
	 * Safely finds a piece of data by checking if it is available first before accessing it
	 */
	[[nodiscard]] DataItem* _find(uint16_t dataId) const
	{
		if(this->data.size() <= dataId)
		{
			return nullptr;
		}
		return this->_get(dataId);
	}

	/*
	 * Grabs a piece of data without safely checking if it is available
	 * use _find for added safety checks.
	 */
	[[nodiscard]] DataItem* _get(uint16_t dataId) const
	{
		return this->data.at(dataId);
	}

	/*
	 * Resizes the internal data vector to fit a given data id
	 */
	void _resizeToContain(uint16_t dataId)
	{
		if(dataId <= this->data.size())
		{
			this->data.resize(dataId+1);
		}
	}

public:
	/*
	 * Safely read an int64 value from a data id
	 */
	int8_t getInt8(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::BYTE)
			{
				return item->getInt8();
			}
		}
		return 0;
	}

	/*
	 * Safely read an int value from a data id
	 */
	int32_t getInt(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::INT)
			{
				return item->getInt();
			}
		}
		return 0;
	}

	/*
	 * Safely read a float value from a data id
	 */
	float getFloat(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::FLOAT)
			{
				return item->getFloat();
			}
		}
		return 0.0f;
	}

	/*
	 * Safely read a string value from a data id
	 */
	const std::string& getString(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::STRING)
			{
				return item->getString();
			}
		}
		return Bedrock::EMPTY_STRING;
	}

	/*
	 * Safely read an int64 value from a data id
	 */
	int64_t getInt64(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::INT64)
			{
				return item->getInt64();
			}
		}
		return 0;
	}

	/*
	 * Safely read a status flag value from a data id
	 */
	bool getStatusFlag(uint16_t dataId, ActorFlags flags)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr)
			{
				return (item->getStatusFlags() & flags) == flags;
			}
		}
		return false;
	}

	/*
	 * Safely read a Vector3<float> value from a data id
	 */
	Vector3<float> getVec3(uint16_t dataId)
	{
		if(dataId < this->data.size())
		{
			auto item = this->_get(dataId);
			if(item != nullptr && item->getType() == DataItem::Type::VEC3)
			{
				return item->getVec3();
			}
		}
		return {0,0,0};
	}

	/*
	 * Check if a given dataId is available
	 */
	bool hasData(uint16_t dataId)
	{
		return this->_find(dataId) != nullptr;
	}

	/*
	 * Mark a given dataId as dirty
	 */
	void markDirty(uint16_t dataId)
	{
		auto item = this->_get(dataId);
		auto* plVar1 = &this->field_18;
		auto local_18 = item->field_a;
		item->dirty = true;
		auto plVar3 = &local_18;
		if (*plVar1 < local_18) {
			plVar3 = plVar1;
		}
		*plVar1 = *plVar3;
		auto puVar2 = &this->field_1a;
		auto puVar4 = &local_18;
		if (local_18 < *puVar2) {
			puVar4 = puVar2;
		}
		*puVar2 = *puVar4;
	}

	/*
	 * Set any given value
	 */
	template<typename type_t>
	void set(uint16_t dataId, const type_t& value)
	{
		auto item = this->_get(dataId);
		if(item != nullptr && item->getType() == DataItem::TypeMapper<type_t>::value && item->value == value)
		{
			item->value = value;
			this->markDirty(dataId);
		}
	}

	/*
	 * Set a status flag value
	 */
	void setStatusFlag(ActorFlags flags, bool param_2)
	{

	}
};
#pragma pack(pop)

static_assert(sizeof(SynchedActorData) == 0x1C);