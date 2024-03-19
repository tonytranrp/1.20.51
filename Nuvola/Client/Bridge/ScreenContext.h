#ifndef GUARD_ScreenContext
#define GUARD_ScreenContext
#ifndef SCANSIG
#define SCANSIG Mem::FindSig
#endif
#ifndef WRITEOUT
#define WRITEOUT printf
#endif
#ifndef PREINIT
#define PREINIT Utils::PreInit
#endif
#include <Math/Math.h>
#include "../../Utils/Utils.h"
#include "../../Mem/Mem.h"
#include "string"
#include "functional"
struct ScreenContext {
	/* Fields */
	char padding_48[48];
	class Color* shaderColor;
	char padding_176[120];
	class Tessellator* tessellator;
	/* Virtuals */
	/* Functions */
};
#endif