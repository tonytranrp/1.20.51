#include "SigManager.h"

#include "SigProvider.h"
#include "Actor/ActorSigProvider.h"
#include "Block/BlockSigProvider.h"
#include "BlockLegacy/BlockLegacySigProvider.h"
#include "BlockSource/BlockSourceSigProvider.h"
#include "ClientInstance/ClientInstanceSigProvider.h"
#include "ContainerScreenController/ContainerScreenControllerSigProvider.h"
#include "Dimension/DimensionSigProvider.h"
#include "DirectPlayerMovementProxy/DirectPlayerMovementProxySigProvider.h"
#include "Enchant/EnchantSigProvider.h"
#include "EnchantUtils/EnchantUtilsSigProvider.h"
#include "Global/GlobalSigProvider.h"
#include "GuiData/GuiDataSigProvider.h"
#include "GameMode/GameModeSigProvider.h"
#include "InventoryTransactionManager/InventoryTransactionManagerSigProvider.h"
#include "ItemStack/ItemStackSigProvider.h"
#include "Level/LevelSigProvider.h"
#include "LoopbackPacketSender/LoopbackPacketSenderSigProvider.h"
#include "MinecraftPackets/MinecraftPacketsSigProvider.h"
#include "Mob/MobSigProvider.h"
#include "SystemImpl/SystemImplSigProvider.h"
#include "NetworkItemStackDescriptor/NetworkItemStackDescriptorSigProvider.h"
#include "Packet/PacketSigProvider.h"
#include "PacketHandler/PacketHandlerSigProvider.h"
#include "RakNet/RakPeerSigProvider.h"
#include "ScreenView/ScreenViewSigProvider.h"
#include "StackResultStorageEntity/StackResultStorageEntitySigProvider.h"
#include "HudPlayerRenderer/HudPlayerRendererSigProvider.h"

SigManager::SigManager() : System<Manager<SigProvider>>("SigManager")
{

};

void SigManager::install(const std::shared_ptr<SystemInterface> &ownerPtr)
{
    this->provide<ActorSigProvider>();
    this->provide<BlockSigProvider>();
    this->provide<BlockLegacySigProvider>();
    this->provide<BlockSourceSigProvider>();
    this->provide<ClientInstanceSigProvider>();
    this->provide<ContainerScreenControllerSigProvider>();
	this->provide<DimensionSigProvider>();
	//TODO: Replace with a new sig for NoSlowdown. This one is outdated + this method doesnt work anymore anyway
    //this->provide<DirectPlayerMovementProxySigProvider>();
    //this->provide<EnchantSigProvider>();
    this->provide<EnchantUtilsSigProvider>();
    this->provide<GlobalSigProvider>();
    this->provide<GuiDataSigProvider>();
    this->provide<GameModeSigProvider>();
    this->provide<InventoryTransactionManagerSigProvider>();
    this->provide<ItemStackSigProvider>();
    this->provide<LevelSigProvider>();
    this->provide<LoopbackPacketSenderSigProvider>();
    this->provide<MinecraftPacketsSigProvider>();
	this->provide<MobSigProvider>();
	this->provide<SystemImplSigProvider>();
    this->provide<NetworkItemStackDescriptorSigProvider>();
    this->provide<PacketSigProvider>();
    this->provide<PacketHandlerSigProvider>();
    this->provide<RakPeerSigProvider>();
    this->provide<ScreenViewSigProvider>();
    //this->provide<StackResultStorageEntitySigProvider>();
    this->provide<HudPlayerRendererSigProvider>();

    this->awaitProviders();
}

void* SigManager::findAddressOf(SigTable signature)
{
	for (const auto& provider : this->getItems()) {
		void* ptr = provider->findFor(signature);
		if (ptr) {
			return ptr;
		}
	}
	__debugbreak();
	return nullptr;
}

void SigManager::awaitProviders() {
    for (const auto& provider : this->initializingProviders) {
        provider.wait();
    }
    this->initializingProviders.clear();
}
