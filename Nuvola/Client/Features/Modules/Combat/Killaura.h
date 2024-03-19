#pragma once

#include "../../../Bridge/NamespacedKey.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../Module.h"
#include "../ModeSetting.h"
#include "../EntityTypeListSetting.h"

#include <optional>
#include <mutex>

enum class auraModes_t
{
	Switch,
	Multi
};
class LevelTickEvent;
class RenderEvent;
class PacketEvent;
class Killaura : public Module {
	std::shared_ptr<Setting> pauseInMenu = nullptr;
	std::shared_ptr<ModeSetting<auraModes_t>> auraMode = nullptr;
	std::shared_ptr<Setting> perfectHits = nullptr;
	std::shared_ptr<Setting> interval = nullptr;
	std::shared_ptr<Setting> reach = nullptr;
	std::shared_ptr<Setting> targetReach = nullptr;
	std::shared_ptr<Setting> rotSpeed = nullptr;
	std::shared_ptr<EntityTypeListSetting> targets = nullptr;
	std::shared_ptr<Setting> checkTeam = nullptr;
	std::shared_ptr<Setting> debugRender = nullptr;

	float lastAttackTime = 0.0f;
	float distance = 0;
	bool interacted = false;

	Actor* lastTarget = nullptr;
	Actor* theTarget = nullptr;
	std::vector<Actor*> theTargets;
	std::mutex targetLock;
	std::optional<AABB<>> targetAABB;
	Vector3<float> targetLookPos;
	Vector2<float> angles;
	float interpProgress = 0.0f;
public:
	Killaura();
	~Killaura();
	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
	void onRenderEvent(RenderEvent& event);
	void onPacketEvent(PacketEvent& event);
};