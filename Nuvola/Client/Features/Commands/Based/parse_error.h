#pragma once

#include "bcs_error.h"

namespace bcs
{
	struct parse_error : public bcs_error
	{
		parse_error(const std::string& error) : bcs_error(error) {};
	};
}