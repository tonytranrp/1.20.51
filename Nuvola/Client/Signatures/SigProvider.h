#pragma once

#include "SigTable.h"

#include <Data/ManagedItem.h>

#include <string>
#include <map>

class SigProvider : public ManagedItem {
	std::map<SigTable, void*> sigResults;
public:
	explicit SigProvider(const std::string& sourceName);
	std::string getSource();
	void addResult(SigTable, void*);
	void addResult(SigTable, const std::string&, int offset = 0);
	void* findFor(SigTable sig);
};