#include "Clicktp.h"

#include "../../../Bridge/BlockSource.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/MouseEvent.h"
#include "../../../Events/RenderEvent.h"
#include "../../../GUI/ScreenManager.h"
#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/WorldRenderer.h"
#include "../../../Rendering/GeometryBuilder.h"
#include "../Render/Notifications.h"
#include "../ModuleManager.h"

ClickTp::ClickTp() : Module("ClickTp", "Right click a block and tp to it.") {
	this->range = this->addItem<Setting>("Range", "Your reach value.", SettingType::SLIDER_FLOAT, 10.0f, 10.0f, 30.0f);
}

void ClickTp::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<MouseEvent, &ClickTp::onMouseEvent>(this);
}
void ClickTp::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<MouseEvent, &ClickTp::onMouseEvent>(this);
	this->needsTp = false;
}

void ClickTp::onMouseEvent(MouseEvent& event) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	auto screenMgr = systems->getScreenManager();
	if(screenMgr->getActiveScreen() != nullptr) { return; };

	if (event.getButton() == MouseButton::RIGHT && event.getAction() == MouseAction::PRESS) {
		this->needsTp = true;
	}
}

void ClickTp::onRenderEvent(RenderEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	if (event.getTime() != RenderTime::WORLD) return;

	auto screenMgr = systems->getScreenManager();
	if(screenMgr->getActiveScreen() != nullptr) { return; };

	auto moduleManager = systems->getModuleManager();
	std::shared_ptr<Notifications> notifs = moduleManager->findModule<Notifications>();

	auto& hitResult = player->level->mHitResult;
	auto blockLocation = hitResult.blockPos;
	Vector3<float> newBlockLocation = blockLocation;

	if(player->getdimension()->getBlockSource()->getBlock(blockLocation)->blockLegacy->isAir()) {
		int range = 0;
		while (player->getdimension()->getBlockSource()->getBlock(newBlockLocation)->blockLegacy->isAir()) {
			if (range < (rint(this->range->getValueAs<float>()) * 10)){
				range++;
				Vector2<float> lookingVec = player->rotationComponent->Get();
				float xmath = cos((lookingVec.y + 90.0f) * (PI / 180.0f)) * 0.1;
				float zmath = sin((lookingVec.y + 90.0f) * (PI / 180.0f)) * 0.1;
				float ymath = sin((lookingVec.x) * -(PI / 180.0f)) * 0.1;
				newBlockLocation = Vector3(newBlockLocation.x + xmath, newBlockLocation.y + ymath, newBlockLocation.z + zmath);
			}
			else {
				this->needsTp = false;
				return;
			}
		}
	}

	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();
	GeometryBuilder builder(&renderer);

	auto plane = AABB<float>(Vector3<float>(floor(newBlockLocation.x), (float)(ceil(newBlockLocation.y)), floor(newBlockLocation.z)), Vector3<float>(floor(newBlockLocation.x) + 1, (float)(ceil(newBlockLocation.y)), floor(newBlockLocation.z) + 1));
	builder.buildAABB(plane, ColorF::parse("#FF0004FF"), true);

	if (this->needsTp) {
		this->needsTp = false;

		newBlockLocation.y += 2;
		player->setPosition(newBlockLocation);
		notifs->notify("Teleported to block!", "ClickTp");
	}
}
