#include "AirPlace.h"

#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GameMode.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/KeyEvent.h"
#include "../../../Events/MouseEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/WorldRenderer.h"
#include "../../../Rendering/GeometryBuilder.h"

#include "../../Theme/ThemeManager.h"

#include <imgui.h>
#include <imgui_custom.h>

AirPlace::AirPlace() : Module("AirPlace", "Allows you to place blocks in the air.")
{

}


void AirPlace::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &AirPlace::onLevelTick>(this);
	dispatcher->listen<MouseEvent, &AirPlace::onMouseEvent>(this);
	dispatcher->listen<KeyEvent, &AirPlace::onKeyEvent>(this);
	dispatcher->listen<RenderEvent, &AirPlace::onRenderEvent>(this);
}

void AirPlace::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &AirPlace::onLevelTick>(this);
	dispatcher->deafen<MouseEvent, &AirPlace::onMouseEvent>(this);
	dispatcher->deafen<KeyEvent, &AirPlace::onKeyEvent>(this);
	dispatcher->deafen<RenderEvent, &AirPlace::onRenderEvent>(this);
}

void AirPlace::onLevelTick(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	GameMode* gm = lp->mGameMode;
	if (gm == nullptr) return;

	if (needsPlace)
	{
		gm->buildBlock(blockPos, 0);
	}
	needsPlace = false;
}

void AirPlace::onMouseEvent(MouseEvent& event)
{
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

	int key = kms->GetMCKey("key.use");
	if (kms->isMouseButton(key))
	{
		int mouseBtn = kms->keyIdToMB(key);
		if (static_cast<int>(event.getButton()) == mouseBtn)
		{
			this->needsPlace = true;
		}
	}
}
void AirPlace::onKeyEvent(KeyEvent& event)
{
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

	int key = kms->GetMCKey("key.use");
	if (!kms->isMouseButton(key))
	{
		if (event.getKey() == key)
		{
			this->needsPlace = true;
		}
	}
}

void AirPlace::onRenderEvent(RenderEvent& event)
{
	if (event.getTime() != RenderTime::WORLD) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	if (!ci->isInGame()) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (cih == nullptr) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (cimf == nullptr) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (kms == nullptr) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	GameMode* gm = lp->mGameMode;
	if (gm == nullptr) return;
	Level* level = lp->level;
	if (level == nullptr) return;
	auto& hitResult = level->mHitResult;
	std::shared_ptr<Dimension> dim = lp->dimension;
	if (dim == nullptr) return;
	BlockSource* source = dim->blockSource;
	if (source == nullptr) return;
	if (hitResult.selectedFace != 0) return;

	int currentSlot = lp->mInventory->selectedSlot;
	const ItemStack& inHand = lp->mInventory->container->stacks[currentSlot];
	if (inHand.block == nullptr) return;

	const Vector3<float>& hitPos = hitResult.endPos;
	blockPos.x = (int)floor(hitPos.x);
	blockPos.y = (int)floor(hitPos.y);
	blockPos.z = (int)floor(hitPos.z);

	const Block* block = source->getBlock(blockPos);
	if (block == nullptr) return;
	BlockLegacy* legacy = block->blockLegacy;
	if (legacy == nullptr) return;

	if(!legacy->isAir()) return;

	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();
	GeometryBuilder builder(&renderer);
	Theme theme = systems->getThemeManager()->getActiveTheme();

	builder.buildBlock(blockPos, theme[ThemeColor::AIRPLACE_OUTLINE], false, 1.0f);
}
