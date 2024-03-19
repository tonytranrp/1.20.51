#include "TickHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/LocalPlayer.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/LevelTickEvent.h"

#include <xorstr.hpp>

using namespace _Level;
void __fastcall TickHook::TickHookCallback_1_19_22(Level* level) {
	auto ci = Bridge::getClientInstance();
	if(ci != nullptr)
	{
		auto lp = ci->getClientPlayer();
		if(lp != nullptr)
		{
			auto localLevel = lp->level;
			if(localLevel != nullptr && localLevel == level)
			{
				auto event = nes::make_holder<LevelTickEvent>(level);
				auto dispatcher = systems->getEventDispatcher();
				dispatcher->trigger(event);
				if (event->isCancelled()) {
					return;
				}
			}
		};
	};
	PLH::FnCast(funcOriginal, &TickHookCallback_1_19_22)(level);
}

TickHook::TickHook() : IPatch::IPatch(xorstr_("Level::tick")) {
	this->addSignature(SigTable::Level_tick);
}

bool TickHook::apply() {
	return this->autoPatch(&TickHookCallback_1_19_22, &funcOriginal);
}