#include "Fullbright.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Options.h"
#include "../../../Bridge/FloatOption.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

#include <vector>
#include <string>

static std::vector<std::string> modes = { "Regular", "Infinite", "Random" };

Fullbright::Fullbright() : Module("Fullbright", "Makes everything bright")
{
	this->modeSetting = this->addItem<Setting>("Mode", "The way the fullbright should function.", SettingType::ENUM, 0, 0, modes.size() - 1, modes);
}

static float normalBrightness = 0.0f;
void Fullbright::onEnable()
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	Options* options = ci->getOptions();
	if (!options) return;
	auto* gamma = options->getOption<FloatOption, Options::OptionID::gfx_gamma>();
	if (!gamma) return;
	normalBrightness = gamma->value;

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &Fullbright::onLevelTickEvent>(this);
}

void Fullbright::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &Fullbright::onLevelTickEvent>(this);
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	Options* options = ci->getOptions();
	if (!options) return;
	auto* gamma = options->getOption<FloatOption, Options::OptionID::gfx_gamma>();
	if (!gamma) return;
	gamma->value = normalBrightness;
}

void Fullbright::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	Options* options = ci->getOptions();
	if (!options) return;
	auto* gamma = options->getOption<FloatOption, Options::OptionID::gfx_gamma>();
	if (!gamma) return;

	switch (this->modeSetting->getValueAs<int>())
	{
	default:
	case 0:
		gamma->value = 100.0f;
		break;
	case 1:
		gamma->value = 100000000000.0f;
		break;
	case 2:
		gamma->value = ((double)rand() / (RAND_MAX)) + 1;
		break;
	}
}