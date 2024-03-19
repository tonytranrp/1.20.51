#pragma once

#include "command_node.h"

#include <string>
#include <vector>

namespace bcs
{
	struct literal_command_node : public command_node
	{
		template<typename... args_t>
		literal_command_node(std::string text, args_t... args) : command_node(), text(text), aliases({ args... }) {}

		const std::string& get() const
		{
			return text;
		}

		bool parse(const std::string_view word, context_builder& builder) const override
		{
			if (word == text)
			{
				return true;
			}
			for (const auto& alias : aliases)
			{
				if (word == alias)
				{
					return true;
				}
			}
			return false;
		}
	private:
		std::string text;
		std::vector<std::string> aliases;
	};
}