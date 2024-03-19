#include "ItemUseSlowdownHook.h"

#include "../../Bridge/Components/ItemUseSlowdownModifierComponent.h"
#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/LocalPlayer.h"
#include "../../Bridge/StrictEntityContext.h"

#include "../../Events/EventDispatcher.h"
#include "../../Events/PlayerSlowedByItemEvent.h"

void __cdecl ItemUseSlowdownHook::itemUseSlowdownCallback_1_20_51_1(void* dont_care, StrictEntityContext* context, void* lol, void* lol2)
{
	auto* ci = Bridge::getClientInstance();
	auto* player = ci->getClientPlayer();
	if(context->entity == player->getEntityId())
	{
		auto event = nes::make_holder<PlayerSlowedByItemEvent>(player);

		auto dispatcher = systems->getEventDispatcher();
		dispatcher->trigger(event);

		if(event->isCancelled()) {
			return;
		}
	}
	PLH::FnCast(funcOriginal, &itemUseSlowdownCallback_1_20_51_1)(dont_care, context, lol, lol2);
}

ItemUseSlowdownHook::ItemUseSlowdownHook() : IPatch::IPatch("ItemUseSlowdownSystemImpl::doItemUseSlowdownSystem") {
	this->addSignature(SigTable::ItemUseSlowdownSystemImpl_doItemUseSlowdownSystem);
}

bool ItemUseSlowdownHook::apply() {
	return this->autoPatch(&itemUseSlowdownCallback_1_20_51_1, &funcOriginal);
}