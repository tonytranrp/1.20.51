#include "Utils.h"

static std::string roamingState;
const std::string& Utils::GetRoamingState() {
	if (!roamingState.empty()) {
		return roamingState;
	}
	std::string dirP = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\");
	if (roamingState.empty()) {
		roamingState = dirP;
	}

	return roamingState;
}