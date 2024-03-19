#pragma once

#include "../context/context_builder.h"

#include <memory>
#include <string>
#include <vector>

namespace bcs
{
	struct command_node
	{
		command_node() {}
		command_node(const command_node& other)
		{
			children = other.children;
		}

		template<typename node_t>
		void add(const node_t& child)
		{
			children.emplace_back(std::make_shared<node_t>(child));
		}

		std::shared_ptr<command_node> get(size_t index) const
		{
			return children.at(index);
		}
		std::shared_ptr<command_node> operator[](size_t index) const
		{
			return get(index);
		}
		size_t size() const
		{
			return children.size();
		}

		virtual bool parse(const std::string_view word, context_builder& builder) const = 0;
	private:
		std::vector<std::shared_ptr<command_node>> children;
	};
}