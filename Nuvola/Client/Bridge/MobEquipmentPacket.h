#include "Packet.h"
#include "NetworkItemStackDescriptor.h"


class MobEquipmentPacket : public Packet
{
public:
	int64_t ActorRuntimeID; //0x0030
	NetworkItemStackDescriptor ItemStackDescriptor; //0x0038
	uint32_t InventorySlot; //0x0098
	uint32_t HotbarSlot; //0x009C
	uint8_t ContainerID; //0x00A0
	uint8_t InventorySlot2; //0x00A1
	uint8_t HotbarSlot2; //0x00A2
	uint8_t ContainerID2; //0x00A3
}; //Size: 0x0170