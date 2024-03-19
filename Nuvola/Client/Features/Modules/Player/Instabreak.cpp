#include "Instabreak.h"

#include "../../../Bridge/Nuvola/Bridge.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/GameMode.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/KeyEvent.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/MouseEvent.h"
#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/GeometryBuilder.h"
#include "../../../GUI/ScreenManager.h"

#include <imgui.h>

Instabreak::Instabreak() : Module("Instabreak", "Instantly break blocks") {}

void Instabreak::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<MouseEvent, &Instabreak::onMouseEvent>(this);
	dispatcher->listen<KeyEvent, &Instabreak::onKeyEvent>(this);
	dispatcher->listen<LevelTickEvent, &Instabreak::onLevelTickEvent>(this);
}

void Instabreak::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<MouseEvent, &Instabreak::onMouseEvent>(this);
	dispatcher->deafen<KeyEvent, &Instabreak::onKeyEvent>(this);
	dispatcher->deafen<LevelTickEvent, &Instabreak::onLevelTickEvent>(this);
}

void Instabreak::onMouseEvent(MouseEvent &event) {
	if (event.getAction() != MouseAction::PRESS) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (cih == nullptr) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (cimf == nullptr) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (kms == nullptr) return;

	auto screenMgr = systems->getScreenManager();
	if(screenMgr->getActiveScreen() != nullptr) { return; };

	int key = kms->GetMCKey("key.attack");
	if (kms->isMouseButton(key))
	{
		int mouseBtn = kms->keyIdToMB(key);
		if (static_cast<int>(event.getButton()) == mouseBtn)
		{
			this->needsBreak = true;
		}
	}
}

void Instabreak::onKeyEvent(KeyEvent &event) {
	if (event.getAction() != KeyAction::PRESSED) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (cih == nullptr) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (cimf == nullptr) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (kms == nullptr) return;

	auto screenMgr = systems->getScreenManager();
	if(screenMgr->getActiveScreen() != nullptr) { return; };

	int key = kms->GetMCKey("key.attack");
	if (!kms->isMouseButton(key))
	{
		if (event.getKey() == key)
		{
			this->needsBreak = true;
		}
	}
}

void Instabreak::onLevelTickEvent(LevelTickEvent &event) {
	ClientInstance* ci = Bridge::getClientInstance();
	CHECK(ci);
	if (!ci->isInGame()) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	CHECK(cih);
	ClientInputMappingFactory* cimf = cih->factory;
	CHECK(cimf);
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	CHECK(kms);
	LocalPlayer* player = ci->getClientPlayer();
	CHECK(player);
	Level* level = player->level;
	CHECK(level);
	GameMode* gm = player->mGameMode;
	CHECK(gm);

	auto screenMgr = systems->getScreenManager();
	if(screenMgr->getActiveScreen() != nullptr) { return; };

	auto& hitResult = level->mHitResult;
	if (this->needsBreak && hitResult.type == HitResultType::BLOCK) {
		gm->destroyBlock(&hitResult.blockPos, hitResult.selectedFace);
	}
	this->needsBreak = false;
}

/*void Instabreak::onRenderEvent(RenderEvent &event) {
	if(event.getTime() != RenderTime::WORLD) return;

	ClientInstance* ci = Bridge::getClientInstance();
	CHECK(ci);
	if (!ci->isInGame()) return;
	LocalPlayer* player = ci->getClientPlayer();
	CHECK(player);
	Level* level = player->level;
	CHECK(level);

	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();
	GeometryBuilder builder(&renderer);

	builder.buildBlock(level->blockLocation, ColorF(1.0f, 1.0f, 1.0f, 0.5f), true);
}*/
