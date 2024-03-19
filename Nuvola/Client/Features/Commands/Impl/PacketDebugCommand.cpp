#include "PacketDebugCommand.h"

#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/GuiData.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"

PacketDebugCommand::PacketDebugCommand() : Command("packetdebugger") {
	systems->getEventDispatcher()->listen<PacketEvent, &PacketDebugCommand::onPacketEvent>(this);
}

void PacketDebugCommand::initialize(bcs::command_dispatcher& dispatcher)
{
}

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x" << std::hex << i;
  return stream.str();
}

bool resolveNext = false;
PacketID packetID;
void PacketDebugCommand::onPacketEvent(PacketEvent& event) {
	//If theres no task, don't do anything!
	//if(currentTask == Task::NONE) { return; }

	if(resolveNext) {
		if(event.getPacket()->getId() == packetID) {
			resolveNext = false;
			displayMessage("Resolved packet with ID (" + std::to_string((int)packetID) + ") as \"" + event.getPacket()->getName() + "\" associated VTable is "+int_to_hex(*(void**)event.getPacket()));
		}
	}
}

/*
bool PacketDebugCommand::execute(const std::vector<std::string>& params) {

	if(params.empty()) {
		displayMessage("&7Nothing to do");
		return false;
	}

	if(params[0] == "resolve") {
		if(params.size() > 1) {
			resolveNext = true;
			packetID = (PacketID)std::stoi(params[1]);
			displayMessage("Waiting for next resolve for packet "+std::to_string((int)packetID));
		} else {
			return false;
		}
	}

	return true;
};
*/

std::string PacketDebugCommand::getDescription() const {
	return "A packet 'debugger' (its really a dumping/interception tool). This is only useful for developers.";
}

std::vector<std::string> PacketDebugCommand::getAliases() const {
	std::vector<std::string> aliases;
	aliases.emplace_back("pdbg");
	return aliases;
}