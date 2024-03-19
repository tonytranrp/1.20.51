#pragma once

#include "../Signatures/SigTable.h"

#include <Data/ManagedItem.h>
#include <polyhook2/Detour/x64Detour.hpp>

#include <memory>
#include <string>

class IPatch : public ManagedItem
{
protected:
	//Patch metadata
	std::vector<SigTable>* signatures;

	//PLH
	std::unique_ptr<PLH::IHook> hook;
public:
	//Value stating if the patch's data is deleted
	bool cleaned = false;

	explicit IPatch(const std::string& name);
	~IPatch() override;
	//Not using AutoPatch? You'll have to clean up
	//your mess alone! Here's a func to let IPatch know
	//you've got it alone. Make sure to set "cleaned" to
	//true to let me know not to clean up myself.
	virtual bool manualCleanup();
	//If you are using AutoPatch, this func will be called
	//when deleting the object automatically
	bool autoCleanup();
	//When adding sigs, the first added have priority.
	//Order matters, older version sigs might work on new versions
	//but result in the wrong func or bad memory
	void addSignature(SigTable sig);
	//Scans all sigs in the order
	uintptr_t scanSigs();
	//Actually apply the hook
	virtual bool apply();
	//Use polyhook to hook the function
	bool autoPatch(void* callback, uintptr_t* original);
};
