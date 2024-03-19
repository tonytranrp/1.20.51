#pragma once

#include "../../System.h"

#include <Data/Manager.h>

#include "Keymap.h"

#include <string>
#include <vector>

class KeymapManager : public ::System<Manager<Keymap>>
{
public:
	KeymapManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

	std::shared_ptr<Keymap> get();
	void save(const std::shared_ptr<Keymap>&);
};