#pragma once

#include "EntityContext.h"
#include "OwnerPtrT.h"
#include <optional>

#pragma pack(push, 1)
struct OwnerStorageEntity : public OwnerPtrT<EntityContext> {
	/* Virtuals */
	/* Functions */
};
#pragma pack(pop)
static_assert(sizeof(OwnerStorageEntity) == 0x18);