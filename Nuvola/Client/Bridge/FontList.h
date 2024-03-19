#ifndef GUARD_FontList
#define GUARD_FontList
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
#include "FontEntry.h"
struct FontList {
	/* Fields */
	FontEntry fontEntries[9];
	/* Virtuals */
	/* Functions */
};
#endif