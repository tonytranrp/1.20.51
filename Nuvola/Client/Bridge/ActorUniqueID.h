#pragma once

class ActorUniqueID {
	size_t leId;
public:
	ActorUniqueID(size_t id) {
		this->leId = id;
	}
	size_t GetID() const {
		return this->leId;
	}
	size_t operator=(const ActorUniqueID& rhs) const {
		return rhs.GetID();
	}
};