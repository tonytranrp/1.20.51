#include "GetBrightnessDependentFogColorHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/FogGetColorEvent.h"

ColorF* __fastcall GetBrightnessDependentFogColorHook::getBrightnessDependentFogColorCallback_1_20_12_1(const Dimension* self, ColorF* mceColor /* mce::Color idk if in sdk*/, const __int64 arg3, const float arg4 /*idk im not very good at decompile*/)
{
	auto* resultColor = PLH::FnCast(funcOriginal, &getBrightnessDependentFogColorCallback_1_20_12_1)(self, mceColor, arg3, arg4);
	auto event = nes::make_holder<FogGetColorEvent>(*resultColor);

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);

	*resultColor = event->getColor();
	return resultColor;
}

GetBrightnessDependentFogColorHook::GetBrightnessDependentFogColorHook() : IPatch::IPatch("Dimension::getBrightnessDependentFogColor")
{
	this->addSignature(SigTable::Dimension_getBrightnessDependentFogColor); //1.20.12.1
}

bool GetBrightnessDependentFogColorHook::apply()
{
	return this->autoPatch(&getBrightnessDependentFogColorCallback_1_20_12_1, &funcOriginal);
}