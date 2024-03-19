#include "CustomSky.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Minecraft.h"
#include "../../../Bridge/MCTimer.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/FogGetColorEvent.h"
#include "../../../Events/GetCloudHeightEvent.h"	

CustomSky::CustomSky() : Module("Custom Sky", "Change the overworld sky color.") {
	this->r = this->addItem<Setting>("R", "How much red (RGBA color)", SettingType::SLIDER_FLOAT, 1.000f, 0.000f, 1.000f);
	this->g = this->addItem<Setting>("G", "How much green (RGBA color)", SettingType::SLIDER_FLOAT, 1.000f, 0.000f, 1.000f);
	this->b = this->addItem<Setting>("B", "How much  blue (RGBA color)", SettingType::SLIDER_FLOAT, 1.000f, 0.000f, 1.000f);
	this->rainbow = this->addItem<Setting>("Rainbow sky", "Change between colors.", SettingType::TOGGLE, false, false, true);
	this->intensity = this->addItem<Setting>("Speed", "The speed of the rainbow animation", SettingType::SLIDER_FLOAT, 0.1f, 0.0f, 1.0f);

	this->CloudHeight = this->addItem<Setting>("Cloud Height", "How high the clouds render.", SettingType::SLIDER_INT, 128, -64, 320);
}


void CustomSky::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<FogGetColorEvent, &CustomSky::onFogGetColorEvent>(this);
	dispatcher->listen<GetCloudHeightEvent, &CustomSky::onGetCloudHeightEvent>(this);
}
void CustomSky::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<FogGetColorEvent, &CustomSky::onFogGetColorEvent>(this);
	dispatcher->deafen<GetCloudHeightEvent, &CustomSky::onGetCloudHeightEvent>(this);
}

void CustomSky::onFogGetColorEvent(FogGetColorEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	Minecraft* mc = ci->mMinecraft;
	if (!mc) return;
	MCTimer* renderTimer = mc->renderTimer;

	ColorF finalColor;
	if (this->rainbow->getValueAs<bool>()) {
		float hue = fmodf(renderTimer->seconds * this->intensity->getValueAs<float>(), 1.0f);
		finalColor = ColorF::fromHSV(hue, 1.0f, 1.0f);
	}
    else {
		finalColor.x = this->r->getValueAs<float>();
		finalColor.y = this->g->getValueAs<float>();
		finalColor.z = this->b->getValueAs<float>();
		finalColor.w = 1.0f;
    }
	event.setColor(finalColor);
}

void CustomSky::onGetCloudHeightEvent(GetCloudHeightEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (ci->getClientPlayer() == nullptr) return;

	event.setHeight(this->CloudHeight->getValueAs<int>());
}