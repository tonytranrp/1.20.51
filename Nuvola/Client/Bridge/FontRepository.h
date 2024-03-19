#ifndef GUARD_FontRepository
#define GUARD_FontRepository
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
struct FontRepository {
	/* Fields */
	char padding_16[16];
	class FontList* fontList;
	/* Virtuals */
	/* Functions */
};
#endif