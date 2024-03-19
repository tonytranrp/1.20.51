#pragma once

#include "command_node.h"

#include <string>

namespace bcs
{
	struct root_command_node : public command_node
	{
		root_command_node() : command_node() {}

		bool parse(const std::string_view word, context_builder& builder) const override
		{
			return true;
		}
	};
}