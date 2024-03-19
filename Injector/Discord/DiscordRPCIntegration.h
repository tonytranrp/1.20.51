#ifndef DRPC_HEADER
#define DRPC_HEADER

#include <string>
#include <chrono>
#include "Library/discord_rpc.h"
#include <Utils/Utils.h>
#include <vector>

enum class DiscordRPCStatus : int
{
	DEFAULT,
	PLAYING,
	NOT_INJECTED,
	DOWNLOADING
};


class DiscordRPCIntegration
{
public:
	DiscordRPCIntegration() {}
private:
	const int AppID = 1136371033063043122;
public:
	void Start();
	void UpdatePresence(DiscordRPCStatus);
	void Shutdown();
};
#endif