#include "PacketDebugger.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../SystemManager.h"

#include <imgui.h>
#include <vector>

#include <xorstr.hpp>

PacketDebugger::PacketDebugger() : Module("PacketDebugger", "A module for watching packets.")
{
}

struct PacketLog {
	size_t id;
};
static std::vector<PacketLog> logs;

void PacketDebugger::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<PacketEvent, &PacketDebugger::onPacketEvent>(this);
	dispatcher->listen<RenderEvent, &PacketDebugger::onRenderEvent>(this);
	logs.clear();
}

void PacketDebugger::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<PacketEvent, &PacketDebugger::onPacketEvent>(this);
	dispatcher->deafen<RenderEvent, &PacketDebugger::onRenderEvent>(this);
	logs.clear();
}

void PacketDebugger::onRenderEvent(RenderEvent& event)
{
	if (this->isEnabled()) {
		ImGui::Begin("Packet Debugger");
		ImGui::SetWindowFontScale(0.5f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		for (PacketLog& log : logs) {
			ImGui::Text("ID: %d (%x)", log.id, log.id);
		}
		ImGui::PopStyleColor();
		ImGui::End();
	}
}

void PacketDebugger::onPacketEvent(PacketEvent& event)
{
	Packet* thePacket = event.getPacket();
	if(thePacket->getId() == PacketID::ItemStackRequestPacket)
		logs.emplace_back((size_t)thePacket->getId());
}
