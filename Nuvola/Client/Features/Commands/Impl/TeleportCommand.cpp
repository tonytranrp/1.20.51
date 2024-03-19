#include "TeleportCommand.h"

#include "../../../../Utils/TargetUtils.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../Based/extra/bridge_type.h"
#include "../Based/extra/relative_type.h"

TeleportCommand::TeleportCommand() : Command("teleport")
{
}

void TeleportCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("teleport", "tp")
		.then(
			bcs::argument<bcs::bridge_type<Player>>("player")
			.executes([](bcs::command_context context) -> bool {
				auto targetRef = context.get<bcs::bridge_type<Player>>("player");

				ClientInstance* ci = Bridge::getClientInstance();
				LocalPlayer* lp = ci->getClientPlayer();
				if (!lp) return false;

				if (targetRef)
				{
					auto target = targetRef.get();
					lp->setPosition(target->getPosition());
					displayMessage(std::format("Teleported to {}!", target->mPlayerName));
				}
				return true;
			})
		)
		.then(
			//Passing true type enables potential relativity
			bcs::argument<bcs::relative<float>>("x")
			.then(
				bcs::argument<bcs::relative<float>>("y")
				.then(
					bcs::argument<bcs::relative<float>>("z")
					.executes([](bcs::command_context context) -> bool {
						bcs::relative<float> x = context.get<bcs::relative<float>>("x");
						bcs::relative<float> y = context.get<bcs::relative<float>>("y");
						bcs::relative<float> z = context.get<bcs::relative<float>>("z");

						ClientInstance* ci = Bridge::getClientInstance();
						LocalPlayer* lp = ci->getClientPlayer();

						Vector3<float> curPos = lp->getPosition();
						curPos.x = x ? curPos.x + float(x) : x;
						curPos.y = y ? curPos.y + float(y) : y;
						curPos.z = z ? curPos.z + float(z) : z;

						lp->setPosition(curPos);
						return true;
					})
				)
			)
		)
	);
}

std::vector<std::string> TeleportCommand::getAliases() const
{
	return { "tp" };
}

/*
bool TeleportCommand::execute(const std::vector<std::string>& params)
{
	if(params.empty())
	{
		return false;
	}

	if (params.at(0).starts_with("@")) {
		try
		{
			std::string fullName = "";

			int thing = 0;
			for (std::string param : params) {
				thing = thing + 1;
				std::string thing2 = params.size() == thing ? "" : " ";
				std::string thing3 = thing == 1 ? param.replace(0, 1, "") : param;
				fullName = fullName + thing3 + thing2;
			}

			ClientInstance* ci = Bridge::getClientInstance();
			LocalPlayer* lp = ci->getClientPlayer();

			if (!lp) return false;
			std::vector<Actor*> toIterate = TargetUtils::findTargetMulti(lp,
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

			if (target == nullptr) {
				this->displayMessage("&cNo player was in render with that name");
				return true;
			}

			lp->setPosition(target->getPosition());
			this->displayMessage("Teleported to player!");
			return true;

		}
		catch (std::exception& ex)
		{
			this->displayMessage(ex.what());
			return false;
		}

		return true;
	}
	else {
		if (params.size() < 3)
		{
			return false;
		}

		try
		{
			auto strX = params.at(0);
			auto strY = params.at(1);
			auto strZ = params.at(2);

			bool isRelX = strX.at(0) == '~';
			bool isRelY = strY.at(0) == '~';
			bool isRelZ = strZ.at(0) == '~';

			strX = isRelX ? strX.substr(1) : strX;
			strY = isRelY ? strY.substr(1) : strY;
			strZ = isRelZ ? strZ.substr(1) : strZ;

			float x = strX.empty() ? 0.0f : std::stof(strX);
			float y = strY.empty() ? 0.0f : std::stof(strY);
			float z = strZ.empty() ? 0.0f : std::stof(strZ);

			ClientInstance* ci = Bridge::getClientInstance();
			LocalPlayer* lp = ci->getClientPlayer();
			if (!lp) return false;

			Vector3<float> curPos = lp->getPosition();
			curPos.x = isRelX ? curPos.x + x : x;
			curPos.y = isRelY ? curPos.y + y : y;
			curPos.z = isRelZ ? curPos.z + z : z;

			lp->setPosition(curPos);
			return true;
		}
		catch (std::exception& ex)
		{
			return false;
		}

		return true;
	}
}
*/

std::string TeleportCommand::getDescription() const
{
	return "Teleports you to a specific location";
}

std::string TeleportCommand::getUsage() const
{
	return "Correct Usage: !teleport (x: float) (y: float) (z: float) | !teleport @(playername: string)";
}
