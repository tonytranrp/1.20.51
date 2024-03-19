#include "Notifications.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GuiData.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RenderEvent.h"
#include "../../../Features/Theme/ThemeManager.h"

#include <Fonts/FontRegistry.h>
#include <Graphics/imfx.h>

#include <imgui.h>

#include <deque>

Notifications::Notifications() : Module("Notifications", "Shows notifications for when things happen in the client.")
{
	this->setEnabled(true);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &Notifications::onRenderEvent>(this);
}

struct notif_t
{
	std::string message;
	std::string title = "Nuvola";
	float time = 0.0f;
	float maxTime = 0.0f;

	float enterProgress = 0.0f;
};

float easeOutExpo(float x) {
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}
float easeInOutQuart(float x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}
float easeInCubic(float x) {
	return x * x * x;
}
float easeOutCubic(float x) {
	return 1 - pow(1 - x, 3);
}

static std::deque<notif_t> notifications;
void Notifications::notify(std::string message, std::string title, float time)
{
	if(this->isEnabled())
		notifications.emplace_back(message, title, time, time);
}

void Notifications::onRenderEvent(RenderEvent& event)
{
	if (event.getTime() != RenderTime::MENU) return;

	ClientInstance* ci = Bridge::getClientInstance();
	float fontScale = 0.0f;
	if (ci != nullptr) {
		GuiData* gd = ci->getGuiData();
		if (gd != nullptr) {
			fontScale = gd->scale;
		}
	}

	float maxWidth = 100.0f * fontScale;
	float padding = 4.0f * fontScale;
	float lastHeight = 0.0f + padding;
	ImGuiIO& io = ImGui::GetIO();

	ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
	if (uiFont == nullptr)
		return;
	ImFont* uiFontBold = FontRegistry::getFont(FontRegistry::FontID::THICC);
	if (uiFontBold == nullptr)
		return;

	Theme theme = systems->getThemeManager()->getActiveTheme();

	int index = 0;
	for (auto& notif : notifications)
	{
		notif.time -= io.DeltaTime;
		if (notif.time > 0.0f)
		{
			if (notif.enterProgress < 1.0f)
			{
				notif.enterProgress += io.DeltaTime * 2;
			}
			else
			{
				notif.enterProgress = 1.0f;
			}
		}
		else
		{
			notif.time = 0.0f;
			if (notif.enterProgress >= 0.0f)
			{
				notif.enterProgress -= io.DeltaTime * 2;
			}
			else
			{
				notif.enterProgress = 0.0f;
				notifications.erase(notifications.begin() + index);
				break;
			}
		}
		index++;
	}

	index = 0;
	for (auto& notif : notifications)
	{
		Vector2<float> titleSize = uiFontBold->CalcTextSizeA(8.0f * fontScale, FLT_MAX, maxWidth, notif.title.c_str());
		Vector2<float> messageSize = uiFont->CalcTextSizeA(8.0f * fontScale, FLT_MAX, maxWidth, notif.message.c_str());
		Vector2<float> notifSize = {
			maxWidth + (padding * 2),
			titleSize.y + messageSize.y + (padding * 2)
		};

		Vector2<float> notifPos = {
			io.DisplaySize.x - ((notifSize.x + padding) * ImGui::easeInOutCubic(notif.enterProgress)),
			io.DisplaySize.y - ((notifSize.y + padding) + lastHeight)
		};

		Vector4<float> clipRect(notifPos, notifPos + notifSize);

		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f * fontScale);
		
		ImFX::Begin(drawList);
		ImFX::AddShadow(4.0f * fontScale, theme[ThemeColor::NOTIFICATION_SHADOW], clipRect);
		ImFX::AddBlur(8.0f, clipRect);
		ImFX::AddColor(theme[ThemeColor::NOTIFICATION_COLOR], clipRect);
		ImFX::End();
		ImGui::PopStyleVar();

		drawList->AddText(uiFontBold, 8.0f * fontScale, notifPos + padding, theme[ThemeColor::NOTIFICATION_TEXT_BOLD].asInt(), notif.title.c_str(), 0, maxWidth);
		drawList->AddText(uiFont, 8.0f * fontScale, Vector2<float>(notifPos.x + padding, notifPos.y + padding + titleSize.y), theme[ThemeColor::NOTIFICATION_TEXT].asInt(), notif.message.c_str(), 0, maxWidth);

		Vector1<float> loadingBarStart(notifPos.x + (padding * 2));
		Vector1<float> loadingBarEnd((notifPos.x + notifSize.x) - (padding * 2));
		Vector1<float> loadingBarMidpoint = loadingBarStart.interpolate(loadingBarEnd, 0.5f);

		Vector2<float> loadingBarMin(loadingBarMidpoint.interpolate(loadingBarStart, (notif.time / notif.maxTime)).x, (notifPos.y + notifSize.y) - (padding / 2));
		Vector2<float> loadingBarMax(loadingBarMidpoint.interpolate(loadingBarEnd, (notif.time / notif.maxTime)).x, notifPos.y + notifSize.y);

		if (notif.time > 0.0f)
		{
			drawList->AddRectFilled(
				loadingBarMin,
				loadingBarMax,
				theme[ThemeColor::NOTIFICATION_TEXT].asInt(),
				3.0f * fontScale
			);
		}

		lastHeight = std::lerp(lastHeight, lastHeight + notifSize.y + padding, ImGui::easeInOutCubic(notif.enterProgress));
		index++;
	}
}