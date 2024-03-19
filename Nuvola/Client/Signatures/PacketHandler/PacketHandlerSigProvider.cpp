#include "PacketHandlerSigProvider.h"

#include <iostream>
#include <iomanip>
#include <sstream>

std::string packetSigFromOffset(uint32_t handlerOffset)
{
    int8_t* bytes = reinterpret_cast<int8_t*>(&handlerOffset);

    std::stringstream ss;
    ss << "49 8b 00 49 8b c8 4d 8b 01 48 8b 80"; //First half of the handler func

    //Add the hash with this
    for (int i = 0; i < sizeof(int32_t); i++)
    {
        ss << " " << std::setw(2) << std::setfill('0') << std::hex << (0xFF & bytes[i]);
    }
    return ss.str();
}

PacketHandlerSigProvider::PacketHandlerSigProvider() : SigProvider("PacketHandler")
{
	this->addResult(SigTable::PacketHandler_MovePlayerPacket_handle, packetSigFromOffset(0x5C8));
}
