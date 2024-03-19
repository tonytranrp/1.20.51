#include "Killaura.h"

#include "../../../../Utils/TargetUtils.h"

#include "../../../Bridge/Components/MobHurtTimeComponent.h"
#include "../../../Bridge/Components/RuntimeIDComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GameMode.h"
#include "../../../Bridge/InteractPacket.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/Minecraft.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../Rendering/GeometryBuilder.h"
#include "../../../Rendering/RenderController.h"

#include "../../Theme/ThemeManager.h"

void Killaura::onLevelTickEvent(LevelTickEvent& event)
{
	if (!this->isEnabled()) {
		return;
	}

	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) { return; }
	if(this->pauseInMenu->getValueAs<bool>() && !ci->isInGame()) { return; }
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) { return; }
	GameMode* gm = player->mGameMode;
	if (!gm) { return; }
	Minecraft* mc = ci->mMinecraft;
	if (!mc) return;

	bool canAttack = mc->tickTimer->seconds - this->lastAttackTime >= 1.0f / this->interval->getValueAs<float>();
	if (!canAttack) {
		return;
	}

	std::lock_guard<std::mutex> lock(targetLock);
	//Switch mode
	if (this->auraMode->getValueAs<int>() == 0)
	{
		// Find the nearest target
		lastTarget = theTarget;
		theTarget = (Mob*)TargetUtils::findTarget(
			player,
			this->targets->getSelectedTypes(),
			targetReach->getValueAs<float>(),
			false,
			checkTeam->getValueAs<bool>()
		);

		if (theTarget != lastTarget)
		{
			interpProgress = 0.0f;
			if (interacted) {
				InteractPacket interaction(InteractAction::MOUSEOVER, 0, Vector3<float>());
				ci->getPacketSender()->send(&interaction);
				interacted = false;
			}
		}

		if (!theTarget || !player->canAttack(theTarget, false)) {
			targetAABB.reset();
			interpProgress = 0.0f;
			if (interacted) {
				InteractPacket interaction(InteractAction::MOUSEOVER, 0, Vector3<float>());
				ci->getPacketSender()->send(&interaction);
				interacted = false;
			}
			return;
		}

		targetAABB = theTarget->aabbComponent->aabb;
		Vector3<float> currentLookPos = player->level->mHitResult.endPos;
		Vector3<float> optimalLookPos = targetAABB->closestPoint(player->getPosition());
		targetLookPos = currentLookPos.interpolate(optimalLookPos, interpProgress);

		//RenderPos is eye level for players, so use that for angle calculations
		Vector3<float> playerPos = player->getRenderPos();
		angles = playerPos.calcAngle(targetLookPos);

		bool withinReach = (player->getPosition().distanceTo<float>(targetLookPos) <= reach->getValueAs<float>());

		if (!interacted && withinReach && interpProgress >= 0.9f) {
			InteractPacket interaction(InteractAction::MOUSEOVER, theTarget->entityContext.tryGetComponent<RuntimeIDComponent>()->runtimeId, targetLookPos);
			ci->getPacketSender()->send(&interaction);
			interacted = true;
		}

		if (theTarget->isMob())
		{
			Mob* casted = reinterpret_cast<Mob*>(theTarget);
			if (this->perfectHits->getValueAs<bool>() && casted->mHurtTimeComponent->hurtTime - 1 > 0) {
				return;
			}
		}

		player->swing();
		if (withinReach && interacted) {
			gm->attack(theTarget);
		}

		if (interpProgress >= 1.0f) {
			interpProgress = 1.0f;
		}
		else {
			interpProgress += this->rotSpeed->getValueAs<float>();
		}
	}
	//Multi mode
	if (this->auraMode->getValueAs<int>() == 1)
	{
		theTargets = TargetUtils::findTargetMulti(
			player,
			this->targets->getSelectedTypes(),
			targetReach->getValueAs<float>(),
			false,
			checkTeam->getValueAs<bool>()
		);

		for (Actor* target : theTargets)
		{
			Mob* mobTarget = reinterpret_cast<Mob*>(target);
			if(!mobTarget->isAlive()) continue;
			if(!player->canAttack(mobTarget, false)) continue;
			targetAABB = mobTarget->aabbComponent->aabb;
			targetLookPos = targetAABB->closestPoint(player->getPosition());
			bool withinReach = (player->getPosition().distanceTo<float>(targetLookPos) <= reach->getValueAs<float>());

			if (this->perfectHits->getValueAs<bool>() && mobTarget->mHurtTimeComponent->hurtTime - 1 > 0) {
				continue;
			}

			player->swing();
			if (withinReach) {
				gm->attack(mobTarget);
			}
		}
	}
	this->lastAttackTime = mc->tickTimer->seconds;
}

void Killaura::onRenderEvent(RenderEvent& event)
{
	if (!this->debugRender->getValueAs<bool>()) return;
	std::lock_guard<std::mutex> lock(targetLock);
	if (event.getTime() != RenderTime::WORLD) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;

	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();
	GeometryBuilder builder(&renderer);

	Theme theme = systems->getThemeManager()->getActiveTheme();

	AABB<float> hitLocBox(
		player->level->mHitResult.endPos - 0.5f,
		player->level->mHitResult.endPos + 0.5f
	);
	builder.buildAABB(hitLocBox, theme[ThemeColor::KILLAURA_DEBUG_LOOK], false);

	AABB<float> targetLookBox(
		targetLookPos - 0.5f,
		targetLookPos + 0.5f
	);
	builder.buildAABB(targetLookBox, theme[ThemeColor::KILLAURA_DEBUG_REAL_LOOK], false);

	Vector3<float> optimalLookPos = targetAABB->closestPoint(player->getPosition());
	AABB<float> optimalBox(
		optimalLookPos - 0.5f,
		optimalLookPos + 0.5f
	);
	builder.buildAABB(optimalBox, theme[ThemeColor::KILLAURA_DEBUG_OPTIMAL], false);

	if (theTarget == nullptr) return;

	if (targetAABB.has_value()) {
		builder.buildAABB(*targetAABB, theme[ThemeColor::KILLAURA_TARGET], false, 12.0f);
	}
}

void Killaura::onPacketEvent(PacketEvent& event)
{
	//If we are in multi mode, we cant look at all actors at once so we do nothing for packets
	if (this->auraMode->getValueAs<int>() == 2) return;
	if (theTarget == nullptr) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;

	Packet* packet = event.getPacket();
	if (packet->getId() == PacketID::PlayerAuthInputPacket)
	{
		auto* paip = (PlayerAuthInputPacket*)packet;
		paip->rotation = angles;
		paip->headYaw = angles.y;
	}
	else if (packet->getId() == PacketID::MovePlayerPacket)
	{
		auto* mpp = (MovePlayerPacket*)packet;
		mpp->rotation = angles;
		mpp->headYaw = angles.y;
	}
}

Killaura::Killaura() : Module("Killaura", "Automatically attacks everything around you.") {
	this->pauseInMenu = this->addItem<Setting>("Pause in menu", "Prevent Killaura from attacking while GUIs are open", SettingType::TOGGLE, true, false, true);
	this->auraMode = this->addItem<ModeSetting<auraModes_t>>("Mode", "The mode for how killaura will attack entities.");
	//A setting to perfectly time hits
	this->perfectHits = this->addItem<Setting>("Perfect Hits", "Waits until the target can be attacked again before attacking.", SettingType::TOGGLE, false, false, true);
	//A setting for the max attacks per second
	this->interval = this->addItem<Setting>("Interval", "The amount of attacks per second (aka CPS/APS).", SettingType::SLIDER_FLOAT, 10.0f, 0.0f, 20.0f);
	//A setting for our attack reach
	this->reach = this->addItem<Setting>("Reach", "The range at which a target can be attacked.", SettingType::SLIDER_FLOAT, 3.0f, 0.0f, 10.0f);
	this->targetReach = this->addItem<Setting>("Target Reach", "The range at which a target can be determined.", SettingType::SLIDER_FLOAT, 3.0f, 0.0f, 11.0f);

	std::shared_ptr<Manager<Setting>> switchSettings = this->auraMode->getManagerFor(auraModes_t::Switch);
	if (switchSettings != nullptr)
	{
		//Speed for rotations
		this->rotSpeed = switchSettings->addItem<Setting>("Rotation Speed", "The speed of the rotations, use the 'ServerRotations' module to make them visible.", SettingType::SLIDER_FLOAT, 0.35f, 0.0f, 1.0f);
	}

	//Setting to target players
	this->targets = this->addItem<EntityTypeListSetting>("Targets", "Which entities should or shouldn't be targeted.");
	//If teammates should be ignored
	this->checkTeam = this->addItem<Setting>("Check Team", "Tries to prevent you from attacking your teammates.", SettingType::TOGGLE, false, false, true);
	this->debugRender = this->addItem<Setting>("Debug Render", "Shows your look target (white), the optimal look target (green), and where you are looking on the server side (von color).", SettingType::TOGGLE, false, false, true);
}
Killaura::~Killaura()
{
}

void Killaura::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &Killaura::onLevelTickEvent>(this);
	dispatcher->listen<PacketEvent, &Killaura::onPacketEvent>(this);
	dispatcher->listen<RenderEvent, &Killaura::onRenderEvent>(this);
};

void Killaura::onDisable() {
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;

	std::lock_guard<std::mutex> lock(targetLock);
	if (interacted)
	{
		InteractPacket interaction(InteractAction::MOUSEOVER, 0, Vector3<float>());
		ci->getPacketSender()->send(&interaction);

		interacted = false;
	}
	theTarget = nullptr;
	distance = 0;
	interpProgress = 0.0f;

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &Killaura::onLevelTickEvent>(this);
	dispatcher->deafen<PacketEvent, &Killaura::onPacketEvent>(this);
	dispatcher->deafen<RenderEvent, &Killaura::onRenderEvent>(this);
};