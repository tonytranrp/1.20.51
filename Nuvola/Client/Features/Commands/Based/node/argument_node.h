#pragma once

#include "../bcs_internal.h"
#include "../parse_error.h"
#include "command_node.h"

#include <string>

namespace bcs
{
	template<typename type_t>
	struct argument_node : public command_node
	{
		argument_node(std::string name) : command_node(), name(name) {};

		bool parse(const std::string_view word, context_builder& builder) const override
		{
			if (word.empty())
				return false;
			try
			{
				type_t val;
				if (internal::fromString<type_t>(std::string(word), val))
				{
					builder.emplace(name, val);
					return true;
				}
				return false;
			}
			catch (...)
			{
				//Welp the arg wasnt emplaced so too bad
			}
			//throw bcs::parse_error(std::format("Couldn't parse '{}', expected '{}'", word, internal::type_string<type_t>::str()));
			return false;
		}
	private:
		std::string name;
	};
}