#pragma once

#include <cstdint>

enum class CommandPermissionLevel : int8_t
{
	Any = 0x0,
	GameMasters = 0x1,
	Admin = 0x2,
	Host = 0x3,
	Owner = 0x4,
	Internal = 0x5
};

enum class PlayerPermissionLevel : int8_t
{
	Visitor = 0x0,
	Member = 0x1,
	Operator = 0x2,
	Custom = 0x3
};

class PermissionsHandler
{
	CommandPermissionLevel commandPermissions;
	PlayerPermissionLevel playerPermissions;
	char pad[2];

public:
	explicit PermissionsHandler() : commandPermissions(CommandPermissionLevel::Any), playerPermissions(PlayerPermissionLevel::Visitor), pad{0,0} {}

	[[nodiscard]] CommandPermissionLevel getCommandPermissions() const
	{
		return commandPermissions;
	}
	[[nodiscard]] PlayerPermissionLevel getPlayerPermissions() const
	{
		return playerPermissions;
	}
};

static_assert(sizeof(PermissionsHandler) == sizeof(int32_t));