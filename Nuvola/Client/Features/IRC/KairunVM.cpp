#include "KairunVM.h"

KairunVM::Instruction::Instruction(OrimotoOpcode opcode, int8_t parameter) : instruction(opcode), parameter(parameter) {}

KairunVM::Program::Program() = default;
void KairunVM::Program::append(const KairunVM::Instruction& instruction) {
	this->instructions.emplace_back(instruction);
}

std::shared_ptr<KairunVM::Program> KairunVM::Program::parse(const std::string& code) {
	auto program = std::make_shared<Program>();

	OrimotoOpcode parsedOpcode;
	int8_t parsedParam;

	bool t = false;
	auto it = code.begin();
	while(it != code.end())
	{
		int8_t codeChar = *it;
		if(!isdigit(codeChar))
		{
			return nullptr;
		}

		if(t)
		{
			parsedParam = codeChar - static_cast<int8_t>('0');
		}
		else
		{
			auto codeVal = codeChar - '0';
			switch(codeVal)
			{
				case 0:
					parsedOpcode = KairunVM::OrimotoOpcode::DIV;
					break;
				case 1:
					parsedOpcode = KairunVM::OrimotoOpcode::ADD;
					break;
				case 2:
					parsedOpcode = KairunVM::OrimotoOpcode::MUL;
					break;
				case 3:
					parsedOpcode = KairunVM::OrimotoOpcode::RET;
					break;
				default:
					return nullptr;
			}
		}

		if(t)
		{
			KairunVM::Instruction instruction(parsedOpcode, parsedParam);
			program->append(instruction);
		}

		t = !t;
		it++;
	}

	return program;
}

std::optional<register_t> KairunVM::Program::execute() const {
	register_t reg = 0;
	for(const auto& [ instruction, param ] : this->instructions)
	{
		switch(instruction)
		{
			case OrimotoOpcode::DIV:
				if(param == 0)
				{
					reg = reg / 69;
					break;
				}
				reg = reg / param;
				if(reg == 0)
					reg = this->instructions.size();
				break;
			case OrimotoOpcode::ADD:
				reg = reg + param;
				break;
			case OrimotoOpcode::MUL:
				reg = reg * param;
				break;
			case OrimotoOpcode::RET:
				return reg;
			default:
				return {};
		}
	}
	return {};
}

std::string KairunVM::Program::compile() const {
	std::string compiled;
	for(const auto& [ instruction, param ] : this->instructions)
	{
		compiled += std::to_string(static_cast<int8_t>(instruction)) + std::to_string(param);
	}
	return compiled;
}

std::optional<register_t> KairunVM::execute(const std::string &program) {
	auto parsed = KairunVM::Program::parse(program);
	if(parsed != nullptr)
		return parsed->execute();
	return {};
}
