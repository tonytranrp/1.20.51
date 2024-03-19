#pragma once
#include <Data/ManagedItem.h>

#include <any>
#include <mutex>
#include <vector>

#define COUNT(a) (sizeof(a) / sizeof(*a))

enum class SettingType
{
    TOGGLE,
	SLIDER_FLOAT,
	SLIDER_INT,
	TEXT,
	ENUM,
	ENTITY_TYPES,
	BLOCK_TYPES,
	GROUP,
	ENCHANT_WEIGHTS
};


class Setting : public ManagedItem {
	std::any minimumValue;
	std::any maximumValue;
	std::any defaultValue;
    std::any value;
	std::vector<std::string> enumStrs;
    SettingType type;
	std::string description;
	std::mutex valueLock;
public:
	Setting(std::string name, std::string description, SettingType type, std::any defaultValue, std::any minimumValue, std::any maximumValue, std::vector<std::string> enumStrs = {}) : ManagedItem(name) {
		this->type         = type;
		this->maximumValue = maximumValue;
		this->minimumValue = minimumValue;
		this->defaultValue = defaultValue;
		this->value        = defaultValue;
		this->enumStrs = enumStrs;
		this->description = description;
	}
	std::any getValue() {
		std::lock_guard<std::mutex> guard(valueLock);
		return this->value;
	}
	template<typename T>
	T getValueAs() {
		std::lock_guard<std::mutex> guard(valueLock);
		try {
			return std::any_cast<T>(this->value);
		}
		catch (std::exception& ex) {
			return T();
		}
	}
	void setValue(std::any value) {
		std::lock_guard<std::mutex> guard(valueLock);
		this->value = value;
	}
	std::any getMinimum() {
		return this->minimumValue;
	}
	std::any getMaximum() {
		return this->maximumValue;
	}
	SettingType getType() {
		return this->type;
	}
	const std::vector<std::string>& getEnumStrs() {
		return this->enumStrs;
	}
	const std::string& getDescription()
	{
		return this->description;
	}
	void resetToDefault()
	{
		std::lock_guard<std::mutex> guard(valueLock);
		this->value = this->defaultValue;
	}
};
