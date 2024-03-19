#include "HClipCommand.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

HClipCommand::HClipCommand() : Command("hclip")
{
}

void HClipCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("hclip", "hc")
		.then(
			bcs::argument<float>("x")
			.then(
				bcs::argument<float>("z")
				.executes([](bcs::command_context context) -> bool {
					float x = context.get<float>("x");
					float z = context.get<float>("z");

					ClientInstance* ci = Bridge::getClientInstance();
					LocalPlayer* lp = ci->getClientPlayer();
					if (!lp) return false;

					Vector3<float> curPos = lp->getPosition();
					curPos.x += x;
					curPos.z += z;

					lp->setPosition(curPos);
					return true;
				})
			)
		)
	);
}

std::vector<std::string> HClipCommand::getAliases() const
{
	return { "hc" };
}

/*
bool HClipCommand::execute(const std::vector<std::string>& params)
{
	if(params.empty())
	{
		return false;
	}

	if (params.size() < 2)
	{
		return false;
	}

	try
	{
		auto strX = params.at(0);
		auto strZ = params.at(1);

		float x = strX.empty() ? 0.0f : std::stof(strX);
		float z = strZ.empty() ? 0.0f : std::stof(strZ);

		ClientInstance* ci = Bridge::getClientInstance();
		LocalPlayer* lp = ci->getClientPlayer();
		if (!lp) return false;

		Vector3<float> curPos = lp->getPosition();
		curPos.x += x;
		curPos.z += z;

		lp->setPosition(curPos);
		return true;
	}
	catch (std::exception& ex)
	{
		return false;
	}

	return true;
}
*/

std::string HClipCommand::getDescription() const
{
	return "Teleports you to a specific location in horiozontal";
}

std::string HClipCommand::getUsage() const
{
	return "Correct Usage: !hclip (x: float) (z: float)";
}
