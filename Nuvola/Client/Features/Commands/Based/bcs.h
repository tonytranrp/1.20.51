#pragma once

#include "builder/command_builder.h"
#include "node/argument_node.h"
#include "command_dispatcher.h"

/*
* bcs - Based Command System
* I like using namespaces for shit like this :)
*/
namespace bcs
{
	/* Aliases */
	using literal_node = literal_command_node;
	using literal = command_builder<literal_node>;
	template<typename type_t, typename allow_relative_v = std::false_type>
	using argument = command_builder<argument_node<type_t>>;
}