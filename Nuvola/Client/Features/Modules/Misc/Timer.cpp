#include "Timer.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Minecraft.h"
#include "../../../Bridge/MCTimer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

Timer::Timer() : Module("Timer", "Speed up or slow down the game's (client side) tickrate.")
{
	this->tps = this->addItem<Setting>("TPS", "The new amount of ticks per second.", SettingType::SLIDER_INT, 20, 0, 100);
}
void Timer::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &Timer::onLevelTickEvent>(this);
}
void Timer::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &Timer::onLevelTickEvent>(this);

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	Minecraft* mc = ci->mMinecraft;
	if (!mc) return;
	MCTimer* tickTimer = mc->tickTimer;
	if (!tickTimer) return;
	MCTimer* renderTimer = mc->renderTimer;

	tickTimer->ticksPerSecond = 20;
	renderTimer->ticksPerSecond = 20;
}
void Timer::onTick() {

}

void Timer::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	Minecraft* mc = ci->mMinecraft;
	if (!mc) return;
	MCTimer* tickTimer = mc->tickTimer;
	if (!tickTimer) return;
	MCTimer* renderTimer = mc->renderTimer;

	tickTimer->ticksPerSecond = this->tps->getValueAs<int>();
	renderTimer->ticksPerSecond = this->tps->getValueAs<int>();
}
