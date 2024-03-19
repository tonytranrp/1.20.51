#pragma once

#include "ThemeColor.h"
#include "ThemeIcon.h"

#include <Data/ManagedItem.h>

#include <Math/Math.h>
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>

#include <functional>
#include <map>
#include <optional>
#include <variant>

struct IconGlyph
{
	std::string glyph;
	int offsetX = 0;
	int offsetY = 0;
	std::optional<float> size;

	IconGlyph() = default;
	static IconGlyph fromString(std::string glyph, int offsetX = 0, int offsetY = 0, std::optional<float> size = {});
	operator nlohmann::json() const;
	operator std::string() const;
};

class ThemeVariable
{
public:
	ThemeVariable() {};
	virtual std::shared_ptr<ThemeVariable> buildNew(const std::vector<std::string>& argTokens) { return std::make_shared<ThemeVariable>(); };
	virtual void nextFrame() {};
	virtual std::string getName() { return "themevar"; };
	virtual ColorF getColor() { return ColorF(); };
	operator ColorF() { return this->getColor(); };
};

typedef std::function<ColorF(ThemeColor, ColorF)> ThemeFilter;
class Theme : public ManagedItem
{
protected:
	std::string name;
	std::string author;

	std::map<const ThemeIcon, IconGlyph> icons;
	std::vector<std::shared_ptr<ThemeVariable>> variables;
	std::map<const ThemeColor, std::variant<ColorF, std::shared_ptr<ThemeVariable>>> colors;
	std::vector<void*> textures;

	std::vector<std::string> errors;
	std::vector<std::string> warnings;
public:
	Theme();
	Theme(const Theme&);
	Theme& operator=(const Theme&);
	~Theme() override;
	Theme(Theme&&);
	Theme& operator=(Theme&&);

	void addError(const std::string& errorMsg);
	[[nodiscard]] const std::vector<std::string>& getErrors() const;
	void addWarn(const std::string& message);
	[[nodiscard]] const std::vector<std::string>& getWarnings() const;
	void nextFrame();

	virtual nlohmann::json serialize();
	virtual void deserialize(nlohmann::json data);

	virtual Theme interpolate(Theme, float);
	virtual bool hasVariable(std::string name);
	virtual std::shared_ptr<ThemeVariable> getVariable(std::string name);

	[[nodiscard]] virtual std::string getName() const;
	virtual void setName(std::string name);
	[[nodiscard]] virtual std::string getAuthor() const;
	virtual void setAuthor(std::string author);
	[[nodiscard]] virtual bool madeByUser() const;
	virtual bool hasTexture(int index);
	virtual void* getTexture(int index);
	template<typename T>
	T getTextureAs(int index)
	{
		return reinterpret_cast<T>(this->getTexture(index));
	}

	void pushFilter(ThemeFilter filter);
	void popFilter();

	const IconGlyph& operator[](ThemeIcon icon);
	ColorF operator[](ThemeColor color);
};