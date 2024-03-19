#include "BHop.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/MovePlayerPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"
#include "../../../Events/ReceivePacketEvent.h"

#include "../Render/Notifications.h"
#include "../ModuleManager.h"

#include <imgui_custom.h>

BHop::BHop() : Module("BHop", "Makes the player jump automatically when moving to gain a speed boost.")
{
	this->instantDirection = this->addItem<Setting>("Instant Direction", "Instantly changes your motion towards the desired direction.", SettingType::TOGGLE, true, false, true);
	this->cutMotion = this->addItem<Setting>("Cut Motion", "Instantly removes all movement velocity when no movement keys are pressed.", SettingType::TOGGLE, false, false, true);
	this->hop = this->addItem<Setting>("Hop", "Makes the player jump automatically.", SettingType::TOGGLE, false, false, true);
	//this->impulseCount = this->addItem<Setting>("Impulse Count", "The number of jumps that should be stacked in a single tick.", SettingType::SLIDER_INT, 1, 0, 3);
	//this->hopHeight = this->addItem<Setting>("Hop Height", "The upward velocity of the jump.", SettingType::SLIDER_FLOAT, 0.42f, 0.0f, 1.0f);
	this->checkLagback = this->addItem<Setting>("Check lagback", "Automatically disables bhop if a lagback is detected", SettingType::TOGGLE, true, false, true);
}

void BHop::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &BHop::onLevelTickEvent>(this);
	dispatcher->listen<PacketEvent, &BHop::onPacketEvent>(this);
	dispatcher->listen<ReceivePacketEvent, &BHop::onReceivePacketEvent>(this);
}

void BHop::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &BHop::onLevelTickEvent>(this);
	dispatcher->deafen<PacketEvent, &BHop::onPacketEvent>(this);
	dispatcher->deafen<ReceivePacketEvent, &BHop::onReceivePacketEvent>(this);
}

void BHop::onTick()
{
	//switch (this->preset->getValueAs<int>())
	//{
	//case 1: //Hive LowHop
	//	this->instantDirection->setValue(true);
	//	this->cutMotion->setValue(false);
	//	this->hop->setValue(true);
	//	this->impulseCount->setValue(2);
	//	this->hopHeight->setValue(0.5f);
	//	this->damageBoost->setValue(true);
	//	this->boostAmount->setValue(0.5f);
	//	this->fakeLag->setValue(true);
	//	this->lagTicks->setValue(12);
	//	this->lagRandom->setValue(5);
	//	this->timer->setValue(true);
	//	this->timerSpeed->setValue(1.1f);
	//	break;
	//case 0:
	//default:
	//	break;
	//}
}

bool BHop::isKeyDown(std::string key) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return false;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (!cih) return false;
	ClientInputMappingFactory* cimf = cih->factory;
	if (!cimf) return false;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (!kms) return false;

	return isKeyDown(kms->GetMCKey(std::move(key)));
}

bool BHop::isKeyDown(int key) {
	return ImGui::IsKeyDown(ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key));
}

//Modifies the playerYaw value depending on what keys are down
Vector1<float> BHop::modYawByInputs(float playerYaw)
{
	bool w = isKeyDown("key.forward");
	bool a = isKeyDown("key.left");
	bool s = isKeyDown("key.back");
	bool d = isKeyDown("key.right");

	if (w)
	{
		if (!a && !d)
			playerYaw += 0.0f;
		if (a)
			playerYaw += -45.0f;
		else if (d)
			playerYaw += 45.0f;
	}
	else if (s)
	{
		if (!a && !d)
			playerYaw -= 180.0f;
		if (a)
			playerYaw -= 135.0f;
		else if (d)
			playerYaw -= -135.0f;
	}
	else
	{
		if (!a && d)
			playerYaw += 90.0f;
		if (!d && a)
			playerYaw -= 90.0f;
	}

	return Vector1<float>(playerYaw);

}

bool BHop::isUsingMoveKeys() {
	bool w = isKeyDown("key.forward");
	bool a = isKeyDown("key.left");
	bool s = isKeyDown("key.back");
	bool d = isKeyDown("key.right");
	bool isUsingMoveKeys = w || a || s || d;
	return isUsingMoveKeys;
}

void BHop::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	GuiData* gd = ci->getGuiData();
	if (gd == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	bool movementKeyDown = this->isUsingMoveKeys();

	Vector3<float> velocity = player->stateVectorComponent->velocity;
	Vector2<float> playerRots = player->rotationComponent->Get();
	if (movementKeyDown)
	{
		motionCutted = false;
	}

	if (movementKeyDown && this->instantDirection->getValueAs<bool>())
	{
		Vector1<float> newYaw = modYawByInputs(playerRots.y);
		Vector2<float> newAngles(playerRots.x, newYaw.x);
		Vector3<float> rotatedVelocity = velocity.rotateXYZ(newAngles);
		rotatedVelocity.y = velocity.y;
		player->stateVectorComponent->velocity = rotatedVelocity;
	}

	if (!movementKeyDown && this->cutMotion->getValueAs<bool>() && !motionCutted)
	{
		player->stateVectorComponent->velocity *= 0.0f;
		motionCutted = true;
	}

	if (this->hop->getValueAs<bool>())
	{
		if (player->isOnGround() && movementKeyDown && !player->isImmobile())
		{
			player->jumpFromGround();
		}
	}
}

void BHop::onPacketEvent(PacketEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
	{
		auto* paip = (PlayerAuthInputPacket*)event.getPacket();
		/*paip->inputData |= AuthInputAction::NORTH_JUMP;
		paip->inputData |= AuthInputAction::JUMP_DOWN;
		paip->inputData |= AuthInputAction::WANT_UP;*/

		if (isUsingMoveKeys()) {
			paip->inputData |= AuthInputAction::SPRINTING;
			paip->inputData |= AuthInputAction::SPRINT_DOWN;
			paip->inputData |= AuthInputAction::START_SPRINTING;
			// remove the StopSpinting flag if it's there
			paip->inputData &= ~AuthInputAction::STOP_SPRINTING;
		}
		else {
			paip->inputData |= AuthInputAction::STOP_SPRINTING;
		}

		if (player->isOnGround()) {
			paip->inputData |= AuthInputAction::NORTH_JUMP;
			paip->inputData |= AuthInputAction::JUMP_DOWN;
			paip->inputData |= AuthInputAction::WANT_UP;
		}
	}
}

void BHop::onReceivePacketEvent(PacketEvent& event)
{
	if (!this->checkLagback->getValueAs<bool>()) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	Packet* packet = event.getPacket();
	if (packet->getId() == PacketID::MovePlayerPacket)
	{
		auto* mpp = reinterpret_cast<MovePlayerPacket*>(packet);
		if (player->getPosition().distanceTo(mpp->position) < 10 && player->getPosition().distanceTo(mpp->position) > 0.01f)
		{
			auto notifs = systems->getModuleManager()->findModule<Notifications>();
			notifs->notify("A lagback was detected, so bhop was disabled.", "BHop Disabled", 10.0f);
			this->setEnabled(false);
		}
	}
}