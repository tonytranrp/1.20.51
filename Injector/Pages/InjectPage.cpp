#include <Auth/User.h>
#include <Files/Files.h>
#include <Utils/Utils.h>

#include "../Injection/Injector.h"
#include "../Process/ProcUtil.h"
#include "../Versions/VersionList.h"
#include "InjectPage.h"

#include <imgui.h>
#include <imgui_custom.h>

#include <thread>
#include <vector>

extern User* currentUser;
extern int selected_method;

static bool injecting = false;
static bool error = false;
static std::string currentStep = "Loading...";
static std::string errorMsg = "Loading...";
static size_t downloadProgress = 0;
static size_t downloadTotal = 0;
static std::vector<uint8_t> downloadCache;


InjectPage::InjectPage(size_t idx) : Page("Inject", idx) {}

void DownloadComplete(std::vector<uint8_t> dllData) {
    currentStep = "Authenticating...";
    std::string session_id = currentUser->GetSessionID();
    std::string roamingDir = Utils::GetRoamingState();
    HANDLE hFile = Files::OpenFile(roamingDir + CLIENT_AUTH_DAT);
    Files::SaveFile(hFile, (char*)session_id.c_str(), session_id.size());
    Files::CloseFile(hFile);

    DiscordRPCIntegration* RPC = new DiscordRPCIntegration();
    currentStep = "Injecting...";
    size_t procId = ProcUtil::FindProcID("Minecraft.Windows");
    if (procId == 0) {
        //Store the bytes in the cache so we don't re download them. Remember to clear the cache when switching versions!
        downloadCache = dllData;
        RPC->UpdatePresence(DiscordRPCStatus::NOT_INJECTED);
        delete RPC;
        MessageBoxA(nullptr, "Could not find Minecraft.Windows.exe, is the game open?", "Inject Error", MB_OK);
        injecting = false;
        return;
    }
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
    RPC->UpdatePresence(DiscordRPCStatus::PLAYING);
    switch (selected_method) {
    case 0: {
        ManualMap(hProc, dllData, false, false, false, false, DLL_PROCESS_ATTACH, 0);
        break;
    }
    case 1: {
        std::string dllFilePath = roamingDir + "/winsxs.dll";
        HANDLE hDllFile = Files::OpenFile(dllFilePath);
        Files::SaveFile(hDllFile, (char*)dllData.data(), dllData.size());
        Files::CloseFile(hDllFile);
        LoadLib(hProc, dllFilePath);
        break;
    }
    }
    

    CloseHandle(hProc);
    delete RPC;
    injecting = false;
}

bool DownloadProgress(uint64_t progress, uint64_t total) {
    downloadProgress = progress;
    downloadTotal = total;
    return true;
}

void InjectPage::DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {
    ImVec2 textSize = ImGui::CalcTextSize("Inject");
    ImGui::SetCursorPosX((guiSize.x / 2) - (textSize.x / 2));
    ImGui::SetCursorPosY((guiSize.y / 2) - (textSize.y / 2) + pageOffY);
    ImGui::BeginGroup();
    if (error)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", errorMsg.c_str());
    }
    if (injecting) {
        ImGui::Text("%s", currentStep.c_str());
        ImGui::Text("%lld of %lld bytes downloaded", downloadProgress, downloadTotal);
    }
    else {
        if (ImGui::ButtonAnimated("Inject")) {
            if (currentUser == nullptr)
                curPage = 0;
            else {
                DiscordRPCIntegration* RPC = new DiscordRPCIntegration();
                RPC->UpdatePresence(DiscordRPCStatus::DOWNLOADING);
                injecting = true;
                currentStep = "Downloading...";
                if (downloadCache.size() > 0) {
                    DownloadComplete(downloadCache);
                }
                else {
                    try {
                        VersionList::GetList()->GetLatest().DownloadAsync(DownloadComplete, DownloadProgress);
                    }
                    catch (std::exception& ex)
                    {
                        errorMsg = std::string("Error: ") + std::string(ex.what());
                        injecting = false;
                        error = true;
                    }
                }
            }
        }
    }
    ImGui::EndGroup();
}