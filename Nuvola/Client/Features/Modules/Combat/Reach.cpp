#include "Reach.h"

#include "../../../Signatures/SigManager.h"
#include "../../../SystemManager.h"

Reach::Reach() : Module("Reach", "Allows you to hit opponents further than normal")
{
	this->range = this->addItem<Setting>("Range", "Your reach value.", SettingType::SLIDER_FLOAT, 3.0f, 3.0f, 8.0f);
}

void Reach::onEnable() {}
void Reach::onDisable()
{
	auto* creativeReach = systems->getSigManager()->findAddressAs<float *>(SigTable::Global_creativeReachPtr);
	auto* reach = systems->getSigManager()->findAddressAs<float *>(SigTable::Global_reachPtr);

	*creativeReach = 7.0f;
	*reach = 3.0f;
}

void Reach::onTick()
{
	auto* creativeReach = systems->getSigManager()->findAddressAs<float *>(SigTable::Global_creativeReachPtr);
	auto* reach = systems->getSigManager()->findAddressAs<float *>(SigTable::Global_reachPtr);

	*creativeReach = this->range->getValueAs<float>();
	*reach = this->range->getValueAs<float>();
}