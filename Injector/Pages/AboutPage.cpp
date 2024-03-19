#include "AboutPage.h"

#include <ImGui/imgui.h>
#include <Utils/Utils.h>

#include "../Versions/VersionList.h"

AboutPage::AboutPage(size_t idx) : Page("About", idx) {

}

void AboutPage::DrawPageContent(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage) {
    ImGui::SetCursorPosY(pageOffY + (fontScale * 128));
    ImGui::Text("Nuvola Client made by " NUVOLA_AUTHORS);
    ImGui::Text("Build %s", STRING(NUVOLA_BUILD_VERSION));
    ImGui::Text("Injector Version %s", STRING(INJECTOR_BUILD_TAG));
}