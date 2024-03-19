#include "Spider.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

Spider::Spider() : Module("Spider", "Crawl up any wall like a spider.")
{
	this->speed = this->addItem<Setting>("Speed", "The speed at which you will climb walls.", SettingType::SLIDER_FLOAT, 0.2f, 0.0f, 1.0f);
}

void Spider::onEnable()
{
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Spider::onLevelTickEvent>(this);
}

void Spider::onDisable()
{
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Spider::onLevelTickEvent>(this);
}

void Spider::onLevelTickEvent(LevelTickEvent& event)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;

    if (player->isCollidingHorizontal())
    {
        player->stateVectorComponent->velocity.y = this->speed->getValueAs<float>();
    }
}
