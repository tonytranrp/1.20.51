#include "ESP.h"

#include "../../../../Utils/TargetUtils.h"
#include "../../../../Utils/RenderUtils.h"

#include "../../../Bridge/Components/MobHurtTimeComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GuiData.h"
#include "../../../Bridge/LevelRenderer.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../Rendering/GeometryBuilder.h"
#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/WorldRenderer.h"

#include "../../Theme/ThemeManager.h"

#include "../Misc/AntiBot.h"

#include "../ModuleManager.h"

#include <imgui.h>
#include <imgui_custom.h>
#include <imgui_internal.h>

#include <map>
#include <mutex>
#include <regex>

ESP::ESP() : Module("ESP", "Highlights entities through blocks.") {
	//Setting to show players
	//this->Player_NameTag = this->addItem<Setting>(xorstr_("Name Tags"), xorstr_("If players names are shown."), SettingType::TOGGLE, true, false, true);

	//Fade in & out settings
	this->fadeMin = this->addItem<Setting>("Fade Min", "How close the entity needs to be for the ESP box to fully fade out.", SettingType::SLIDER_FLOAT, 2.5f, 0.0f, 15.0f);
	this->fadeMax = this->addItem<Setting>("Fade Max", "How far the entity needs to be for the ESP box to begin to fade out.", SettingType::SLIDER_FLOAT, 5.0f, 0.0f, 15.0f);

	this->targets = this->addItem<EntityTypeListSetting>("Targets", "Which entities should or shouldn't be targeted.");
	//If teammates should be rendered differently
	this->checkTeam = this->addItem<Setting>("Render Teammates", "If teammates should be distinquished from other players.", SettingType::TOGGLE, false, false, true);
}

void ESP::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &ESP::onRenderEvent>(this);
}

void ESP::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<RenderEvent, &ESP::onRenderEvent>(this);
}

void ESP::onRenderEvent(RenderEvent& event) {
	if (!this->isEnabled()) { return; }
	if (event.getTime() != RenderTime::WORLD) { return; }

	//Try to get the font scale (assume 1.0 if we fail)
	float fontScale = 1.0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiContext* g = ImGui::GetCurrentContext();
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) { return; }
	if (!ci->isInGame()) { return; }
	GuiData* gd = ci->getGuiData();
	if (!gd) { return; }
	LocalPlayer* lp = ci->getClientPlayer();
	if (!lp) { return; }

	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();
	GeometryBuilder builder(&renderer);

	fontScale = gd->scale;

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ColorF espCol = theme[ThemeColor::ESP_DEFAULT];
	ColorF teamCol = theme[ThemeColor::ESP_TEAMMATES];
	ColorF botCol = theme[ThemeColor::ESP_BOTS];
	ColorF hurtCol = theme[ThemeColor::ESP_HURT];

	std::vector<Actor*> toShow = TargetUtils::findTargetMulti(lp,
		this->targets->getSelectedTypes(),
		-1.0f, //Negative reach = all
		true,
		false
	);

	std::shared_ptr<AntiBot> antiBot = systems->getModuleManager()->findModule<AntiBot>();

	for (Actor* actor : toShow)
	{
		ColorF color = espCol;
		bool teammate = false;

		if (checkTeam->getValueAs<bool>()) {
			std::string targetName = actor->getNameTag();
			std::string localName = lp->getNameTag();

			if (targetName.length() > 2 && localName.length() > 2) {
				targetName = std::regex_replace(targetName, std::regex("\\�r"), "");
				localName = std::regex_replace(localName, std::regex("\\�r"), "");

				if (targetName.at(2) == localName.at(2)) {
					teammate = true;
				}
			}
		}

		if (teammate)
		{
			color = teamCol;
		}
		else if (antiBot->isBot(actor))
		{
			color = botCol;
		}

		if (actor->isMob())
		{
			MobHurtTimeComponent* hurtComponent = reinterpret_cast<Mob*>(actor)->mHurtTimeComponent;
			if (hurtComponent != nullptr)
			{
				int hurtTime = hurtComponent->hurtTime;
				float hurtPercent = 0.0f;
				if (hurtTime > 0)
					hurtPercent = static_cast<float>(hurtTime) / 10.0f;

				color = color.interpolate(hurtCol, hurtPercent);
			}
		}

		float distance = lp->getRenderPos().distanceTo(actor->getRenderPos());
		auto minFadeDist = this->fadeMin->getValueAs<float>();  // Minimum distance for percent to be 0.0f
		auto maxFadeDist = this->fadeMax->getValueAs<float>();  // Maximum distance for percent to be 1.0f

		float percent;
		if (distance <= minFadeDist) {
			percent = 0.0f;
		}
		else if (distance >= maxFadeDist) {
			percent = 1.0f;
		}
		else {
			percent = (distance - minFadeDist) / (maxFadeDist - minFadeDist);
		}

		color.w *= percent;

		builder.buildAABB(actor->getRenderAABB(), color, true);
	}
}
