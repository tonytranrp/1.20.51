#include "PatchManager.h"

#include <iostream>

//Actor
#include "Actor/RenderDispatcherHook.h"
#include "Actor/SetVelocityHook.h"
//BlockSource
#include "BlockSource/FireBlockChangedHook.h"
//ClientInstance
#include "ClientInstance/UpdateHook.h"
//ContainerScreenController
#include "ContainerScreenController/TickHook.h"
//DirectX hooks
#include <kiero.h>
#include "D3D12/ExecCommandListHook.h"
#include "D3D12/SignalHook.h"
#include "DXGI/PresentHook.h"
#include "DXGI/ResizeBuffersHook.h"
//Global or Unknown
#include "Global/CameraDeltaHook.h"
//#include "Global/DecoupleRotations.h"
#include "Global/KeyPressHook.h"
#include "Global/MouseActionHook.h"
//Level
#include "Level/TickHook.h"
//LoopbackPacketSender
#include "LoopbackPacketSender/SendHook.h"
//PacketHandler
#include "PacketHandler/GenericPacketHandlerHook.h"
//RakNet
#include "RakNet/RakPeer/RunUpdateCycleHook.h"
//ScreenView
#include "ScreenView/SetupAndRenderHook.h"
//SystemImpl
#include "ItemUseSlowdownSystemImpl/ItemUseSlowdownHook.h"
#include "MobJumpFromGroundSystemImpl/MobJumpFromGroundHook.h"
//Dimension
#include "Dimension/GetBrightnessDependentFogColorHook.h"
#include "Dimension/GetCloudHeightHook.h"

//GameMode
#include "GameMode/blockStartDestroyHook.h"
#include "GameMode/attackActorHook.h"

#include <magic_enum.hpp>
#include <magic_enum_utility.hpp>

PatchManager::PatchManager() : ::System<Manager<IPatch>>("PatchManager")
{
    this->depends<EventDispatcher>();
    this->depends<SigManager>();
}

bool PatchManager::removeAll() {
	for(auto patch : this->getItems()) {
		this->removePatch(patch);
	}
	kiero::shutdown();
	return true;
}
bool PatchManager::removePatch(std::shared_ptr<IPatch> toRemove) {
	if(toRemove && !toRemove->manualCleanup()) {
		if (!toRemove->autoCleanup()) {
			__debugbreak();
		}
	}
	this->removeItem(toRemove);
	return true;
}

void PatchManager::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
    System::install(ownerPtr);

    /* Global hooking */
    this->applyPatch<CameraDeltaHook>();
    //this->applyPatch<DecoupleRotations>();
    this->applyPatch<KeyPressHook>();
    this->applyPatch<MouseActionHook>();

    /* BlockSource hooks */
	this->applyPatch<FireBlockChangedHook>();

    /* ClientInstance reference receiver hook */
    this->applyPatch<UpdateHook>();

    /* ContainerScreenController */
    this->applyPatch<_ContainerScreenController::TickHook>();

    /* Graphics hooking */
    this->applyPatch<SetupAndRenderHook>();
    //Kiero is useful for getting the func addresses we need.
    //Kiero is used in the directx hooks
    kiero::Status::Enum kiero_status = kiero::init(kiero::RenderType::D3D12);
    if (kiero_status != kiero::Status::Success)
        kiero_status = kiero::init(kiero::RenderType::D3D11);
    bool kieroReady = (kiero_status == kiero::Status::Success);
	assert(kieroReady);
    if (kieroReady) {
        this->applyPatch<ExecCommandListHook>();
        this->applyPatch<SignalHook>();
        this->applyPatch<PresentHook>();
        this->applyPatch<ResizeBuffersHook>();
    }

    /* LoopbackPacketSender patches */
    this->applyPatch<SendHook>();

    /* RakNet patches */
    this->applyPatch<RunUpdateCycleHook>();

    /* Actor patches */
    this->applyPatch<RenderDispatcherHook>();
	this->applyPatch<SetVelocityHook>();

	/* SystemImpl */
	this->applyPatch<ItemUseSlowdownHook>();
	this->applyPatch<MobJumpFromGroundHook>();

    /* PacketHandler patches */
    magic_enum::enum_for_each<PacketID>([&](auto val) {
        constexpr PacketID pktId = val;
        if (GenericPacketHandlerHook<pktId>::HandlerExists())
        {
            this->applyPatch<GenericPacketHandlerHook<pktId>>();
        }
    });

    /* Level patches */
    this->applyPatch<_Level::TickHook>();

	/* Dimension hooks, why is this codebase like this */
	//The codebase is like this so everything is hyper-organized and super easy to maintain in the long-run
    //thankyouuu asmmmm
	this->applyPatch<GetBrightnessDependentFogColorHook>();
    this->applyPatch<GetCloudHeightHook>();
    /* Gamemode */
    this->applyPatch<blockStartDestroyHook>();
    this->applyPatch<attackActorHook>();
}
