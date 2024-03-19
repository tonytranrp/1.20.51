#include "Rainbow.h"

#include <imgui.h>

Rainbow::Rainbow() : ThemeVariable()
{

}

std::shared_ptr<ThemeVariable> Rainbow::buildNew(const std::vector<std::string>& argTokens)
{
	auto cloned = std::make_shared<Rainbow>();
	if (!argTokens.empty())
	{
		if (argTokens.size() > 0)
		{
			cloned->speed = std::stof(argTokens[0]);
		}
		if (argTokens.size() > 1)
		{
			cloned->offset = std::stof(argTokens[1]);
		}
		if (argTokens.size() > 2)
		{
			cloned->saturation = std::stof(argTokens[2]);
		}
		if (argTokens.size() > 3)
		{
			cloned->brightness = std::stof(argTokens[3]);
		}
		if (argTokens.size() > 4)
		{
			cloned->alfuh = std::stof(argTokens[4]);
		}
	}
	return cloned;
}

void Rainbow::nextFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	this->progress += io.DeltaTime;
}

std::string Rainbow::getName()
{
	return "rainbow";
}

ColorF Rainbow::getColor()
{
	return ColorF::fromHSV((this->progress * this->speed) + this->offset, this->saturation, this->brightness, this->alfuh);
}