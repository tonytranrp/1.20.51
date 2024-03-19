#include "bridge_type.h"

#include "../../../../../Utils/TargetUtils.h"
#include "../../../../Bridge/ClientInstance.h"
#include "../../../../Bridge/Level.h"
#include "../../../../Bridge/LocalPlayer.h"
#include "../../../../Bridge/Player.h"

std::istream& bcs::operator>>(std::istream& stream, bridge_type<Player>& obj)
{
	if (stream.peek() != '@')
	{
		stream.setstate(std::ios::failbit);
		return stream;
	}

	//Seek 1 to remove the @ prefix
	stream.seekg(1);

	std::string fullName;
	stream >> fullName;

	ClientInstance* ci = Bridge::getClientInstance();
	LocalPlayer* lp = ci->getClientPlayer();
	if (!lp) return stream;
	std::vector<Actor*> toIterate = TargetUtils::findTargetMulti(
		lp,
		{ "minecraft:player" },
		-1.0f,
		true,
		false
	);

	Actor* target = nullptr;
	for (Actor* actor : toIterate) {
		if (actor->getNameTag().find(fullName) != std::string::npos) {
			target = actor;
			break;
		}
	}

	obj.player = reinterpret_cast<Player*>(target);

	return stream;
}