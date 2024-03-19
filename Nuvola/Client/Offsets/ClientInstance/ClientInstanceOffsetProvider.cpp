#include "ClientInstanceOffsetProvider.h"

ClientInstanceOffsetProvider::ClientInstanceOffsetProvider() : OffsetProvider("ClientInstance")
{
	//this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getGuiData, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 8b d0 48 8d 4d ? e8 ? ? ? ? 48 89 5d", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getClientPlayer, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 4c 8b e0 48 85 f6", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getItemInHandRenderer, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 89 46 ? 48 8b 07 48 8b cf 48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 89 46 ? 48 89 6e", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getOptions, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 8b f8 48 8b 8e ? ? ? ? 48 8b 01", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getProfanityContext, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 8d 0d ? ? ? ? 48 89 4e", 3);
	//getScreenName is called by InGamePlayScreen::_renderItemInHand, which I used to help update this sig
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_getScreenName, "48 8b 80 ? ? ? ? ff 15 ? ? ? ? 90 bb ? ? ? ? 89 5c 24 ? 4c 8b 40", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_dropCursor, "48 8b 80 ? ? ? ? eb ? 48 8b 0b", 3);
	this->addResult<int32_t>(OffsetTable::ClientInstance_vft_acquireCursor, "48 8b 80 ? ? ? ? 48 8b ce ff 15 ? ? ? ? 84 db", 3);

	this->addResult<int32_t>(OffsetTable::ClientInstance_mMinecraftGame, "48 8b 8f ? ? ? ? 48 8b 01 48 8b 80 ? ? ? ? ff 15 ? ? ? ? 85 c3", 3);
	this->addResult(OffsetTable::ClientInstance_mMinecraft, 0xD0);
	this->addResult<int32_t>(OffsetTable::ClientInstance_mLevelRenderer, std::string("48 8b 8f ? ? ? ? 48 85 c9 74 ? 48 8d 54 24 ? e8 ? ? ? ? 48 8b 07"), 3);
	this->addResult(OffsetTable::ClientInstance_mPacketSender, 0xF0);
	this->addResult(OffsetTable::ClientInstance_mClientInputHandler, 0x110);
	this->addResult(OffsetTable::ClientInstance_mLocalPlayer, 0x160);
	this->addResult<int32_t>(OffsetTable::ClientInstance_mCamera, std::string("48 8d 8e ? ? ? ? e8 ? ? ? ? 90 0f 10 05"), 3);
	//this->addResult(OffsetTable::ClientInstance_mUIProfanityContext, 0x4D8);
	//this->addResult(OffsetTable::ClientInstance_mBlockTessellator, 0x510);
	//this->addResult<int32_t>(OffsetTable::ClientInstance_mGuiData, std::string("4c 89 be ?? ?? ?? ?? 4c 89 be ?? ?? ?? ?? 4c 89 be ?? ?? ?? ?? 4c 89 be ?? ?? ?? ?? 0f b6 45"), 3);
	this->addResult(OffsetTable::ClientInstance_mGuiData, 0x560);
	this->addResult<int32_t>(OffsetTable::ClientInstance_mHmdState, std::string("48 8d 8e ? ? ? ? 48 8b d6 e8 ? ? ? ? 90 48 8d 8e ? ? ? ? e8"), 3);
}
