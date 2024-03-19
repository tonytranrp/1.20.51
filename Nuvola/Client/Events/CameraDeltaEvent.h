#pragma once

#include "Event.h"
#include "Cancellable.h"

#include <Math/Math.h>

class CameraDeltaEvent : public Event, public Cancellable
{
	Vector2<float> anglesRad;
	float delta;
	Vector2<float> yawRange;
public:
	CameraDeltaEvent(Vector2<float> anglesRad, float delta, Vector2<float> yawRange)
	{
		this->anglesRad = anglesRad;
		this->delta = delta;
		this->yawRange = yawRange;
	}

	Vector2<float> getAnglesRad()
	{
		return this->anglesRad;
	}
	Vector2<float> getAnglesDeg()
	{
		return this->anglesRad.toDegrees();
	}

	void setAnglesRad(Vector2<float> anglesRad)
	{
		this->anglesRad = anglesRad;
	}
	void setAnglesDeg(Vector2<float> anglesDeg)
	{
		this->anglesRad = anglesDeg.toRadians();
	}

	float getDelta()
	{
		return this->delta;
	}

	Vector2<float> getYawRangeRad()
	{
		return this->yawRange;
	}
	Vector2<float> getYawRangeDeg()
	{
		return this->yawRange.toDegrees();
	}

	void setYawRangeRad(Vector2<float> pitchRange)
	{
		this->yawRange = pitchRange;
	}
	void setYawRangeDeg(Vector2<float> pitchRangeDeg)
	{
		this->yawRange = pitchRangeDeg.toRadians();
	}
};