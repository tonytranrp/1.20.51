#include "AntiVoid.h"

#include "../../../Bridge/BlockSource.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Dimension.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/MovePLayerPacket.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"

AntiVoid::AntiVoid() : Module("AntiVoid", "Prevents you from falling into the void.")
{}

static std::optional<Vector3<float>> lastGrounded;
static std::vector<PlayerAuthInputPacket> paips;
static std::vector<MovePlayerPacket> mpps;

void AntiVoid::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<PacketEvent, &AntiVoid::onPacketEvent>(this);
}

void AntiVoid::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<PacketEvent, &AntiVoid::onPacketEvent>(this);
}

void AntiVoid::onPacketEvent(PacketEvent& event)
{
	Packet* packet = event.getPacket();
	PlayerAuthInputPacket* paip = nullptr;
	MovePlayerPacket* mpp = nullptr;
	if (packet->getId() == PacketID::PlayerAuthInputPacket)
	{
		paip = (PlayerAuthInputPacket*)packet;
		//paip->inputData |= AuthInputAction::JUMPING;
	}
	else if (packet->getId() == PacketID::MovePlayerPacket)
	{
		mpp = (MovePlayerPacket*)mpp;
	}
	else
	{
		return;
	}

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	std::shared_ptr<Dimension> dim = player->dimension;
	if (dim == nullptr) return;
	BlockSource* source = dim->blockSource;
	if (source == nullptr) return;

	bool overVoid = true;
	for (int y = player->getBlockPos().y; y > -64; y--)
	{
		int x = player->getBlockPos().x;
		int z = player->getBlockPos().z;

		const Block* block = source->getBlock(Vector3<int>(x, y, z));
		if (block->blockLegacy != nullptr && !block->blockLegacy->isAir())
		{
			overVoid = false;
		}
	}

	if (overVoid && lastGrounded.has_value() && paip)
	{
		Vector3<float> originalPosition = paip->position;
		paip->position = lastGrounded.value();
		AuthInputAction originalActions = paip->inputData;
		paip->inputData = (AuthInputAction)0;
		float originalFall = paip->downVelocity;
		paip->downVelocity = -0.0784f;

		if (lastGrounded->y - player->getBlockPos().y >= 5.0f)
		{
			player->setPosition(lastGrounded.value());
			player->stateVectorComponent->velocity *= 0;
		}
		else
		{
			paip->position = originalPosition;
			paip->inputData = originalActions;
			paip->downVelocity = originalFall;
		}
	}
	else
	{
		lastGrounded = player->getPosition();
		lastGrounded->y = floor(lastGrounded->y + 0.4f);
	}
}