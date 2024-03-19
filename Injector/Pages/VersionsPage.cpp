#include "VersionsPage.h"

#include <imgui.h>
#include <imgui_custom.h>

#include "../Versions/VersionList.h"

VersionsPage::VersionsPage(size_t idx) : Page("Versions", idx) {
}

void VersionsPage::DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {
    try {
        ImGui::SetCursorPosY(pageOffY + (fontScale * 100));
        if (ImGui::ButtonAnimated("Refresh")) {
            VersionList::Refresh();
        }
        VersionList* vlist = VersionList::GetList();
        const std::vector<Version>& versions = vlist->GetVersions();
        bool first = true;
        if (ImGui::BeginListBox("##VERLIST")) {
            for (const Version& ver : versions) {
                //The first is the latest, and that will show up twice
                if (first) {
                    first = false;
                    continue;
                }
                ImGui::Text("%s", ver.GetTag().c_str());
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75);
                if (ImGui::Button("Inject")) {
                    //TODO: Inject older vers
                }
                ImGui::Separator();
            }
            ImGui::EndListBox();
        }
    }
    catch (std::exception& ex) {
        ImGui::Text("Failed to load versions: %s", ex.what());
    }
}