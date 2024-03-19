#define USE_UWP
#include <Auth/User.h>
#include "Client/GUI/ScreenManager.h"
#include <Utils/Exception.h>
#include <Utils/Utils.h>

#include <thread>
#include <string>
#include <vector>
#include <mutex>

#include "Client/Features/Config/ConfigManager.h"
#include "Client/Features/Keymap/KeymapManager.h"
#include "Client/Features/Modules/Module.h"
#include "Client/Features/Modules/ModuleManager.h"
#include "Client/Features/Modules/Render/Notifications.h"
#include "Client/Features/Theme/ThemeManager.h"
#include "Client/Features/Commands/CommandManager.h"

#include "Client/GUI/EnchantWeightScreen.h"
#include "Client/Patches/PatchManager.h"
#include "Client/Preferences/Preferences.h"
#include "Client/SystemManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::unique_ptr<User> currentUser = nullptr;
bool exiting = false;
bool readyToUninject = false;
std::mutex initMutex;

DWORD WINAPI uninjectThread(LPVOID lpParameter)
{
	while (!readyToUninject)
	{
		Sleep(100);
	}

	Sleep(1000);

	//Docs say dont get module with GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT cuz it could prematurely unmap
	//To that I say lol dont care lmao
	HMODULE hMod = nullptr;
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, "Nuvola.dll", &hMod);

	//Free library
	FreeLibraryAndExitThread(hMod, 1);
	return 1;
}

#include <winrt/windows.ui.h>
#include <winrt/windows.ui.notifications.h>
#include <winrt/windows.ui.notifications.management.h>
#include <winrt/windows.data.xml.dom.h>

using namespace winrt::Windows::UI::Notifications;
using namespace winrt::Windows::Data::Xml::Dom;
DWORD WINAPI init(LPVOID lpParameter) {
    auto start = std::chrono::steady_clock::now();

    ToastNotification toastNotif = ToastNotification(ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastImageAndText02));
    // Set the title
    IXmlNodeList txtElmnt = toastNotif.Content().GetElementsByTagName(L"text");
    txtElmnt.Item(0).InnerText(winrt::to_hstring("Nuvola"));
    // Set the message
    txtElmnt.Item(1).InnerText(winrt::to_hstring("Nuvola has been injected, please wait..."));
    // Show
    ToastNotificationManager::CreateToastNotifier().Show(toastNotif);

	//Authenticate
	//UWP only allows requests to be made to non-local addresses for some reason
#if defined _DEBUG or defined AUTH_DISABLED
	currentUser = std::make_unique<User>("NULL");
#else
	std::string roamingDir = Utils::GetRoamingState();
	HANDLE hFile = Files::OpenFile(roamingDir + CLIENT_AUTH_DAT);
	if (hFile != NULL) {
		std::pair<char*, size_t> content = Files::ReadFile(hFile);
		Files::CloseFile(hFile);
		assert(Files::DeleteFile(roamingDir + CLIENT_AUTH_DAT));
		std::string session_id(content.first, content.second);
		if(!session_id.empty())
			currentUser = std::make_unique<User>(session_id);
	}
#endif
#ifndef _DEBUG
	SetUnhandledExceptionFilter(Exception::exceptionHandler);
#endif

	initMutex.lock();
	if (systems == nullptr)
	{
		systems = std::make_shared<SystemManager>();
	}
    systems->installAllSystems();

	//Apply all patches
	const auto patches = systems->getPatchManager();
	//Get Module Manager instance;
	const auto moduleManager = systems->getModuleManager();
	//Get all modules
	const auto allMods = moduleManager->getAllModules();
	//Get Config Manager instance
	const auto configManager = systems->getConfigManager();
	//Load themes
	const auto themeManager = systems->getThemeManager();

	/* Load all keybinds */
	const auto keyMgr = systems->getKeymapManager();
	const auto& keymap = keyMgr->get();
	if (keymap != nullptr)
	{
		keymap->applyToClient();
	}

    assert(systems->allInstalled());

    //Do any remaining initialization
    themeManager->reloadAllThemes();
    configManager->reloadAllFromDisk();

	const auto notifs = moduleManager->findModule<Notifications>();
	if (notifs != nullptr)
	{
        auto end = std::chrono::steady_clock::now();
		notifs->notify(std::format("Initialization completed in {}ms! 'Insert' is the default key for the GUI.",
                       std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()));
	}
	initMutex.unlock();

	//Nuvola tick caller
	while(!exiting) {
		for(const auto& mod : allMods) {
			mod->onNuvolaTick();
		}
		Sleep(10); //Allow the os to take some time on that core lol
	}

	//Cleanup
	patches->removeAll();
	
	readyToUninject = true;
	return 0;
}

auto __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) -> bool {
	if(fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, &init, 0, 0, 0);
		CreateThread(0, 0, &uninjectThread, 0, 0, 0);
	}
    return true;  // Successful DLL_PROCESS_ATTACH
}