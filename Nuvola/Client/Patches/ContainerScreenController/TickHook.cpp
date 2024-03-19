#include  "TickHook.h"

#include "../../Bridge/ContainerScreenController.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/ScreenControllerEvent.h"

using namespace _ContainerScreenController;
uint32_t __fastcall TickHook::TickCallback_1_20_0(ContainerScreenController* controller) {
	auto event = nes::make_holder<ScreenControllerEvent>(controller);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
	return PLH::FnCast(funcOriginal, &TickCallback_1_20_0)(controller);
}

TickHook::TickHook() : IPatch::IPatch("ContainerScreenController::tick") {
	this->addSignature(SigTable::ContainerScreenController_tick);
}

bool TickHook::apply() {
	return this->autoPatch(&TickCallback_1_20_0, &funcOriginal);
}