#pragma once

#include "../Module.h"

#include <Math/Math.h>

class ActorRotateEvent;
class PacketEvent;
class ServerRotations : public Module
{
	Vector2<float> lastRotation;
	Vector2<float> oldLastRotation;
public:
	ServerRotations();
	void onEnable() override;
	void onDisable() override;

	void setRotation(const Vector2<float>& rotation);

	void onActorRotateEvent(ActorRotateEvent& event);
	void onPacketEvent(PacketEvent& event);
};