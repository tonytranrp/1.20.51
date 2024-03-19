#include "VClipCommand.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

VClipCommand::VClipCommand() : Command("vclip")
{
}

void VClipCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("vclip", "vc")
		.then(
			bcs::argument<float>("y")
			.executes([](bcs::command_context context) -> bool {
				float y = context.get<float>("y");

				ClientInstance* ci = Bridge::getClientInstance();
				LocalPlayer* lp = ci->getClientPlayer();
				if (!lp) return false;

				Vector3<float> curPos = lp->getPosition();
				curPos.y += y;

				lp->setPosition(curPos);
				return true;
			})
		)
	);
}

std::vector<std::string> VClipCommand::getAliases() const
{
	return { "vc" };
}

/*
bool VClipCommand::execute(const std::vector<std::string>& params)
{
	if (params.empty())
	{
		return false;
	}

	if (params.size() < 0)
	{
		return false;
	}

	try
	{
		auto strY = params.at(0);
		float y = strY.empty() ? 0.0f : std::stof(strY);

		ClientInstance* ci = Bridge::getClientInstance();
		LocalPlayer* lp = ci->getClientPlayer();
		if (!lp) return false;

		Vector3<float> curPos = lp->getPosition();
		curPos.y += y;

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

std::string VClipCommand::getDescription() const
{
	return "Modifies your Y value by a given amount";
}

std::string VClipCommand::getUsage() const
{
	return "Correct Usage: !vclip (y: float)";
}
