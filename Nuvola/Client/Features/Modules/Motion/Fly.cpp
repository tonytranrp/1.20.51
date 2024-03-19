#include "Fly.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/KeyEvent.h"
#include "../../../Events/LevelTickEvent.h"

#include "imgui.h"
#include "imgui_custom.h"

Fly::Fly() : Module("Fly", "Volare tra le Nuvole! (Soar in the clouds!)") {
	this->mode = this->addItem<ModeSetting<flyMode_t>>("Mode", "The type of fly mode.");
	this->speed = this->addItem<Setting>("Speed", "The speed at which you will fly.", SettingType::SLIDER_FLOAT, 1.0f, 0.0f, 20.0f);
    this->speedy = this->mode->getManagerFor(flyMode_t::Motion)->addItem<Setting>("Y", "Y speed", SettingType::SLIDER_FLOAT, 1.0f, 0.0f, 20.0f);
	this->cutMotion = this->mode->getManagerFor(flyMode_t::Jetpack)->addItem<Setting>("Cut motion", "Cancels all velocity when the module is disabled.", SettingType::TOGGLE, false, false, true);
}

void Fly::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Fly::onLevelTickEvent>(this);

    ClientInstance* client = Bridge::getClientInstance();
    
    LocalPlayer* player = client->getClientPlayer();
    if (player == nullptr) return this->setEnabled(false);

    lastState = player->isFlying();
    canWrite = true;
}
void Fly::onDisable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Fly::onLevelTickEvent>(this);

	ClientInstance* client = Bridge::getClientInstance();
	
    LocalPlayer* player = client->getClientPlayer();
    if (player == nullptr) return;

	if (std::any_cast<bool>(cutMotion->getValue())) {
		player->stateVectorComponent->velocity *= 0;
	}

    if (this->mode->getCurrentMode() == flyMode_t::Vanilla)
    {
        player->setFlying(lastState);
		player->setFlightSpeed(0.05f);
    }
}

void Fly::onTick()
{

}

Vector2<float> Fly::getMotion(float playerYaw, float speed)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return Vector2<float>(0.0f, 0.0f);
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return Vector2<float>(0.0f, 0.0f);
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return Vector2<float>(0.0f, 0.0f);
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return Vector2<float>(0.0f, 0.0f);
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return Vector2<float>(0.0f, 0.0f);

    bool w = isKeyDown("key.forward");
    bool a = isKeyDown("key.left");
    bool s = isKeyDown("key.back");
    bool d = isKeyDown("key.right");

    if (w)
    {
        if (!a && !d)
            playerYaw += 90.0f;
        if (a)
            playerYaw += 45.0f;
        else if (d)
            playerYaw += 135.0f;
    }
    else if (s)
    {
        if (!a && !d)
            playerYaw -= 90.0f;
        if (a)
            playerYaw -= 45.0f;
        else if (d)
            playerYaw -= 135.0f;
    }
    else
    {
        if (!a && d) playerYaw += 180.0f;
    }

    float calcYaw = playerYaw * 0.01745329251f; //0.01745329251f = PI / 180
    return { cos(calcYaw) * speed / 10.0f,sin(calcYaw) * speed / 10.0f };

}

bool Fly::isKeyDown(std::string key) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return false;
    ClientInputHandler* cih = ci->mClientInputHandler;
    if (!cih) return false;
    ClientInputMappingFactory* cimf = cih->factory;
    if (!cimf) return false;
    KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
    if (!kms) return false;
    
    return isKeyDown(kms->GetMCKey(std::move(key)));
}

bool Fly::isKeyDown(int key) {
    return ImGui::IsKeyDown(ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key));
}

void Fly::onHotkeyCheckEvent(KeyEvent& event) {
    if (this->mode->getCurrentMode() == flyMode_t::Jetpack) {
        if (event.getKey() == this->getHotkey()) {
            if (event.getAction() == KeyAction::RELEASED) {

                this->setEnabled(false);

            }
            if (event.getAction() == KeyAction::PRESSED) {
                this->setEnabled(true);
            }
        }
    } else {
        if(event.getAction() == KeyAction::PRESSED) {
            if(this->getHotkey() == event.getKey()) {
                this->toggle();
            }
        }
    }
}

void Fly::onLevelTickEvent(LevelTickEvent& event) {
    ClientInstance *client = Bridge::getClientInstance();
    LocalPlayer *player = client->getClientPlayer();
    if (player == nullptr) return this->setEnabled(false);
    if (this->mode->getCurrentMode() == flyMode_t::Jetpack) {
        //Get the jetpack speed from the setting
        float flySpeed = std::any_cast<float>(speed->getValue());

        Vector2<float> lookingVec = player->rotationComponent->Get();
        //Thanks for the math in old Lunity, echo!
        player->stateVectorComponent->velocity.x = cos((lookingVec.y + 90.0f) * (PI / 180.0f)) * flySpeed;
        player->stateVectorComponent->velocity.z = sin((lookingVec.y + 90.0f) * (PI / 180.0f)) * flySpeed;
        player->stateVectorComponent->velocity.y = sin((lookingVec.x) * -(PI / 180.0f)) * flySpeed;
    } else if (this->mode->getCurrentMode() == flyMode_t::Motion) {
        player->stateVectorComponent->velocity.y = -0.02f;
        float flySpeed = std::any_cast<float>(speed->getValue());
        Vector2<float> motion = getMotion(player->rotationComponent->Get().y, flySpeed);
        float flySpeedy = std::any_cast<float>(speedy->getValue());
        Vector2<float> motiony = getMotion(player->rotationComponent->Get().y, flySpeedy);
        bool w = isKeyDown("key.forward");
        bool a = isKeyDown("key.left");
        bool s = isKeyDown("key.back");
        bool d = isKeyDown("key.right");
        bool usingMoveKeys = w || a || s || d;
        if (!usingMoveKeys) {
            motion.x = 0.0f;
            motion.y = 0.0f;
        }
        player->stateVectorComponent->velocity.x = motion.x;
        player->stateVectorComponent->velocity.z = motion.y;

        if (isKeyDown("key.jump")) {
            player->stateVectorComponent->velocity.y = flySpeedy / 10.0f;
        }
        else if (isKeyDown("key.Shift")) {
            player->stateVectorComponent->velocity.y = -flySpeed / 10.0f;
        }
        else {
            player->stateVectorComponent->velocity.y = 0.0f;
        }
    }
    else if (this->mode->getCurrentMode() == flyMode_t::Vanilla) {
        if (!canWrite)
            return;

        player->setFlying(true);
        player->setFlightSpeed(std::max(speed->getValueAs<float>() / 20.0f, 0.05f));
    }
}