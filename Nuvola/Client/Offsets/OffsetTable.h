#pragma once

enum class OffsetTable
{
	/* Actor */
	Actor_vft_swing,
	Actor_entityContext,
	Actor_entityData,
	Actor_ticksAlive,
	Actor_dimension,
	Actor_level,
	Actor_renderId,
	Actor_stateVectorComponent,
	Actor_aabbComponent,
	Actor_rotationComponent,
	Actor_distanceTraveledComponent,
	Actor_dead,
	Actor_armorContainer,
	Actor_handContainer,
	Actor_isDestroying,

	/* ClientHMDState */
	ClientHMDState_mLastLevelViewMatrix,
	ClientHMDState_mLastLevelViewMatrixAbsolute,
	ClientHMDState_mLastLevelProjMatrix,
	ClientHMDState_mLastLevelWorldMatrix,
	ClientHMDState_mHUDMatrixPatch,
	ClientHMDState_mVRTransitionMatrixPatch,
	ClientHMDState_mLastLevelViewMatrixVerticalShift,
	ClientHMDState_mLastVRPitchAngle,
	ClientHMDState_mVRRotationAdjustment,
	ClientHMDState_mVRRotAdjYawTweak,
	ClientHMDState_mGazeCursorPitchBoostAngle,
	ClientHMDState_mFadeScreenAlpha,
	ClientHMDState_mDesiredFadeScreenAlpha,
	ClientHMDState_mRealityFrameModeWorldScale,
	ClientHMDState_mHeadSetDirForSleeping,
	ClientHMDState_mPointerX,
	ClientHMDState_mPointerY,
	ClientHMDState_mHoloCursorOn,
	ClientHMDState_mAppJustResumed,

	/* ClientInstance */
	ClientInstance_vft_getGuiData,
	ClientInstance_vft_getClientPlayer,
	ClientInstance_vft_getItemInHandRenderer,
	ClientInstance_vft_getOptions,
	ClientInstance_vft_getProfanityContext,
	ClientInstance_vft_getScreenName,
	ClientInstance_vft_dropCursor,
	ClientInstance_vft_acquireCursor,
	ClientInstance_mMinecraftGame,
	ClientInstance_mMinecraft,
	ClientInstance_mLevelRenderer,
	ClientInstance_mClientInputHandler,
	ClientInstance_mPacketSender,
	ClientInstance_mLocalPlayer,
	ClientInstance_mCamera,
	//ClientInstance_mUIProfanityContext,
	//ClientInstance_mBlockTessellator,
	ClientInstance_mGuiData,
	ClientInstance_mHmdState,

	/* Gamemode */
	GameMode_breakProgress,

	/* Level */
	Level_mBlockPalette,
	Level_mActorFactory,
	Level_mActorManager,
	Level_mHitResult,
	/* LevelRendererPlayer */
	LevelRendererPlayer_mOrigin,
	LevelRendererPlayer_mLastOrigin,

	/* LocalPlayer */
	LocalPlayer_LocalTicksAlive,

	/* Mob */
	Mob_mSwingProgress,
	Mob_mIsSwinging,
	Mob_mSwingTicks,
	Mob_mHurtTimeComponent,

	/* Options */
	Options_OptionID_gfx_gamma,

	/* Player */
	Player_mPlayerInventory,
	Player_mInventoryTransactionManager,
	Player_mGameMode,
	Player_mItemStackNetManagerClient,
	Player_mName,
	Player_mIContainerManager,
};