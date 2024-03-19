#pragma once

enum class SigTable {
	/* ActorRenderDispatcher */
	ActorRenderDispatcher_render,

	/* Actor */
	Actor_move,
	//Actor_normalTick,
	Actor_setVelocity,
	//Actor_tryGetFromEntity,

	/* Block */
	Block_getRenderLayer,

	/* BlockLegacy */
	BlockLegacy_getRenderLayer,

    /* BlockSource */
    BlockSource_vftable,
    BlockSource_getBlock,
	BlockSource_fireBlockChanged,

	/* ClientInstance */
	ClientInstance_vftable,
	ClientInstance_frameUpdate,
	ClientInstance_getInput,
	ClientInstance_update,

	/* ContainerScreenController */
	ContainerScreenController_tick,
	ContainerScreenController__handleAutoPlace,
	ContainerScreenController__handleDropItem,
	ContainerScreenController__handlePlaceAll,
	ContainerScreenController__handleTakeAll,

	/* Dimension */
	Dimension_getBrightnessDependentFogColor,
	Dimension_getCloudHeight,

	/* DirectPlayerMovementProxy */
	DirectPlayerMovementProxy_isSlowedByItemUse,

	/* Enchant */
	Enchant_mEnchants,

	/* EnchantUtils */
	EnchantUtils_getEnchantLevel,
	EnchantUtils_getEnchantNameAndLevel,

	/* GameMode */
	GameMode_getDestroyRate,
	GameMode_startDestroyBlock,
	GameMode_attack,

	/* Global */
	Global_cameraDelta,
	Global_creativeReachPtr,
	//Global_decoupleRotations,
	Global_mouseAction,
	Global_keyAction,
	Global_reachPtr,

	/* GuiData */
	GuiData_displayMessage,

	/* InteractPacket */
	InteractPacket_cctor,

	/* InventoryTransactionManager */
	InventoryTransactionManager_addAction,

	/* ItemStack */
	ItemStackBaseOperatorEquals,
	ItemStackBase_cctor,

	/* ItemUseSlowdownSystemImpl */
	ItemUseSlowdownSystemImpl_doItemUseSlowdownSystem,

	/* Level */
	Level_tick,

	/* LoopbackPacketSender */
	LoopbackPacketSender_send,

	/* MinecraftPackets */
	MinecraftPackets_createPacket,

	/* Mob */
	Mob_getJumpPower,

	/* SystemImpl */
	MobJumpFromGroundSystemImpl_mobJumpFromGround,

	/* MovePlayerPacket */
	MovePlayerPacket_cctor,

	/* NetworkItemStackDescriptor */
	NetworkItemStackDescriptor_cctor,

	/* PacketHandler */
	PacketHandler_MovePlayerPacket_handle,

	/* RakNet::RakPeer */
	RakNet_RakPeer_RunUpdateCycle,

	/* ScreenView */
	ScreenView_setupAndRender,

	/* StackResultStorageEntity */
	StackResultStorageEntity_cctor,
	
	/* HudPlayerRenderer */
	HudPlayerRenderer_render,

};