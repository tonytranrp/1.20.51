#pragma once

#include "Attribute.h"

#include <unordered_map>

class BaseAttributeMap {
	static AttributeInstance mInvalidInstance;
public:
	std::unordered_map<uint32_t, AttributeInstance> mInstanceMap; // 0x0
	std::vector<AttributeInstanceHandle> mDirtyAttributes; // 0x40

	inline AttributeInstance &getInstance(uint32_t id) {
		auto it = this->mInstanceMap.find(id);
		if (it != this->mInstanceMap.end()) {
			return it->second;
		}
		return mInvalidInstance;
	}
	inline AttributeInstance& getInstance(AttributeID id) {
		return this->getInstance(static_cast<uint32_t>(id));
	}
};
static_assert(sizeof(BaseAttributeMap) == 0x58);