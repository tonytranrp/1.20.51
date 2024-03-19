#pragma once

#include "../System.h"

#include "IPatch.h"

#include <Data/Manager.h>

#include <memory>
#include <vector>

class PatchManager : public ::System<Manager<IPatch>>
{
public:
	PatchManager();
    void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;
	template<typename T>
	void applyPatch()
	{
		auto toApply = this->addItem<T>();
		bool success = toApply->apply();
		assert(success);
	}
	bool removeAll();
	bool removePatch(std::shared_ptr<IPatch> toRemove);
};