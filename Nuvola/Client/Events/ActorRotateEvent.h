#pragma once

#include "ActorEvent.h"
#include "Cancellable.h"
#include "../Bridge/Actor.h"

enum class RotationType {
	REAL,
	RENDER
};

class ActorRotateEvent : public ActorEvent, public Cancellable {
	Vector2<float> rotation;
	Vector2<float> oldRotation;
	RotationType rotationType;
public:
	ActorRotateEvent(Actor* actor, const Vector2<float>& rotation, const Vector2<float>& oldRotation, RotationType rotationType) : ActorEvent(actor) {
		this->rotation = rotation;
		this->oldRotation = oldRotation;
		this->rotationType = rotationType;
	};

	RotationType getRotationType() {
		return this->rotationType;
	}

	const Vector2<float>& getRotation() {
		return this->rotation;
	}
	void setRotation(const Vector2<float>& rot) {
		this->rotation = rot;
	}

	const Vector2<float>& getOldRotation() {
		return this->oldRotation;
	}
	void setOldRotation(const Vector2<float>& rot) {
		this->oldRotation = rot;
	}

	[[nodiscard]] float getYaw() const {
		return this->rotation.y;
	}
	void setYaw(float yaw) {
		this->rotation.y = yaw;
	}
	[[nodiscard]] float getOldYaw() const {
		return this->oldRotation.y;
	}
	void setOldYaw(float yaw) {
		this->oldRotation.y = yaw;
	}

	[[nodiscard]] float getPitch() const {
		return this->rotation.x;
	}
	void setPitch(float pitch) {
		this->rotation.x = pitch;
	}
	[[nodiscard]] float getOldPitch() const {
		return this->oldRotation.x;
	}
	void setOldPitch(float pitch) {
		this->oldRotation.x = pitch;
	}
};
