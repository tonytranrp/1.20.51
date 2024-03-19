#include "CameraDeltaHook.h"

#include "../../Bridge/RotationInput.h"
#include "../../Events/EventDispatcher.h"
#include "../../SystemManager.h"

#include <imgui.h>
#include <imgui_custom.h>

#include <xorstr.hpp>

void CameraDeltaHook::cameraDeltaCallback_1_20_12_1(void* cameraStackRef, void* quaternionCamera, RotationInput* input) 
{
    auto event = nes::make_holder<CameraDeltaEvent>(input->angles, input->delta, input->yawRange);

    auto dispatcher = systems->getEventDispatcher();
    dispatcher->trigger(event);

    input->angles = event->getAnglesRad();
    input->yawRange = event->getYawRangeRad();
    PLH::FnCast(funcOriginal, cameraDeltaCallback_1_20_12_1)(cameraStackRef, quaternionCamera, input);
}

CameraDeltaHook::CameraDeltaHook() : IPatch("Global::CameraDelta") {
    this->addSignature(SigTable::Global_cameraDelta);
}

bool CameraDeltaHook::apply() {
    return this->autoPatch(cameraDeltaCallback_1_20_12_1, &funcOriginal);
}