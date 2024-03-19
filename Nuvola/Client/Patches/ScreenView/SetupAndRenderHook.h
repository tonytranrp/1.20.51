#ifndef NUVOLA_CLIENT_PATCHES_SCREENVIEW_SETUPANDRENDERHOOK
#define NUVOLA_CLIENT_PATCHES_SCREENVIEW_SETUPANDRENDERHOOK

#include "../IPatch.h"

class SetupAndRenderHook : public IPatch {
    static inline uint64_t setupAndRenderOriginal = 0;
	static void __fastcall setupAndRenderCallback_1_17_10_4(class ScreenView* screenView, class MinecraftUIRenderContext* renderContext);
public:
	SetupAndRenderHook();
	bool apply() override;
};

#endif /* NUVOLA_CLIENT_PATCHES_SCREENVIEW_SETUPANDRENDERHOOK */
