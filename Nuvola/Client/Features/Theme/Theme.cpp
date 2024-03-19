#include "Theme.h"

#include "ThemeManager.h"

#include "Variables/DefinedVariable.h"

#include <Graphics/imfx.h>

#include <regex>

static std::vector<ThemeFilter> filterStack;
IconGlyph IconGlyph::fromString(std::string glyph, int offsetX, int offsetY, std::optional<float> size)
{
	int codePoint = std::stoi(glyph, nullptr, 16);

	// Encode code point as UTF-8 byte sequence
	std::string utf8Bytes;
	if (codePoint <= 0x7f) {
		utf8Bytes += static_cast<char>(codePoint & 0x7f);
	}
	else if (codePoint <= 0x7ff) {
		utf8Bytes += static_cast<char>(0xc0 | ((codePoint >> 6) & 0x1f));
		utf8Bytes += static_cast<char>(0x80 | (codePoint & 0x3f));
	}
	else if (codePoint <= 0xffff) {
		utf8Bytes += static_cast<char>(0xe0 | ((codePoint >> 12) & 0x0f));
		utf8Bytes += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3f));
		utf8Bytes += static_cast<char>(0x80 | (codePoint & 0x3f));
	}

	IconGlyph newGlyph;
	newGlyph.glyph = utf8Bytes;
	newGlyph.offsetX = offsetX;
	newGlyph.offsetY = offsetY;
	newGlyph.size = size;
	return newGlyph;
}

IconGlyph::operator nlohmann::json() const
{
	if (this->offsetX == 0 && this->offsetY == 0)
	{
		nlohmann::json data = this->glyph;
		return data;
	}

	nlohmann::json data = nlohmann::json::object();
	data["glyph"] = this->glyph;
	if (this->offsetX != 0)
		data["offset_x"] = this->offsetX;
	if (this->offsetY != 0)
		data["offset_y"] = this->offsetY;
	if (size.has_value())
		data["size"] = *this->size;

	return data;
}
IconGlyph::operator std::string() const
{
	return this->glyph;
}

Theme::Theme() : ManagedItem("Theme")
{
	this->colors = {};
}

Theme::Theme(const Theme& other) : ManagedItem(other.getName())
{
	*this = other;
}

Theme& Theme::operator=(const Theme& other)
{
	this->author = other.author;
	this->colors = other.colors;
	this->errors = other.errors;
	this->icons = other.icons;
	this->name = other.name;
	this->textures = other.textures;
	this->variables = other.variables;
	this->warnings = other.warnings;
	return *this;
}

Theme::~Theme() = default;

Theme::Theme(Theme&& other) : ManagedItem(other.getName())
{
	*this = std::move(other);
}

Theme& Theme::operator=(Theme&& other)
{
	this->author = other.author;
	this->colors = other.colors;
	this->errors = other.errors;
	this->icons = other.icons;
	this->name = other.name;
	this->textures = other.textures;
	this->variables = other.variables;
	this->warnings = other.warnings;
	return *this;
}

void Theme::addError(const std::string& errorMsg)
{
	this->errors.push_back(errorMsg);
}
const std::vector<std::string>& Theme::getErrors() const
{
	return this->errors;
}
void Theme::addWarn(const std::string& message)
{
	this->warnings.push_back(message);
}
const std::vector<std::string>& Theme::getWarnings() const
{
	return this->warnings;
}
void Theme::nextFrame()
{
	assert(filterStack.size() == 0);

	for (const auto& [themeCol, colorRef] : this->colors)
	{
		if (std::holds_alternative<std::shared_ptr<ThemeVariable>>(colorRef))
		{
			auto var = std::get<std::shared_ptr<ThemeVariable>>(colorRef);
			var->nextFrame();
		}
	}
}

nlohmann::json Theme::serialize()
{
	nlohmann::json data;
	data["meta"] = nlohmann::json::object();
	if (!this->name.empty())
	{
		data["meta"]["name"] = this->name;
	}
	else
	{
		data["meta"]["name"] = "ThemeName";
	}
	if (!this->author.empty())
	{
		data["meta"]["author"] = this->author;
	}
	else
	{
		data["meta"]["author"] = "NO_USERNAME";
	}
	data["icons"] = nlohmann::json::object();
	for (const auto& [iconKey, iconVal] : this->icons)
	{
		data["icons"][magic_enum::enum_name(iconKey)] = iconVal;
	}
	data["colors"] = nlohmann::json::object();
	for (const auto& [colorKey, colorRef] : this->colors)
	{
		if (std::holds_alternative<ColorF>(colorRef))
		{
			data["colors"][magic_enum::enum_name(colorKey)] = std::get<ColorF>(colorRef);
		}
		if (std::holds_alternative<std::shared_ptr<ThemeVariable>>(colorRef))
		{
			std::shared_ptr<ThemeVariable> var = std::get<std::shared_ptr<ThemeVariable>>(colorRef);
			std::string variableRef = "{% ";
			variableRef += var->getName();
			variableRef += " %}";
			data["colors"][magic_enum::enum_name(colorKey)] = variableRef;
		}
	}
	return data;
}

void Theme::deserialize(nlohmann::json data)
{
	if (data.is_null() || !data.is_object())
	{
		return;
	}

	if (data.contains("meta"))
	{
		const nlohmann::json& meta = data["meta"];
		if (meta.contains("name"))
		{
			if(meta["name"].is_string())
				this->name = meta["name"].get<std::string>();
		}
		if (meta.contains("author"))
		{
			if (meta["author"].is_string())
				this->author = meta["author"].get<std::string>();
		}
	}

	if (data.contains("icons"))
	{
		const nlohmann::json& icons = data["icons"];
		for (auto enumVal : magic_enum::enum_values<ThemeIcon>())
		{
			auto iconName = magic_enum::enum_name(enumVal);
			if (icons.contains(iconName))
			{
				const nlohmann::json& data = icons[iconName];

				if (data.is_string())
				{
					this->icons[enumVal] = IconGlyph::fromString(data.get<std::string>());
					continue;
				}
				else if (data.is_object())
				{
					if (!data.contains("glyph"))
					{
						this->addError("IconGlyph data has no glyph field");
						continue;
					}
					if (!data["glyph"].is_string())
					{
						this->addError("IconGlyph::glyph must be a string");
						continue;
					}

					std::string glyphStr = data["glyph"].get<std::string>();
					int offsetX = 0;
					int offsetY = 0;
					std::optional<float> size;

					if (data.contains("offset_x"))
					{
						if (data["offset_x"].is_number_integer())
						{
							offsetX = data["offset_x"].get<int>();
						}
						else
						{
							this->addError("IconGlyph::offset_x must be an integer");
							continue;
						}
					}

					if (data.contains("offset_y"))
					{
						if (data["offset_y"].is_number_integer())
						{
							offsetY = data["offset_y"].get<int>();
						}
						else
						{
							this->addError("IconGlyph::offset_y must be an integer");
							continue;
						}
					}

					if (data.contains("size"))
					{
						if (data["size"].is_number())
						{
							size = data["size"].get<float>();
						}
						else
						{
							this->addError("IconGlyph::offset_y must be a number");
							continue;
						}
					}

					this->icons[enumVal] = IconGlyph::fromString(glyphStr, offsetX, offsetY, size);
				}
			}
			else
			{
				this->addWarn("Theme is missing the icon \"" + std::string(iconName) + "\"");
			}
		}
	}
	if (data.contains("variables"))
	{
		nlohmann::json jsonVars = data["variables"];
		if (!jsonVars.is_array())
		{
			this->addError("\"variables\" is not an array");
		}
		else
		{
			for (nlohmann::json& var : jsonVars)
			{
				if (!var.is_object())
				{
					this->addError("Variable \"" + var.dump() + "\" is not an object!");
				}

				if (!var.contains("name"))
				{
					this->addError("Variable \"" + var.dump() + "\" needs a \"name\" field!");
				}
				if (!var.contains("color"))
				{
					this->addError("Variable \"" + var.dump() + "\" needs a \"color\" field!");
				}

				if (var.contains("name") && var.contains("color"))
				{
					if (!var["name"].is_string())
					{
						this->addError("Variable \"" + var.dump() + "\" has a name field, but it is not a string!");
					}
					if (!var["color"].is_string())
					{
						this->addError("Variable \"" + var.dump() + "\" has a color field, but it is not a string of a hex color code!");
					}

					if (var["name"].is_string() && var["color"].is_string())
					{
						this->variables.emplace_back(
							std::make_shared<DefinedVariable>(
								var["name"].get<std::string>(),
								ColorF::parse(var["color"].get<std::string>())
							)
						);
					}
				}
			}
		}
	}
	if (data.contains("colors"))
	{
		const nlohmann::json& colors = data["colors"];
		for (auto enumVal : magic_enum::enum_values<ThemeColor>())
		{
			auto colorName = magic_enum::enum_name(enumVal);
			if (colors.contains(colorName))
			{
				std::string colorStr = colors[colorName];
				if ((colorStr.find("{%") != std::string::npos) && (colorStr.find("%}") != std::string::npos))
				{
					std::string result;
					std::string params;
					std::regex pattern(R"(\{%\s*(.*?)\s*(?:\((.*?)\))?\s*%\})");
					std::smatch match;

					if (std::regex_search(colorStr, match, pattern)) {
						result = match[1].str();
						params = match[2].str();
					}

					auto mgr = systems->getThemeManager();

					if (this->hasVariable(result))
					{
						this->colors[enumVal] = this->getVariable(result);
					}
					else if (mgr->hasBuiltInVar(result))
					{
						this->colors[enumVal] = mgr->buildBuiltIn(result, params);
					}
					else
					{
						this->addError("Color \"" + std::string(colorName) + "\" references a variable named \"" + result + "\" which does not exist.");
					}
				}
				else
				{
					this->colors[enumVal] = ColorF::parse(colors[colorName]);
				}
			}
			else
			{
				this->addWarn("Theme is missing the color \"" + std::string(colorName) + "\"");
			}
		}
	}
	if (data.contains("textures"))
	{
		const nlohmann::json& textures = data["textures"];
		if (textures.is_array())
		{
			for (const auto& texture : textures)
			{
				if (texture.is_object())
				{
					if (texture.contains("data"))
					{
						if (texture["data"].is_string())
						{
							std::string imageData = texture["data"].get<std::string>();
							ImTextureID texID = ImFX::UploadImageBase64(imageData);
							if (texID != nullptr)
							{
								this->textures.push_back(texID);
							}
							else
							{
								this->addError("A texture failed to load!");
							}
						}
						else
						{
							this->addError("A texture in \"textures\" has a \"data\" field that is not a string!");
						}
					}
					else
					{
						this->addError("A texture in \"textures\" is missing a \"data\" field!");
					}
				}
				else
				{
					this->addError("A texture in \"textures\" is not an object!");
				}
			}
		}
		else
		{
			this->addError("The \"textures\" field must be an array of objects!");
		}
	}
}

Theme Theme::interpolate(Theme other, float delta)
{
	Theme interpolated;
	interpolated.icons = other.icons;
	interpolated.textures = other.textures;
	for (auto color : magic_enum::enum_values<ThemeColor>())
	{
		auto& colorRef = this->colors[color];
		if (std::holds_alternative<ColorF>(colorRef))
		{
			ColorF colorVal = std::get<ColorF>(colorRef);
			interpolated.colors[color] = colorVal.interpolate(other[color], delta);
		}
		if (std::holds_alternative<std::shared_ptr<ThemeVariable>>(colorRef))
		{
			std::shared_ptr<ThemeVariable> colorVar = std::get<std::shared_ptr<ThemeVariable>>(colorRef);
			ColorF resolved = colorVar->getColor();
			interpolated.colors[color] = resolved.interpolate(other[color], delta);
		}
	}
	return interpolated;
}
bool Theme::hasVariable(std::string name)
{
	return this->getVariable(name) != nullptr;
}
std::shared_ptr<ThemeVariable> Theme::getVariable(std::string name)
{
	for (auto& variable : this->variables)
	{
		if (variable->getName() == name)
		{
			return variable;
		}
	}
	return nullptr;
}

std::string Theme::getName() const
{
	return this->name;
}

void Theme::setName(std::string name)
{
	this->name = name;
}

std::string Theme::getAuthor() const
{
	return this->author;
}

void Theme::setAuthor(std::string author)
{
	this->author = author;
}

bool Theme::madeByUser() const
{
	return false;//this->getAuthor() == currentUser->GetUsername();
}

bool Theme::hasTexture(int index)
{
	return this->textures.size() > index;
}
void* Theme::getTexture(int index)
{
	return this->textures.at(index);
}

void Theme::pushFilter(ThemeFilter filter)
{
	filterStack.push_back(filter);
}
void Theme::popFilter()
{
	filterStack.pop_back();
}

#include <Fonts/IconsFontAwesome5.h>
static IconGlyph emptyIcon = IconGlyph::fromString("f059");
const IconGlyph& Theme::operator[](ThemeIcon icon)
{
	if (this->icons.contains(icon))
	{
		return this->icons[icon];
	}
	return emptyIcon;
}

//From PresentHook.cpp
//Need this to check if filters should be applied. We only want to apply them on the present thread.
//This is because the game render's on different threads, and for world rendering we need to use those
//But we only need filters for custom ui stuff, which will always be on the present thread.
//Otherwise we risk random std::bad_function_call exceptions or whatever. This was a pain in the ass to debug.
extern std::thread::id presentThreadId;

ColorF Theme::operator[](ThemeColor color)
{
	ColorF result;

	//Default to missing colors
	if (magic_enum::enum_name(color).find("TEXT") != std::string_view::npos)
		result = ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	result = ColorF(0.0f, 0.0f, 0.0f, 1.0f);

	//Resolve the actual color (if available)
	if (this->colors.contains(color))
	{
		auto& colorRef = this->colors[color];
		if (std::holds_alternative<ColorF>(colorRef))
		{
			result = std::get<ColorF>(colorRef);
		}
		if (std::holds_alternative<std::shared_ptr<ThemeVariable>>(colorRef))
		{
			result = std::get<std::shared_ptr<ThemeVariable>>(colorRef)->getColor();
		}
	}

	//Apply any filters
	auto currentId = std::this_thread::get_id();
	if (currentId == presentThreadId)
	{
		for (ThemeFilter filter : filterStack)
		{
			result = filter(color, result);
		}
	}

	//Done
	return result;
}
