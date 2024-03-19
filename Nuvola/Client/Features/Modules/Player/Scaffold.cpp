#include "Scaffold.h"

#include "../../../../Utils/RenderUtils.h"

#include "../../../Bridge/Block.h"
#include "../../../Bridge/BlockLegacy.h"
#include "../../../Bridge/BlockSource.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GameMode.h"
#include "../../../Bridge/GuiData.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/MinecraftGame.h"
#include "../../../Bridge/Minecraft.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Bridge/Options.h"
#include "../../../Bridge/Packet.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"
#include "../../../Events/RenderEvent.h"

#include "../../../Rendering/GeometryBuilder.h"
#include "../../../Rendering/RenderController.h"
#include "../../../Rendering/WorldRenderer.h"

#include "../../Theme/ThemeManager.h"

#include "../ModuleManager.h"
#include "../Render/Notifications.h"

#include "imgui_custom.h"

#include <Math/Math.h>

#include <deque>

Scaffold::Scaffold() : Module("Scaffold", "Bridges for you automatically")
{
    this->buildSides = this->addItem<Setting>("Sides", "If weird side placements should be allowed. This is best for diagonal scaffold, but often detected by anticheats.", SettingType::TOGGLE, false, false, true);
    this->instantExtend = this->addItem<Setting>("Instant Extend", "Instantly bridges forward.", SettingType::TOGGLE, true, false, true);
    this->extend = this->addItem<Setting>("Extend", "Bridges ahead of you for the specified amount of blocks. This may be detectable by anticheats.", SettingType::SLIDER_FLOAT, 0.f, 0.f, 10.f);
    this->distance = this->addItem<Setting>("Distance", "The max distance to 'predict' from a block", SettingType::SLIDER_INT, 0, 0, 10);
    this->places = this->addItem<Setting>("Places", "The max amount of blocks to place in one tick", SettingType::SLIDER_INT, 1, 1, 30);
}

static bool failed = false;
static int ticksEnabled = 0;
//static int resetTicks = 0;
static std::vector<int> lastSides;
static std::deque<Vector3<int>> lastSolidBlocks;
static std::recursive_mutex blocksMutex;
static Vector3<int> blockPos;
static Vector3<float> currentPos;
static Vector3<int> targetBlock;
static Vector3<int> nextPlace;

static float currentExtend = 0.f;
static float startY = 0.f;
static float leftRightX = 0.f;
static float leftRightZ = 0.f;

void Scaffold::onEnable()
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) { this->setEnabled(false); return; };
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) { this->setEnabled(false); return; };
    ticksEnabled = 0;

    std::vector<int> sides;
    bool bridgedSideways = false;
    currentPos = player->getPosition();
    startY = currentPos.y - 2.0f;

    failed = false;

    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Scaffold::onLevelTickEvent>(this);
    dispatcher->listen<PacketEvent, &Scaffold::onPacketEvent>(this);
    dispatcher->listen<RenderEvent, &Scaffold::onRenderEvent>(this);
}

void Scaffold::onDisable()
{
    std::lock_guard<std::recursive_mutex> guard(blocksMutex);
    lastSolidBlocks.clear();
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Scaffold::onLevelTickEvent>(this);
    dispatcher->deafen<PacketEvent, &Scaffold::onPacketEvent>(this);
    dispatcher->deafen<RenderEvent, &Scaffold::onRenderEvent>(this);
}

bool isSurroundedByAir(Vector3<int> pos)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return true;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return true;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return true;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return true;

    std::vector<Vector3<int>> neighbors = {
        {pos.x + 1, pos.y, pos.z},
        {pos.x - 1, pos.y, pos.z},
        {pos.x, pos.y + 1, pos.z},
        {pos.x, pos.y - 1, pos.z},
        {pos.x, pos.y, pos.z + 1},
        {pos.x, pos.y, pos.z - 1}
    };

    bool surrounded = true;
    for (const auto& neighbor : neighbors) {
        const Block* block = blockSource->getBlock(neighbor);
        if (block->blockLegacy != nullptr && !block->blockLegacy->isAir())
        {
            surrounded = false;
        }
    }

    return surrounded;
}

bool Scaffold::isValidPlacePos(Vector3<int> pos)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return false;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return false;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return false;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return false;
    const Block* block = blockSource->getBlock(pos);
    if (block->blockLegacy == nullptr) return false;
    if (Scaffold::getBlockPlaceFace(pos) == INT_MAX) return false;
    if (!block->blockLegacy->isAir()) return false;
    return true;
}

Vector3<int> Scaffold::getClosestPlacePos(Vector3<int> pos, int maxDistance)
{

    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return pos;
    GuiData* guiData = ci->getGuiData();
    if (guiData == nullptr) return pos;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return pos;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return pos;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return pos;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return pos;
    const Block* block = blockSource->getBlock(pos);
    if (block->blockLegacy == nullptr) return pos;
    if (!block->blockLegacy->isAir()) return pos;

    if (Scaffold::getBlockPlaceFace(pos) != INT_MAX) return pos;

    for (int i = 0; i < 5; i++)
    {
        Vector3<int> blockSel = pos + getSideOffset(i);
        if (isValidPlacePos(blockSel))
        {
            return blockSel;
        }
    }
    
    Vector3<int> maxDist = Vector3<int>(maxDistance, maxDistance, maxDistance);

    std::vector<Vector3<int>> blocks = getBlocksBetween(pos - maxDist,pos + maxDist, true);

    Vector3<int> closestBlock = { INT_MAX, INT_MAX, INT_MAX };
    int closestDistance = INT_MAX;

    for (const auto& bs : blocks)
    {
        Vector3<int> blockSel = bs;

        int face = getClosestBlockFace(blockSel);

        blockSel = blockSel - getSideOffset(face);
        if (isValidPlacePos(blockSel))
        {
            int distance = blockSel.distanceTo(pos);
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestBlock = blockSel;
            }
        }

    }

    return closestBlock;
}

#undef min
#undef max

// getBlocksBetween
std::vector<Vector3<int>> Scaffold::getBlocksBetween(Vector3<int> start, Vector3<int> end, bool includeAir)
{
    std::vector<Vector3<int>> blocks;
    Vector3<int> min = Vector3<int>(std::min(start.x, end.x), std::min(start.y, end.y), std::min(start.z, end.z));
    Vector3<int> max = Vector3<int>(std::max(start.x, end.x), std::max(start.y, end.y), std::max(start.z, end.z));
    bool includeAnySolidBlocks = false;
    for (int x = min.x; x <= max.x; x++)
    {
        for (int y = min.y; y <= max.y; y++)
        {
            for (int z = min.z; z <= max.z; z++) {
                Vector3<int> pos = Vector3<int>(x, y, z);
                //if(!isAirBlock(pos)) includeAnySolidBlocks = true;
                if (!includeAir && isAirBlock(pos)) continue;
                blocks.push_back(pos);
            }
        }
    }
    return includeAnySolidBlocks ? blocks : std::vector<Vector3<int>>();
}

int Scaffold::getBlockPlaceFace(Vector3<int> pos)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return INT_MAX;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return INT_MAX;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return INT_MAX;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return INT_MAX;

    if (!isAirBlock(pos + Vector3<int>(0, -1, 0))) return 1;
    if (!isAirBlock(pos + Vector3<int>(0, 0, 1))) return 2;
    if (!isAirBlock(pos + Vector3<int>(0, 0, -1))) return 3;
    if (!isAirBlock(pos + Vector3<int>(1, 0, 0))) return 4;
    if (!isAirBlock(pos + Vector3<int>(-1, 0, 0))) return 5;
    if (!isAirBlock(pos + Vector3<int>(0, 1, 0))) return 0;

    return INT_MAX;
}

Vector3<int> Scaffold::getSideOffset(int side)
{
    switch (side)
    {
    case 0:
        return Vector3<int>(0, 1, 0);
    case 1:
        return Vector3<int>(0, -1, 0);
    case 2:
        return Vector3<int>(0, 0, 1);
    case 3:
        return Vector3<int>(0, 0, -1);
    case 4:
        return Vector3<int>(1, 0, 0);
    case 5:
        return Vector3<int>(-1, 0, 0);
    default:
        return Vector3<int>(0, 0, 0);
    }
}

int Scaffold::getClosestBlockFace(Vector3<int> blockSel)
{
    float closestDist = FLT_MAX;
    int closestFace = 0;
    blockSel.y = 0;
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return INT_MAX;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return INT_MAX;
    Vector3 playerPos = player->stateVectorComponent->currentPos - Vector3(0, 1, 0);
    if (abs(playerPos.y - blockSel.y) > 1.5f) playerPos.y = blockSel.y;
    for (int i = 5; i >= 0; i--)
    {
        Vector3<int> bSel = blockSel - getSideOffset(i);
        Vector3<float> bSelF = Vector3<float>(bSel.x, bSel.y, bSel.z);
        float dist = Vector3(playerPos.x, playerPos.y, playerPos.z).distanceTo(bSelF);
        if (dist <= closestDist)
        {
            closestDist = dist;
            closestFace = i;
        }
    }
    return closestFace;
}


Vector3<int> Scaffold::getBlockPosRotation(float extend)
{
    Vector3<int> defaultRet = { INT_MAX, INT_MAX, INT_MAX };
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return defaultRet;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return defaultRet;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return defaultRet;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return defaultRet;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return defaultRet;
    ClientInputHandler* cih = ci->mClientInputHandler;
    if (!cih) return defaultRet;
    ClientInputMappingFactory* cimf = cih->factory;
    if (!cimf) return defaultRet;
    KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
    if (!kms) return defaultRet;

    Vector3<int> blockSel = { INT_MAX, INT_MAX, INT_MAX };
    Vector2<float> playerRots = player->rotationComponent->Get();

    bool w = isKeyDown(kms->GetMCKey("key.forward"));
    bool a = isKeyDown(kms->GetMCKey("key.left"));
    bool s = isKeyDown(kms->GetMCKey("key.back"));
    bool d = isKeyDown(kms->GetMCKey("key.right"));

    if(w && !a && !s && !d){
        playerRots.y += 0;
    } else if(w && a && !s && !d){
        playerRots.y -= 45;
    } else if(!w && a && !s && !d){
        playerRots.y -= 90;
    } else if(!w && a && s && !d){
        playerRots.y -= 135;
    } else if(!w && !a && s && !d){
        playerRots.y -= 180;
    } else if(!w && !a && s && d){
        playerRots.y -= 225;
    } else if(!w && !a && !s && d){
        playerRots.y -= 270;
    } else if(w && !a && !s && d){
        playerRots.y -= 315;
    }

    float correctedYaw = (playerRots.y + 90) * ((float)PI / 180);
    float inFrontX = cos(correctedYaw) * extend;
    float inFrontZ = sin(correctedYaw) * extend;
    float placeX = player->stateVectorComponent->currentPos.x + inFrontX;
    float placeZ = player->stateVectorComponent->currentPos.z + inFrontZ;

    if (playerRots.y >= -67.5 && playerRots.y < -22.5 && playerRots.y < 0) //pos x & pos z
    {
        placeX += leftRightX >= 1.f && leftRightZ == 0.f ? leftRightX : 0.f;
        placeZ += leftRightZ >= 1.f && leftRightX == 0.f ? leftRightZ : 0.f;
    }
    else if (playerRots.y >= 22.5 && playerRots.y < 67.5 && playerRots.y > 0) //pos z & neg x
    {
        placeX -= leftRightX >= 1.f && leftRightZ == 0.f ? leftRightX : 0.f;
        placeZ += leftRightZ >= 1.f && leftRightX == 0.f ? leftRightZ : 0.f;
    }
    else if (playerRots.y >= 112.5 && playerRots.y < 157.5 && playerRots.y > 0) //neg x & neg z
    {
        placeX -= leftRightX >= 1.f && leftRightZ == 0.f ? leftRightX : 0.f;
        placeZ -= leftRightZ >= 1.f && leftRightX == 0.f ? leftRightZ : 0.f;
    }
    else if (playerRots.y >= -157.5 && playerRots.y < -112.5 && playerRots.y < 0) //neg z & pos x
    {
        placeX += leftRightX >= 1.f && leftRightZ == 0.f ? leftRightX : 0.f;
        placeZ -= leftRightZ >= 1.f && leftRightX == 0.f ? leftRightZ : 0.f;
    }

    if ((int)placeX >= 0)
        blockSel.x = (int)floor(placeX);
    else
        blockSel.x = (int)floor(placeX);

    if ((int)placeZ >= 0)
        blockSel.z = (int)floor(placeZ);
    else
        blockSel.z = (int)floor(placeZ);

    blockSel.y = startY;
    return blockSel;
}

void Scaffold::onLevelTickEvent(LevelTickEvent& event) {
    int blocksPlaced = 0;
    std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
    if (notifs == nullptr) return;

    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    MinecraftGame* mcGame = ci->getMinecraftGame();
    if (mcGame == nullptr) return;
    GuiData* guiData = ci->getGuiData();
    if (guiData == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return;
    ClientInputHandler* cih = ci->mClientInputHandler;
    if (!cih) return;
    ClientInputMappingFactory* cimf = cih->factory;
    if (!cimf) return;
    KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
    if (!kms) return;

    Vector3<float> playerPos = player->getPosition();
    playerPos.y -= 2.f;

    int places = this->places->getValueAs<int>();
    Vector3<float> predictedPlayerPos = playerPos + (player->stateVectorComponent->velocity * 0.2f);


    for (int i = 0; i < places; i++) {

        float extendMax = this->extend->getValueAs<float>();
        currentExtend = currentExtend >= extendMax ? 0.f : currentExtend + (float)1;

        Vector3<float> blockPos = getBlockPosRotation(currentExtend);
        Vector3<float> velocity = player->stateVectorComponent->velocity;

        bool forwardDown = isKeyDown(kms->GetMCKey("key.forward"));
        bool backDown = isKeyDown(kms->GetMCKey("key.back"));
        bool leftDown = isKeyDown(kms->GetMCKey("key.left"));
        bool rightDown = isKeyDown(kms->GetMCKey("key.right"));


        bool usingMoveKeys = forwardDown || backDown || leftDown || rightDown;

        if(isAirBlock(getBlockPosRotation(0)) || !usingMoveKeys)
        {
            blockPos = getBlockPosRotation(0);
        }
        if (startY > playerPos.y)
        {
            startY = playerPos.y;
            startY = (int)startY;
        } else if (playerPos.y - startY > 3.0f)
        {
            startY = playerPos.y;
            startY = (int)startY;
        }
        bool jumpDown = isKeyDown(32);
        Vector3<float> predictedBlockPos = getBlockPosRotation(currentExtend);
        if (usingMoveKeys) {
            predictedBlockPos.x += predictedPlayerPos.x - playerPos.x;
            predictedBlockPos.y += predictedPlayerPos.y - playerPos.y;
            predictedBlockPos.z += predictedPlayerPos.z - playerPos.z;
        }

        if(!usingMoveKeys && jumpDown && abs(velocity.x) < 0.08f && abs(velocity.z) < 0.08f)
        {
            startY = playerPos.y;
            // round startY down to nearest block
            startY = (int)startY;
        }

        Vector3<int> blockPosInt = { static_cast<int>(blockPos.x), static_cast<int>(blockPos.y), static_cast<int>(blockPos.z) };
        nextPlace = blockPosInt;
        int face = getBlockPlaceFace(blockPosInt);
        if (face == INT_MAX)
        {
            blockPosInt = getClosestPlacePos(blockPosInt, (int)this->distance->getValueAs<int>());
            targetBlock = blockPosInt;
            face = getBlockPlaceFace(blockPosInt);

            if (face == INT_MAX) {
                currentExtend = 0.f;
                continue;
            }
        }
        
        if (!isAirBlock(blockPosInt)) {
            if (isValidPlacePos(targetBlock)) {
                if (!isAirBlock(targetBlock)) {
                    if (!isSurroundedByAir(targetBlock)) {
                        gm->buildBlock(targetBlock, face);
                        
                        predictedPlayerPos = playerPos + (velocity * 0.05f); 

                        blocksPlaced++;
                    }
                }
            }
        }
        blockPosInt += getSideOffset(face);
        targetBlock = blockPosInt;
        
        if (isAirBlock(blockPosInt)) continue;
        
        blocksPlaced++;
        gm->buildBlock(blockPosInt, face);

    }
}

bool Scaffold::isKeyDown(int key) {
    ImGuiKey imkey = ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key);
    return ImGui::IsKeyDown(imkey);
}

void Scaffold::placeBlock(Vector3<int>, int side)
{
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return;
    GameMode* gm = player->mGameMode;
    if (gm == nullptr) return;

}

bool Scaffold::isAirBlock(Vector3<int> bp) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return false;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return false;
    std::shared_ptr<Dimension> dim = player->dimension;
    if (dim == nullptr) return false;
    BlockSource* blockSource = dim->blockSource;
    if (blockSource == nullptr) return false;
    BlockLegacy* block = blockSource->getBlock(bp.x, bp.y, bp.z)->blockLegacy;
    if (block == nullptr) return false;
    return block->blockId == BlockId::AIR;
}

void Scaffold::onRenderEvent(RenderEvent& event)
{
    if (!this->isEnabled()) return;
    if (event.getTime() != RenderTime::WORLD) return;

    RenderController* controller = RenderController::getInstance();
    WorldRenderer& renderer = controller->getWorldRenderer();
    GeometryBuilder builder(&renderer);

    std::lock_guard<std::recursive_mutex> guard(blocksMutex);
    for (auto lastSolidBlock : lastSolidBlocks)
        builder.buildBlock(lastSolidBlock, ColorF(1.0f, 0.0f, 1.0f, 0.5f), false);

    builder.buildBlock(blockPos, ColorF(1.0f, 1.0f, 1.0f, 0.5f), false);
    builder.buildBlock(targetBlock, ColorF(0.0f, 1.0f, 0.0f, 0.5f), false);
    builder.buildBlock(nextPlace, ColorF(0.0f, 1.0f, 1.0f, 0.5f), false);
}

void Scaffold::onPacketEvent(PacketEvent& event) {
    PlayerAuthInputPacket* paip = nullptr;
    MovePlayerPacket* mpp = nullptr;

    if (event.getPacket()->getId() == PacketID::MovePlayerPacket) {
        mpp = (MovePlayerPacket*)event.getPacket();
    }
    else if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket) {
        paip = (PlayerAuthInputPacket*)event.getPacket();
    }
    else {
        return;
    }

    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;

    Vector3<float> targetBlockF = {
        targetBlock.x + 0.5f,
        targetBlock.y + 0.5f,
        targetBlock.z + 0.5f
    };

    Vector3<float> lookTarget = targetBlockF;
    Vector2<float> lookVec = player->getPosition().calcAngle(lookTarget);

    
    if (paip != nullptr) {
        paip->rotation = lookVec;
        paip->headYaw = lookVec.y;
    }
    if (mpp != nullptr) {
        mpp->rotation = lookVec;
        mpp->headYaw = lookVec.y;

        
        mpp->onGround = true;
    }
}

