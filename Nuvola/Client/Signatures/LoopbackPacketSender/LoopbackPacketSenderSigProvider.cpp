#include "LoopbackPacketSenderSigProvider.h"

#include <xorstr.hpp>

LoopbackPacketSenderSigProvider::LoopbackPacketSenderSigProvider() : SigProvider("LoopbackPacketSender")
{
	this->addResult(SigTable::LoopbackPacketSender_send, std::string("48 83 EC 28 48 0F BE 41 ? 48 83 C0 01 74 27 48 83 F8 01"));
}
