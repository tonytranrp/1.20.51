#include <Auth/User.h>
#include <Files/Files.h>
#include <Registry/Registry.h>

#include "AccountPage.h"

#include <imgui.h>
#include <imgui_custom.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern User* currentUser;

AccountPage::AccountPage(size_t idx) : Page("Account", idx) {}

void AccountPage::DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {
	ImGui::SetCursorPosY(pageOffY + (fontScale * 128));
	if (currentUser == nullptr) {
		//No account logged in
		ImGui::Text("No account - please log in!");
		if (ImGui::ButtonAnimated("Login")) {
			try {
				char nullC[1] = { 0 };
				Registry::WriteString(HKEY_CLASSES_ROOT, "injector", "URL Protocol", nullC);
				HMODULE exeHandle = GetModuleHandleA(NULL);
				char buf[MAX_PATH];
				GetModuleFileNameA(exeHandle, buf, MAX_PATH);
				std::string injectorLoc = std::string(buf);
				std::string start_cmd = "\"" + injectorLoc + "\"" + " \"%1\"";
				Registry::WriteString(HKEY_CLASSES_ROOT, "injector\\shell\\open\\command", "(Default)", start_cmd.c_str());
				system("start " "https://" AUTH_URL);
			}
			catch (std::exception& ex) {
				std::string err(ex.what());
				err += "\nTry running as administrator";
				err += "\n(It is necessary to write to the registry to create the auth protocol)";
				MessageBoxA(nullptr, err.c_str(), "Registry Error", MB_OK);
				std::string currentExe = Files::GetExecPath();
				ShellExecuteA(0, "runas", currentExe.c_str(), 0, 0, SW_SHOW);
				exit(0);
			}
		}
	}
	else {
		//Account is logged in
		ImGui::Text("Username: %s", currentUser->GetUsername().c_str());
	}
}