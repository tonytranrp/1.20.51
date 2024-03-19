// REGEN.H
#pragma once

#include "InputMode.h"
#include "../Signatures/SigManager.h"
#include "../SystemManager.h"
#include "Macro.h"
#include <Math/Math.h>


class GameMode {
public:
    /* Fields */

    class Player* player; // 0x0008
    char pad_0008[8]; // 0x0010
    char pad_0010[8]; // 0x0018
    char pad_0018[4]; // 0x0020
    float destroyProgress; // 0x0024

    bool mLastBuildBlockWasInteractive;
    bool mLastBuildBlockWasSnappable;
    float mMinPlayerSpeed;
    int mContinueBreakBlockCount;
    std::chrono::steady_clock::time_point mLastBuildTime;
    std::chrono::steady_clock::time_point mNoDestroyUntil;
    std::chrono::steady_clock::time_point mNoDestroySoundUntil;
    std::chrono::milliseconds mCreativeDestructionTickDelay;
    std::chrono::milliseconds mBuildingTickDelay;
    std::chrono::milliseconds mDestroySoundDelay;
    bool mHasBuildDirection;
    bool mHasLastBuiltPosition;
    unsigned char mContinueFacing;




    /* Virtuals */
    virtual ~GameMode();
    virtual bool startDestroyBlock(Vector3<int>* blockPos, int blockSide, bool& isDestroyedOut) {}
    virtual bool destroyBlock(Vector3<int>* blockPos, int side) {}
    virtual bool continueDestroyBlock(Vector3<int> const& blockPos, unsigned int blockSide, Vector3<float> const& playerPosition, bool&) {}
    virtual void stopDestroyBlock(Vector3<int>* blockPos) {}
    virtual void startBuildBlock(Vector3<int>* param_1, bool param_2) {}
    virtual bool buildBlock(Vector3<int>& where, uint8_t face, bool simTickCheck = true) {}
    virtual void continueBuildBlock(Vector3<int>* param_1, bool param_2) {}
    virtual void stopBuildBlock() {}
    virtual void tick() {}
    virtual float getPickRange(InputMode param_1, bool param_2) {}
    virtual bool useItem(class ItemStack* param_1) {}
    virtual bool useItemOn(class ItemStack* param_1, Vector3<int>* param_2, bool param_3, Vector3<float>* param_4, class Block* param_5) {}
    virtual bool interact(class Actor* param_1, Vector3<float>* param_2) {}
    virtual bool attack(class Actor* param_1) {}
    virtual void releaseUsingItem() {}
    virtual void* _guard_check_icall() { return nullptr; }
    virtual void* sendTryDestroyBlock() { return nullptr; }
    virtual void exitVRMode(std::function<void __cdecl()> param_1) {}

    /* Functions */
    float getDestroyRate(Block* block) {
        auto func = systems->getSigManager()->findAddressAs<float (__fastcall *)(GameMode *, Block *)>(
                SigTable::GameMode_getDestroyRate);
        return func(this, block);
    }
};
// REGEN.CPP

//GAMEMODE.H
