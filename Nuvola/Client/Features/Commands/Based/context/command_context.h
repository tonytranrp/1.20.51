#pragma once

#include <any>
#include <map>
#include <set>
#include <string>

namespace bcs
{
	struct command_context
	{
		using arguments_t = std::map<std::string, std::any>;
		using relativity_t = std::set<std::string>;

		command_context(void* source, arguments_t arguments) : source(source), arguments(arguments) {}
		void* get_source() const
		{
			return source;
		}
		template<typename type_t>
		type_t get(std::string name) const
		{
			try
			{
				if (arguments.contains(name))
				{
					auto& val = arguments.at(name);
					return std::any_cast<type_t>(val);
				}
				//The argument doesnt exist... check your command!!!!!
				__debugbreak();
			}
			catch (...)
			{
				__debugbreak();
				//Likely bad any_cast! This shouldnt happen, check your command!
			}
			//Terminating bc this should never happen!!
			//Plus there is nothing to possibly return either
			__debugbreak();
			abort();
		}
	private:
		void* source = nullptr;
		arguments_t arguments;
	};

	using command_function_args = command_context;
	using command_function = std::function<bool(command_function_args)>;
}