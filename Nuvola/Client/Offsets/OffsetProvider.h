#pragma once

#include "../../Mem/Mem.h"
#include "OffsetTable.h"

#include <Data/ManagedItem.h>

#include <cassert>
#include <map>
#include <string>

class OffsetProvider : public ManagedItem {
	std::map<OffsetTable, int32_t> offsetResults;
public:
	explicit OffsetProvider(std::string sourceName);
	virtual std::string getSource();
	virtual void addResult(OffsetTable, int32_t);
	template<typename asmOffset_t>
	void addResult(OffsetTable offsetId, const std::string& pattern, int offset)
	{
		uintptr_t addr = Mem::FindSig(pattern.c_str());
		assert(addr != 0);

		int32_t shrunk = *reinterpret_cast<asmOffset_t*>(addr + offset);
		this->addResult(offsetId, shrunk);
	}
	virtual int32_t findFor(OffsetTable offset);
	virtual bool has(OffsetTable offset);
};