#pragma once

#include "../node/function_command_node.h"

namespace bcs
{
	template<typename node_t>
	struct command_builder
	{
		template<typename... args_t>
		command_builder(args_t... args) : node(node_t(args...)) {}

		template<typename next_t>
		command_builder<node_t> then(command_builder<next_t> next)
		{
			node.add<next_t>(next.build());
			return *this;
		}

		command_builder<node_t> executes(command_function func)
		{
			node.add(function_command_node(func));
			return *this;
		}

		virtual node_t build() const
		{
			return node;
		}
	private:
		node_t node;
	};
}