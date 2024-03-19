#ifndef GUARD_FontEntry
#define GUARD_FontEntry
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
struct FontEntry {
	/* Fields */
	char padding_40[40];
	class BitmapFont* font;
	char padding_64[16];
	void* wackyptr;
	/* Virtuals */
	/* Functions */
};
#endif