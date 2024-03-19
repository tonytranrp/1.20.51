#pragma once

class ActorRuntimeID {
	int leId;
public:
	ActorRuntimeID(int id) {
		this->leId = id;
	}
	int GetID() const {
		return this->leId;
	}
	int operator=(const ActorRuntimeID& rhs) const {
		return rhs.GetID();
	}
};