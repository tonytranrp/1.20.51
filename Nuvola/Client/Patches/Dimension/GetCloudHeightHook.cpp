#include "GetCloudHeightHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/GetCloudHeightEvent.h"

int __fastcall GetCloudHeightHook::GetCloudHeightCallback_1_20_40(const Dimension* self)
{
	auto height = PLH::FnCast(funcOriginal, &GetCloudHeightCallback_1_20_40)(self);
	auto event = nes::make_holder<GetCloudHeightEvent>(height);

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	height = event->getHeight();
	return height;
}

GetCloudHeightHook::GetCloudHeightHook() : IPatch::IPatch("Dimension::GetCloudHeight")
{
	this->addSignature(SigTable::Dimension_getCloudHeight); //1.20.12.1
}

bool GetCloudHeightHook::apply()
{
	return this->autoPatch(&GetCloudHeightCallback_1_20_40, &funcOriginal);
}