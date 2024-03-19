#pragma once

#include <sstream>
#include <string>

namespace bcs
{
	namespace internal
	{
		template<typename type_t>
		bool fromString(const std::string text, type_t& val)
		{
			std::istringstream ss(text);
			ss >> val;
			return !ss.fail();
		}

		template<typename type_t>
		struct type_string
		{
			static const char* str()
			{
				return typeid(type_t).name();
			};
		};

		template<>
		struct type_string<std::string>
		{
			static const char* str()
			{
				return "string";
			}
		};
	}
}