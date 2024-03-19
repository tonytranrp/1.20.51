#pragma once

#include "builder/command_builder.h"
#include "node/function_command_node.h"
#include "node/literal_command_node.h"
#include "node/root_command_node.h"
#include "parse_error.h"

#include <memory>
#include <string>

namespace bcs
{
	struct command_dispatcher
	{
		command_dispatcher() : root(std::make_shared<root_command_node>()) {}

		void create(command_builder<literal_command_node> builder) const
		{
			root->add(builder.build());
		}

		std::shared_ptr<function_command_node> parse(const std::vector<std::string>& words, std::shared_ptr<command_node> currentNode, context_builder& builder) const
		{
			auto casted = std::dynamic_pointer_cast<function_command_node>(currentNode);
			if (casted != nullptr)
			{
				return casted;
			}

			for (size_t i = 0; i < currentNode->size(); i++)
			{
				auto child = currentNode->get(i);

				if (child->parse(words.empty() ? "" : words[0], builder))
				{
					auto advancedVector = words.empty() ? words : std::vector<std::string>{ words.begin() + 1, words.end() };
					return parse(advancedVector, child, builder);
				}
			}
			if (currentNode == root)
			{
				throw bcs::parse_error(std::format("Unknown command '{}'", words.empty() ? "(No command provided)" : words[0]));
			}
			else
			{
				throw bcs::parse_error(std::format("Invalid command usage (Error: Invalid token '{}')", words.empty() ? "(End of 'words', no more tokens)" : words[0]));
			}
		}

		bool execute(const std::string_view input, void* source) const
		{
			std::vector<std::string> words;

			//Split string by space
			std::string newWord;
			bool canSplit = true; //When using "" the text between counts as 1 word
			for (const auto c : input) {
				if (c == '"') {
					canSplit = !canSplit;
				}
				if (canSplit && c == ' ') {
					words.push_back(newWord);
					newWord = "";
					continue;
				}
				newWord += c;
			}
			words.push_back(newWord);

			context_builder builder;
			auto function = parse(words, root, builder);
			if (function != nullptr)
			{
				command_context context = builder.build();
				return function->invoke(context);
			}
			
			throw parse_error("Unknown internal error.");
		}
	private:
		std::shared_ptr<root_command_node> root = nullptr;
	};
}