#pragma once

#include <cstdint>

class Ability
{
public:
	enum class Options : int8_t
	{
		NONE = 0,
		HIDDEN = 1,
		COMMAND = 2,
	};
	friend inline constexpr Ability::Options operator&(Ability::Options x, Ability::Options y)
	{
		return static_cast<Ability::Options>(static_cast<uint32_t>(x) & static_cast<uint32_t>(y));
	}

	enum class Type : int8_t
	{
		INVALID = 0,
		UNSET = 1,
		BOOL = 2,
		FLOAT = 3
	};

	explicit Ability() : type(Type::INVALID), valueFloat(0), options(Options::NONE) {}
	explicit Ability(Type type) : type(type), valueFloat(0), options(Options::NONE) {}
	explicit Ability(bool value, Options options) : type(Type::BOOL), valueBool(value), options(options) {}

	[[nodiscard]] bool getBool() const
	{
		if(this->type != Type::BOOL)
		{
			return false;
		}
		return this->valueBool;
	}
	[[nodiscard]] float getFloat() const
	{
		if(this->type != Type::FLOAT)
		{
			return 0.0f;
		}
		return this->valueFloat;
	}

	[[nodiscard]] bool hasOption(Ability::Options valB) const
	{
		return (options & valB) != Options::NONE;
	}

	[[nodiscard]] bool isSet() const
	{
		return this->type != Type::UNSET;
	}

	bool operator!=(const Ability& other) const
	{
		if (type != other.type || options != other.options)
			return true;

		if (type == Type::BOOL)
			return valueBool != other.valueBool;

		else if (type == Type::FLOAT)
			return valueFloat != other.valueFloat;

		return false;
	}

	void setBool(bool value)
	{
		this->type = Type::BOOL;
		this->valueBool = value;
	}

	void setFloat(float value)
	{
		this->type = Type::FLOAT;
		this->valueFloat = value;
	}

	void unSet()
	{
		this->type = Type::UNSET;
	}

private:
	Type type; //0x0000
	union //0x0004
	{
		bool valueBool; //0x0000
		float valueFloat;
	};
	Ability::Options options; //0x0008
}; //Size: 0x000C
static_assert(sizeof(Ability) == 0xC);