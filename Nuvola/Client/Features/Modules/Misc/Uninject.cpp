#include "Uninject.h"
#include "../../../Patches/PatchManager.h"

#include <xorstr.hpp>

Uninject::Uninject() : Module(xorstr_("Uninject"), xorstr_("Uninjects Nuvola from the game.")) {
#ifdef _DEBUG
	this->setHotkey('U', false);
#endif
};

extern bool exiting;
void Uninject::onEnable() {
	//Uninject here
	exiting = true;
};

void Uninject::onDisable() {
	//This code is unreachable
	//The library and its memory would have been
	//freed and execution would have stopped.
}