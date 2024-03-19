#pragma once

#include "command_context.h"

#include <any>
#include <map>
#include <set>
#include <string>

namespace bcs
{
	struct context_builder
	{
		using arguments_t = std::map<std::string, std::any>;

		context_builder() = default;

		template<typename type_t>
		void emplace(std::string name, type_t val)
		{
			arguments.emplace(name, val);
		}

		command_context build() const
		{
			return command_context(source, arguments);
		}
	private:
		void* source = nullptr;
		arguments_t arguments;
	};
}