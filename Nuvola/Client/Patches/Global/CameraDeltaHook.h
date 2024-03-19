#pragma once

#include "../../Events/CameraDeltaEvent.h"
#include "../IPatch.h"

class CameraDeltaHook : public IPatch {
	static void cameraDeltaCallback_1_20_12_1(void* cameraStackRef, void* quaternionCamera, struct RotationInput* input);
public:
	static inline uintptr_t funcOriginal = 0;
	CameraDeltaHook();
	bool apply() override;
};
