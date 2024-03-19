#include "ClientHMDStateOffsetProvider.h"

ClientHMDStateOffsetProvider::ClientHMDStateOffsetProvider() : OffsetProvider("ClientHMDState")
{
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mLastLevelViewMatrix, std::string("4c 8d 88 ? ? ? ? 4d 8b c1"), 3);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mLastLevelViewMatrixAbsolute, std::string("f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? f3 0f 10 45 ? f3 0f 11 80 ? ? ? ? f3 0f 10 4d ? f3 0f 11 88 ? ? ? ? 48 85 db"), 4);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mLastLevelProjMatrix, std::string("4c 8d 80 ? ? ? ? 49 8b d0 48 8d 85"), 3);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mLastLevelWorldMatrix, std::string("89 88 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 41 8b 40 ? 41 89 81 ? ? ? ? 48 85 f6 74 ? 48 8b ce e8 ? ? ? ? 48 8b 83 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 48 83 38 ? 0f 84 ? ? ? ? 48 8b 83 ? ? ? ? 48 85 c0 74 ? f0 ff 40 ? 48 8b 8b ? ? ? ? 48 89 4c 24 ? 48 8b b3 ? ? ? ? 48 89 74 24 ? 48 8b 09 48 8b 01 48 8b 80 ? ? ? ? ff 15 ? ? ? ? 41 b9"), 2);
	this->addResult(OffsetTable::ClientHMDState_mHUDMatrixPatch, 0x1C8);
	this->addResult(OffsetTable::ClientHMDState_mVRTransitionMatrixPatch, 0x208);
	this->addResult(OffsetTable::ClientHMDState_mLastLevelViewMatrixVerticalShift, 0x248);
	this->addResult(OffsetTable::ClientHMDState_mLastVRPitchAngle, 0x24C);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mVRRotationAdjustment, std::string("f3 44 0f 11 80 ? ? ? ? 48 8b 96"), 5);
	this->addResult(OffsetTable::ClientHMDState_mVRRotAdjYawTweak, 0x254);
	this->addResult(OffsetTable::ClientHMDState_mGazeCursorPitchBoostAngle, 0x258);
	this->addResult(OffsetTable::ClientHMDState_mFadeScreenAlpha, 0x25C);
	this->addResult(OffsetTable::ClientHMDState_mDesiredFadeScreenAlpha, 0x260);
	this->addResult(OffsetTable::ClientHMDState_mRealityFrameModeWorldScale, 0x264);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mHeadSetDirForSleeping, std::string("f3 0f 11 83 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? 90"), 4);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mPointerX, std::string("44 0f bf a0 ? ? ? ? 44 0f bf a8"), 4);
	this->addResult<int32_t>(OffsetTable::ClientHMDState_mPointerY, std::string("44 0f bf a8 ? ? ? ? 48 8b 86"), 4);
	this->addResult(OffsetTable::ClientHMDState_mHoloCursorOn, 0x270);
	this->addResult(OffsetTable::ClientHMDState_mAppJustResumed, 0x271);
}