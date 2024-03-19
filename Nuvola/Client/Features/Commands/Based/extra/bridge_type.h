#pragma once

#include <sstream>

struct Player;
namespace bcs
{
	template<typename bridge_t>
	struct bridge_type
	{
		using ptr = bridge_t*;

		bridge_t get() const { return bridge_val; }
		ptr bridge_val = nullptr;
	};

	template<>
	struct bridge_type<Player>
	{
		operator bool() const { return player != nullptr; }

		Player* get() const { return player; }
		Player* player = nullptr;
	};

	std::istream& operator>>(std::istream& stream, bridge_type<Player>& obj);
}