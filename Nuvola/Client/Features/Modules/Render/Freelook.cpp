#include "Freelook.h"

#include "../../../Bridge/Components/UpdatePlayerFromCameraComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/Options.h"
#include "../../../Bridge/IntOption.h"

#include "../../../Events/KeyEvent.h"

#include "../ModuleManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Freelook::Freelook() : Module("Freelook", "Allows you to look freely in the third-person camera. Works like Lunar client freelook.", VK_MENU) {

}

void Freelook::onEnable() {
	//Find the player
	const ClientInstance* client = Bridge::getClientInstance();
	LocalPlayer* player = client->getClientPlayer();
	if (player == nullptr)
	{
		this->setEnabled(false);
		return;
	}

	//Store the player's rotation for later
	lookingAngles = player->rotationComponent->Get();

	//Change the camera state in options
	if (Options* options = client->getOptions()) {
		auto* cameraState = options->getOption<IntOption, Options::OptionID::game_thridperson>();

		//Store the camera state from before
		lastCameraState = cameraState->value;
		cameraState->value = 1;
	}

	//Remove the UpdatePlayerFromCameraComponent on the camera
	auto& update_storage = player->entityContext.mEnttRegistry->storage<UpdatePlayerFromCameraComponent>();
	for(const auto& [entt, component] : update_storage.each())
	{
		this->cameras.insert(entt);
	}
	update_storage.clear();
}
void Freelook::onDisable() {
	const ClientInstance* client = Bridge::getClientInstance();
	LocalPlayer* player = client->getClientPlayer();

	//Restore the camera state
	if (const Options* options = client->getOptions()) {
		auto* cameraState = options->getOption<IntOption, Options::OptionID::game_thridperson>();

		cameraState->value = lastCameraState;
	}

	//Restore the player's rotation
	player->rotationComponent->Set(lookingAngles);

	//Restore the UpdatePlayerFromCameraComponent on all cameras that had it
	auto& update_storage = player->entityContext.mEnttRegistry->storage<UpdatePlayerFromCameraComponent>();
	std::ranges::for_each(this->cameras.begin(), this->cameras.end(), [&](auto entt)
	{
		update_storage.emplace(entt, player->getEntityId());
	});
	this->cameras.clear();
}

void Freelook::onHotkeyCheckEvent(KeyEvent& event) {
	if(event.getKey() == this->getHotkey()) {
		if(event.getAction() == KeyAction::RELEASED) {
			this->setEnabled(false);
		}
		if(event.getAction() == KeyAction::PRESSED) {
			this->setEnabled(true);
		}
	}
};