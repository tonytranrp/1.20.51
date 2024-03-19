#include "Page.h"

#include <Fonts/FontRegistry.h>

#include <imgui.h>
#include <imgui_custom.h>

#include <map>

static std::map<void*, float> animProgs;

Page::Page(std::string pageName, size_t idx)
{
	this->pageName = pageName;
	this->idx = idx;
}

const std::string& Page::GetPageName()
{
	return pageName;
}

bool Page::DrawSelection(Vector2<float> guiSize, float maxWid, float fontScale, float darkTheme, int64_t& curPage)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushFont(FontRegistry::getFont(FontRegistry::FontID::THICC));
	ImVec2 textSize = ImGui::CalcTextSize(this->GetPageName().c_str());
	textSize.x = maxWid;
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
	//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(darkTheme, darkTheme, darkTheme, animProgs[this] * 0.1));
	ImGui::BeginChild(this->GetPageName().c_str(), textSize);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5 + (ImGui::easeOutQuart(animProgs[this]) * 20));
	ImGui::Text("%s", this->GetPageName().c_str());
	ImGui::EndChild();
	ImGui::PopStyleColor();
	//ImGui::PopStyleVar();
	ImGui::PopFont();

	if (idx != curPage) {
		if (ImGui::IsItemClicked()) {
			curPage = idx;
			return true;
		}
	}
	if (ImGui::IsItemHovered()) {
		animProgs[this] += io.DeltaTime * 4;
		if (animProgs[this] >= 1) {
			animProgs[this] = 1;
		}
	}
	else {
		animProgs[this] -= io.DeltaTime * 8;
		if (animProgs[this] <= 0) {
			animProgs[this] = 0;
		}
	}
	return false;
}

float Page::GetSelectionHeight(float fontScale)
{
	ImVec2 textSize = ImGui::CalcTextSize(this->GetPageName().c_str());
	return textSize.y;
}

void Page::DrawPage(Vector2<float> guiSize, float maxWid, float fontScale, float pageOffY, int64_t& curPage)
{
	ImGui::SetCursorPosX(maxWid + 20);
	ImGui::SetCursorPosY(pageOffY + 20);
	ImGui::BeginGroup();

	this->DrawPageContent(guiSize, maxWid, fontScale, pageOffY, curPage);

	ImGui::EndGroup();
}
