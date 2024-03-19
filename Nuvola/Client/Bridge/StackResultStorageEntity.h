#pragma once

#include "../Signatures/SigManager.h"

struct StackResultStorageEntity {
	/* Fields */
	char pad_0x0[0x18];
	/* Virtuals */
	/* Functions */
	static inline uintptr_t holder_StackResultStorageEntity = 0;
	StackResultStorageEntity(class EntityRefTraits* entityContext) {
		if(holder_StackResultStorageEntity == 0)
			holder_StackResultStorageEntity = systems->getSigManager()->findAddressAs<uintptr_t>(
                    SigTable::StackResultStorageEntity_cctor);
		((void(__thiscall*)(StackResultStorageEntity*, class EntityRefTraits* weakEntity))holder_StackResultStorageEntity)(this, entityContext);
	};
};