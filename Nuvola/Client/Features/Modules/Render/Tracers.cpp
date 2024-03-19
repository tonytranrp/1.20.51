#include "Tracers.h"

#include "../../../../Utils/TargetUtils.h"
#include "../../../Bridge/Components/MobHurtTimeComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/Level.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RenderEvent.h"
#include "../../../Rendering/RenderController.h"
#include "../../Theme/ThemeManager.h"
#include "../Misc/AntiBot.h"
#include "../ModuleManager.h"

#include <glm/ext/matrix_projection.hpp>
#include <imgui.h>
#include <regex>

Tracers::Tracers() : Module("Tracers", "Draws lines to target entities.")
{
	this->sourcePos = this->addItem<ModeSetting<tracerMode_t>>("Source Position", "The location where the tracer lines should originate from.");
	this->targets = this->addItem<EntityTypeListSetting>("Targets", "Which entities should or shouldn't be targeted.");
	this->checkTeam = this->addItem<Setting>("Render Teammates", "If teammates should be ignored.", SettingType::TOGGLE, false, false, true);
}

void Tracers::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RenderEvent, &Tracers::onRenderEvent>(this);
}

void Tracers::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<RenderEvent, &Tracers::onRenderEvent>(this);
}

void Tracers::onRenderEvent(RenderEvent& event)
{
	if (event.getTime() != RenderTime::WORLD) return;

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) { return; }
	if (!ci->isInGame()) { return; }
	LocalPlayer* lp = ci->getClientPlayer();
	if (!lp) { return; }

	Theme theme = systems->getThemeManager()->getActiveTheme();
	ColorF tracerCol = theme[ThemeColor::TRACERS_DEFAULT];
	ColorF teamCol = theme[ThemeColor::TRACERS_TEAMMATES];
	ColorF botCol = theme[ThemeColor::TRACERS_BOTS];
	ColorF hurtCol = theme[ThemeColor::TRACERS_HURT];

	tracerMode_t mode = this->sourcePos->getCurrentMode();
	Vector2<float> displaySize = ImGui::GetIO().DisplaySize;
	Vector2<float> sourceLoc2d;
	Vector3<float> sourceLoc3d;

	switch (mode)
	{
	case tracerMode_t::Top:
		sourceLoc2d = { displaySize.x / 2.0f, 0.0f };
		sourceLoc3d = lp->getBlockPos() + Vector3<int>( 0, 3, 0 );
		break;
	case tracerMode_t::Center:
	{
		sourceLoc2d = {displaySize.x / 2.0f, displaySize.y / 2.0f};
		sourceLoc3d = lp->level->mHitResult.raycastDirection + lp->getRenderPos();
		break;
	}
	case tracerMode_t::Bottom:
		sourceLoc2d = { displaySize.x / 2.0f, displaySize.y };
		sourceLoc3d = lp->getBlockPos();
		break;
	}

	std::vector<Actor*> toShow = TargetUtils::findTargetMulti(lp,
		this->targets->getSelectedTypes(),
		-1.0f, //Negative reach = all
		true,
		this->checkTeam->getValueAs<bool>()
	);
	if (toShow.empty()) return;

	//Do cool 2d 3d mixing thing for world renderer
	RenderController* controller = RenderController::getInstance();
	WorldRenderer& renderer = controller->getWorldRenderer();

	std::shared_ptr<AntiBot> antiBot = systems->getModuleManager()->findModule<AntiBot>();
	for (Actor* actor : toShow)
	{
		ColorF color = tracerCol;
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

		renderer.begin(VertexCommand::BEGIN_LINE);
		renderer.vertex(sourceLoc2d, color);
		renderer.vertex(actor->getRenderAABB().closestPoint(sourceLoc3d), color);
		renderer.end(VertexCommand::END_LINE, color);
	}
}
