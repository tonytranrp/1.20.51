#pragma once

#include "Components/ActorOwnerComponent.h"
#include "mce/Camera.h"
#include "ClientHMDState.h"
#include "GuiData.h"
#include "LevelRenderer.h"
#include "Macro.h"
#include "MinecraftGame.h"
#include "UIProfanityContext.h"
#include "WeakStorageEntity.h"

#include <Math/Math.h>

#include <string>
#include <functional>

class ClientInstance {
public:
	/* Fields */
	//char pad_0008[160]; //0x0008
	//class MinecraftGame* minecraftGame; //0x00A8
	//char pad_00B0[32]; //0x00B0
	////class Minecraft* minecraft; //0x00D0
	//char pad_00D8[16]; //0x00D8
	//class LevelRenderer* levelRenderer; //0x00E0
	//char pad_00E8[8]; //0x00E8
	//class LoopbackPacketSender* packetSender; //0x00F0
	//char pad_00F8[24]; //0x00F8
	//class ClientInputHandler* clientInputHandler; //0x0110
	//char pad_0118[536]; //0x0118
	//Matrix16<float> worldMatrix; //0x0330
	//char pad_0370[464]; //0x0370
	//class GuiData* guiData; //0x0540
	//char pad_0548[64]; //0x0548
	//ClientHMDState hmdState; //0x0588
	//char pad_0808[312]; //0x0808

	DEFINE_MEMBER(class MinecraftGame*, mMinecraftGame, OffsetTable::ClientInstance_mMinecraftGame);
	DEFINE_MEMBER(class Minecraft*, mMinecraft, OffsetTable::ClientInstance_mMinecraft);
	DEFINE_MEMBER(class LevelRenderer*, mLevelRenderer, OffsetTable::ClientInstance_mLevelRenderer);
	DEFINE_MEMBER(class LoopbackPacketSender*, mPacketSender, OffsetTable::ClientInstance_mPacketSender);
	DEFINE_MEMBER(class ClientInputHandler*, mClientInputHandler, OffsetTable::ClientInstance_mClientInputHandler);
	DEFINE_MEMBER(WeakStorageEntity, mLocalPlayer, OffsetTable::ClientInstance_mLocalPlayer);
	DEFINE_MEMBER(mce::Camera, mCamera, OffsetTable::ClientInstance_mCamera);
	//DEFINE_MEMBER(UIProfanityContext*, uiProfanityContext, OffsetTable::ClientInstance_mUIProfanityContext);
	DEFINE_MEMBER(class GuiData*, mGuiData, OffsetTable::ClientInstance_mGuiData);
	DEFINE_MEMBER(ClientHMDState, mHmdState, OffsetTable::ClientInstance_mHmdState);

	/* Virtuals */
	virtual ~ClientInstance();
	[[nodiscard]] class LocalPlayer* getClientPlayer() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_getClientPlayer, LocalPlayer*, const ClientInstance*>(this);
	}
	[[nodiscard]] class MinecraftGame* getMinecraftGame() const
	{
		return this->mMinecraftGame;
	}
	[[nodiscard]] bool isInGame() const
	{
		return this->mMinecraftGame->canInputMovement;
	}
	[[nodiscard]] class Options* getOptions() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_getOptions, Options*, const ClientInstance*>(this);
	}
	[[nodiscard]] class LevelRenderer* getLevelRenderer() const
	{
		return this->mLevelRenderer;
	}
	[[nodiscard]] class GuiData* getGuiData() const
	{
		return this->mGuiData;
		//return vftCall<OffsetTable::ClientInstance_vft_getGuiData, GuiData*, const ClientInstance*, std::string*>(this, &out);
	}
	const std::string& getScreenName(std::string& out) const
	{
		return vftCall<OffsetTable::ClientInstance_vft_getScreenName, const std::string&, const ClientInstance*, std::string&>(this, out);
	}
	[[nodiscard]] class LoopbackPacketSender* getPacketSender() const
	{
		return this->mPacketSender;
	}
	void dropCursor() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_dropCursor, void, const ClientInstance*>(this);
	}
	void acquireCursor() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_acquireCursor, void, const ClientInstance*>(this);
	}
	[[nodiscard]] UIProfanityContext* getProfanityContext() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_getProfanityContext, UIProfanityContext*, const ClientInstance*>(this);
	}
	[[nodiscard]] class ClientHMDState* getHMDState() const
	{
		//return vftCall<360, class ClientHMDState*, const ClientInstance*>(this);
		return &this->mHmdState;
	}
	[[nodiscard]] class ItemInHandRenderer* getItemInHandRenderer() const
	{
		return vftCall<OffsetTable::ClientInstance_vft_getItemInHandRenderer, ItemInHandRenderer*, const ClientInstance*>(this);
	}
	/* Functions */
	template<typename T>
	bool worldToScreen(const Vector3<T>& pos, Vector2<T>& out)
	{
		auto hmdState = this->getHMDState();
		auto resolution = this->getGuiData()->resolution;
		auto cameraPos = this->getLevelRenderer()->levelRendererPlayer->mOrigin;

		auto relativePos = pos - cameraPos;

		// Apply the MVP transformation to the worldPosition
		glm::mat4 mvpMatrix = hmdState->mLastLevelProjMatrix * hmdState->mLastLevelViewMatrix;
		Vector4<T> clipSpacePosition = mvpMatrix * Vector4<T>(relativePos.x, relativePos.y, relativePos.z, 1.0f);

		//If the w is less than 0, its not on the screen so we return false
		if (clipSpacePosition.w <= 0.0f)
			return false;

		// Perform perspective division
		clipSpacePosition /= clipSpacePosition.w;

		//Otherwise, map to screen space
		out = Vector2<T>(
			(clipSpacePosition.x + 1.0f) * (0.5f * resolution.x),
			(1.0f - clipSpacePosition.y) * (0.5f * resolution.y)
		);

		return true;
	}
};