#include "Aimbot.h"

#include "../../../../Utils/TargetUtils.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/CameraDeltaEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RenderEvent.h"

#include <imgui.h>

Aimbot::Aimbot() : Module("Aimbot", "Automatically aims for you.")
{
	this->speed = this->addItem<Setting>("Rotation Speed", "How much speed the aimbot will have.", SettingType::SLIDER_FLOAT, 0.2f, 0.0f, 1.0f);
	this->range = this->addItem<Setting>("Range", "How far away the target should be before aimbot kicks in.", SettingType::SLIDER_FLOAT, 3.0f, 0.0f, 10.0f);
	this->MouseHeld = this->addItem<Setting>("Mouse Held", "If it should only aim if mousebutton 1 is held.", SettingType::TOGGLE, true, false, true);
	this->targets = this->addItem<EntityTypeListSetting>("Targets", "Which entities should or shouldn't be targeted.");
	this->checkTeam = this->addItem<Setting>("Check Team", "Tries to prevent you from attacking your teammates.", SettingType::TOGGLE, false, false, true);
}

void Aimbot::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<CameraDeltaEvent, &Aimbot::onCameraDeltaEvent>(this);
	dispatcher->listen<RenderEvent, &Aimbot::onRenderEvent>(this);
}

void Aimbot::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<CameraDeltaEvent, &Aimbot::onCameraDeltaEvent>(this);
	dispatcher->deafen<RenderEvent, &Aimbot::onRenderEvent>(this);
}

void Aimbot::onCameraDeltaEvent(CameraDeltaEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	LevelRenderer* lr = ci->mLevelRenderer;
	if (lr == nullptr) return;
	LevelRendererPlayer* lrp = lr->levelRendererPlayer;
	if (lrp == nullptr) return;
	if (MouseHeld->getValueAs<bool>() && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
		return;
	Vector3<float> camPos = lrp->mOrigin;
	Vector3<float> hitTarget = lp->level->mHitResult.endPos;
	if (this->targetBox.has_value())
	{
		AABB<float> aabb = *this->targetBox;
		Vector3<float> aimPoint = hitTarget.interpolate(aabb.closestPoint(camPos), this->speed->getValueAs<float>());
		Vector2<float> interpolatedAnglesDeg = camPos.calcAngleDeg(aimPoint);
		//The camera angles are pitch, yaw
		float pitch = interpolatedAnglesDeg.y;
		interpolatedAnglesDeg.y = interpolatedAnglesDeg.x;
		interpolatedAnglesDeg.x = pitch;

		//Invert so that if the optimal pitch is 180, we get 0 radians
		interpolatedAnglesDeg.x += 180; //Put the pitch in range of 0 to 360
		interpolatedAnglesDeg.x = 360 - interpolatedAnglesDeg.x; //Ivert to range of 360 to 0
		//optimalAnglesDeg.x -= 180; //Make it a range of 180 to -180 again
		interpolatedAnglesDeg.y += 90; //Put the yaw withing range of 0 and 180
		float yPerc = interpolatedAnglesDeg.y / 180;
		Vector1<float> high(-PI / 2.0f);
		Vector1<float> low(PI / 2.0f);
		//interpolatedAnglesDeg.y = 180 - interpolatedAnglesDeg.y; //Invert the value and make it a range of 180 to 0
		//interpolatedAnglesDeg.y -= 90; //Make it a range of 90 to -90 again

		Vector2<float> interpolatedAngles = interpolatedAnglesDeg.toRadians();
		interpolatedAngles.y = low.interpolate(high, yPerc).x;
		Vector2<float> camAnglesRad = event.getAnglesRad();

		event.setAnglesRad(interpolatedAngles);
	}
}

void Aimbot::onRenderEvent(RenderEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	Mob* theTarget = (Mob*)TargetUtils::findTarget(
		lp,
		this->targets->getSelectedTypes(),
		range->getValueAs<float>(),
		false,
		checkTeam->getValueAs<bool>()
	);

	if (theTarget == nullptr)
	{
		this->targetBox.reset();
		return;
	}

	this->targetBox = theTarget->getRenderAABB();
}

