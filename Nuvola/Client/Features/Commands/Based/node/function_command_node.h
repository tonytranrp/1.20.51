#pragma once

#include "../context/command_context.h"
#include "command_node.h"

#include <functional>
#include <string>

namespace bcs
{
	struct function_command_node : public command_node
	{
		function_command_node(command_function function) : command_node(), function(function) {}

		bool parse(const std::string_view word, context_builder& builder) const override
		{
			return word.empty();
		}
		bool invoke(command_function_args args) const
		{
			return function(args);
		}
	private:
		command_function function;
	};
}