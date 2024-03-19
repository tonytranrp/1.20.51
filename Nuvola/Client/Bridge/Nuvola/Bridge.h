#pragma once

#include "../../../Mem/Mem.h"
#include "../EntityId.h"

#include <entt/entity/registry.hpp>
#include <Zydis/Zydis.h>

#include <iomanip>
#include <iostream>
#include <sstream>

class ClientInstance;
namespace Bridge
{
	void setClientInstance(ClientInstance* instance);
	ClientInstance* getClientInstance();

	/*template<typename component_t>
	std::string assureForComponent()
	{
		uint32_t hash = entt::type_hash<component_t>::value();
		auto* bytes = (uint8_t*)&hash;

		std::stringstream ss;
		ss << "ba"; //mov (int32_t) instruction

		//Add the hash with this
		for (int i = 0; i < sizeof(uint32_t); i++)
		{
			ss << " " << std::setw(2) << std::setfill('0') << std::hex << (0xFF & bytes[i]);
		}

		return ss.str();
	}

	template<typename component_t>
	void* resolveAssureFunc()
	{
		//This assure sig finds any reference to the hash
		std::string assureSig = assureForComponent<component_t>();
		uintptr_t resultAddr = Mem::FindSig(assureSig.c_str());
		assert(resultAddr != 0);

		//Search for the next call
		ZydisDisassembledInstruction instruction;
		do
		{
			auto status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, resultAddr, reinterpret_cast<const void *>(resultAddr), 16, &instruction);
			assert(ZYAN_SUCCESS(status));

			if(instruction.info.opcode == 0xE8 || instruction.info.opcode == 0xCC)
				break;

			resultAddr += instruction.info.length;
		}
		while(true); //While the instruction is not a call

		//If the opcode is not a call (E8) then the search failed
		assert(instruction.info.opcode == 0xE8);

		//Get the assure func addr from the immediate value
		uint64_t assureAddr = instruction.runtime_address + instruction.info.length + instruction.operands[0].imm.value.u;

		return reinterpret_cast<void*>(assureAddr);
	}*/

	template<typename component_t>
	auto& assure_for(entt::basic_registry<EntityId>* registry)
	{
		return registry->storage<component_t>();
		/*using assure_t = entt::basic_storage<component_t, EntityId>* (__fastcall *)(const entt::basic_registry<EntityId>*, uint32_t);
		static auto assureFunc = reinterpret_cast<assure_t>(resolveAssureFunc<component_t>());

		return assureFunc(registry, entt::type_hash<component_t>::value());*/
	}
}