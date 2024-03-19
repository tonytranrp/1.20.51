#include "NoFilter.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Options.h"
#include "../../../Bridge/FloatOption.h"

#include <vector>
#include <string>

NoFilter::NoFilter() : Module("NoFilter", "Disables the Minecraft chat filter.")
{
	this->setEnabled(true);
}

void NoFilter::onEnable()
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	auto filter = ci->getProfanityContext();
	if (!filter) return;

	this->wasEnabled = filter->enabled;
	filter->enabled = false;
}

void NoFilter::onDisable()
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	auto filter = ci->getProfanityContext();
	if (!filter) return;

	filter->enabled = this->wasEnabled;
}