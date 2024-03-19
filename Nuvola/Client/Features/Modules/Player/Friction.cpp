#include "Friction.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Components/ActorMovementProxyComponent.h"
#include "../../../Bridge/IActorMovementProxy.h"
#include "../../../Bridge/LocalPlayer.h"

#include <xorstr.hpp>
Friction::Friction() : Module(std::string("Friction"), "Prevents blocks from affecting your movement. Commonly referred to as 'anti-web', but this applies to more than just webs.")
{
	this->x_friction = this->addItem<Setting>("X%", "The percentage of X friction that should be applied.", SettingType::SLIDER_FLOAT, 0.0f, 0.0f, 100.0f);
	this->y_friction = this->addItem<Setting>("Y%", "The percentage of Y friction that should be applied.", SettingType::SLIDER_FLOAT, 0.0f, 0.0f, 100.0f);
	this->z_friction = this->addItem<Setting>("Z%", "The percentage of Z friction that should be applied.", SettingType::SLIDER_FLOAT, 0.0f, 0.0f, 100.0f);
}

void Friction::onEnable()
{
	
}

void Friction::onTick()
{
	
}

void Friction::onDisable()
{
	ClientInstance* client = Bridge::getClientInstance();
	if (client) {
		LocalPlayer* player = client->getClientPlayer();
		if (player) {

		}
	}
}

void Friction::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* client = Bridge::getClientInstance();
	if (client) {
		LocalPlayer* player = client->getClientPlayer();
		if (player) {

		}
	}
}
