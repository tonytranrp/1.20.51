#include "DiscordRPCIntegration.h"
#include <iostream>


std::vector<std::string> DiscordRPCStatusStrings = {
	/* DEFAULT */
	"In the injector",

	/* PLAYING, NOT_INJECTED, DOWNLOADING, etc */
	"Playing Minecraft",
	"In the injector",
	"Downloading Nuvola"
};

static auto CurrentTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void DiscordRPCIntegration::Start()
{
	std::cout << "Starting Discord RPC" << std::endl;

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize("1136371033063043122", &handlers, 1, nullptr);

	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(DiscordRichPresence));
	
	std::string detail = "Injector v";
	detail.append(STRING(INJECTOR_BUILD_TAG));

	presence.details = detail.c_str();
	presence.state = DiscordRPCStatusStrings.at((int)DiscordRPCStatus::DEFAULT).c_str();
	presence.largeImageKey = "nuvola";
	presence.startTimestamp = CurrentTime;
	Discord_UpdatePresence(&presence);

	std::cout << "Started Discord Presence" << std::endl;
}

void DiscordRPCIntegration::UpdatePresence(DiscordRPCStatus status)
{
	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(DiscordRichPresence));

	std::string detail = "Injector v";
	detail.append(STRING(INJECTOR_BUILD_TAG));

	presence.details = detail.c_str();
	presence.largeImageKey = "nuvola";


	presence.state = DiscordRPCStatusStrings.at((int)status).c_str();

	presence.startTimestamp = CurrentTime;

	Discord_UpdatePresence(&presence);
}

void DiscordRPCIntegration::Shutdown()
{
	Discord_Shutdown();
}