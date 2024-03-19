#pragma once

#include <string>

namespace bcs
{
	struct bcs_error
	{
		bcs_error(const std::string_view message) : message(message) {}

		const std::string& what() const
		{
			return message;
		}
	private:
		std::string message;
	};
}