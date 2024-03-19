#pragma once

#include <Math/Math.h>

struct ActorRotationComponent
{
public:
	Vector2<float> rotation;
	Vector2<float> oldRotation;

	ActorRotationComponent(const Vector2<float>& rotation) : rotation(rotation), oldRotation(rotation) {}

	[[nodiscard]] const Vector2<float>& Get() const
	{
		return this->rotation;
	}

	void Set(Vector2<float> rots)
	{
		this->rotation = rots;
	}
};