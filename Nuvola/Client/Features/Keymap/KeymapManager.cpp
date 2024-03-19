#include "KeymapManager.h"

#include <Files/Files.h>

#include <filesystem>

KeymapManager::KeymapManager() : System<Manager<Keymap>>("Keymap")
{
}

void KeymapManager::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
    System::install(ownerPtr);
}

std::shared_ptr<Keymap> KeymapManager::get()
{
	if (this->items.empty())
	{
		std::string keymapFile = Files::GetUniformDir() + "\\keymap.json";
		auto file = Files::OpenFile(keymapFile);
		if (file == nullptr)
		{
			return nullptr;
		}

		std::pair<char*, size_t> dataB = Files::ReadFile(file);
		std::string dataStr = std::string(dataB.first, dataB.second);

		std::shared_ptr<Keymap> keymap = this->addItem<Keymap>();
		keymap->loadJson(dataStr);
		Files::CloseFile(file);
	}
	
	return this->getItem(0);
}

void KeymapManager::save(const std::shared_ptr<Keymap>& keymap)
{
	std::string keymapFile = Files::GetUniformDir() + "\\keymap.json";
	auto file = Files::OpenFile(keymapFile);

	const nlohmann::json& json = keymap->saveJson();
	std::string configJson = json.dump(1, '\t');
	Files::SaveFile(file, configJson);
	Files::CloseFile(file);
}
