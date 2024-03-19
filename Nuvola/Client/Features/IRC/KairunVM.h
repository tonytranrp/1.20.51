#pragma once

#include <list>
#include <memory>
#include <optional>
#include <string>

typedef int32_t register_t;

namespace KairunVM
{
	enum class OrimotoOpcode : int8_t
	{
		DIV = 0,
		ADD = 1,
		MUL = 2,
		RET = 3
	};

	struct Instruction
	{
		Instruction(OrimotoOpcode opcode, int8_t parameter);

		OrimotoOpcode instruction;
		int8_t parameter;
	};

	class Program
	{
		std::list<Instruction> instructions;
	public:
		Program();
		void append(const Instruction& instruction);
		[[nodiscard]] std::optional<register_t> execute() const;
		[[nodiscard]] std::string compile() const;

		static std::shared_ptr<Program> parse(const std::string& code);
	};

	std::optional<register_t> execute(const std::string& program);
}