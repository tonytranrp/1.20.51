#pragma once

#include <glm/mat4x4.hpp>

#include "Macro.h"

class ClientHMDState
{
	Sizer<0x280> size;
public:
	DEFINE_MEMBER(glm::mat4,	mLastLevelViewMatrix,				OffsetTable::ClientHMDState_mLastLevelViewMatrix);
	DEFINE_MEMBER(glm::mat4,	mLastLevelViewMatrixAbsolute,		OffsetTable::ClientHMDState_mLastLevelViewMatrixAbsolute);
	DEFINE_MEMBER(glm::mat4,	mLastLevelProjMatrix,				OffsetTable::ClientHMDState_mLastLevelProjMatrix);
	DEFINE_MEMBER(glm::mat4,	mLastLevelWorldMatrix,				OffsetTable::ClientHMDState_mLastLevelWorldMatrix);
	DEFINE_MEMBER(glm::mat4,	mHUDMatrixPatch,					OffsetTable::ClientHMDState_mHUDMatrixPatch);
	DEFINE_MEMBER(glm::mat4,	mVRTransitionMatrixPatch,			OffsetTable::ClientHMDState_mVRTransitionMatrixPatch);
	DEFINE_MEMBER(float,		mLastLevelViewMatrixVerticalShift,	OffsetTable::ClientHMDState_mLastLevelViewMatrixVerticalShift);
	DEFINE_MEMBER(float,		mLastVRPitchAngle,					OffsetTable::ClientHMDState_mLastVRPitchAngle);
	DEFINE_MEMBER(float,		mVRRotationAdjustment,				OffsetTable::ClientHMDState_mVRRotationAdjustment);
	DEFINE_MEMBER(float,		mVRRotAdjYawTweak,					OffsetTable::ClientHMDState_mVRRotAdjYawTweak);
	DEFINE_MEMBER(float,		mGazeCursorPitchBoostAngle,			OffsetTable::ClientHMDState_mGazeCursorPitchBoostAngle);
	DEFINE_MEMBER(float,		mFadeScreenAlpha,					OffsetTable::ClientHMDState_mFadeScreenAlpha);
	DEFINE_MEMBER(float,		mDesiredFadeScreenAlpha,			OffsetTable::ClientHMDState_mDesiredFadeScreenAlpha);
	DEFINE_MEMBER(float,		mRealityFrameModeWorldScale,		OffsetTable::ClientHMDState_mRealityFrameModeWorldScale);
	DEFINE_MEMBER(float,		mHeadSetDirForSleeping,				OffsetTable::ClientHMDState_mHeadSetDirForSleeping);
	DEFINE_MEMBER(int16_t,		mPointerX,							OffsetTable::ClientHMDState_mPointerX);
	DEFINE_MEMBER(int16_t,		mPointerY,							OffsetTable::ClientHMDState_mPointerY);
	DEFINE_MEMBER(bool,			mHoloCursorOn,						OffsetTable::ClientHMDState_mHoloCursorOn);
	DEFINE_MEMBER(bool,			mAppJustResumed,					OffsetTable::ClientHMDState_mAppJustResumed);
}; //Size: 0x0280
static_assert(sizeof(ClientHMDState) == 0x280);
