#include "SettingsPage.h"

#include <imgui.h>
#include <imgui_custom.h>

constexpr const char* inject_methods[] = { "Manual Map", "LoadLibrary" };
int selected_method = 0;

SettingsPage::SettingsPage(size_t idx) : Page("Settings", idx) {}

void SettingsPage::DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {
    ImVec2 textSize = ImGui::CalcTextSize("Injection Method");
    float leWid = (guiSize.x / 2) - (textSize.x / 2);
    ImGui::SetCursorPosX(maxWid + (leWid/8));
    ImGui::SetCursorPosY((guiSize.y / 3) - (textSize.y / 2) + pageOffY);

    ImGui::SetNextItemWidth(leWid);
    if (ImGui::BeginComboAnimated("Injection Method", inject_methods[selected_method])) {
        ImGui::SetWindowFontScale(fontScale);
        int idx = 0;
        for (auto& enumStr : inject_methods) {
            ImGui::PushID(&enumStr);
            bool selected = selected_method == idx;
            ImGui::Selectable(inject_methods[idx], &selected);
            if (selected) {
                selected_method = idx;
            }
            ImGui::PopID();
            idx++;
        }
        ImGui::EndCombo();
    }
#ifdef SSPROOF
    if (selected_method != 0) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
        ImGui::TextWrapped("Manual Map injection is the only screenshare proof injection method! Use other methods with caution.");
        ImGui::PopStyleColor();
    }
#endif
}