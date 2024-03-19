#pragma once

#include <memory>

//Yeeted from https://github.com/LiteLDev/LiteLoaderBDS/blob/50eb2650568b631679154190fc834bb89b81800b/LiteLoader/include/llapi/mc/Bedrock.hpp lololool

template <typename T>
class NonOwnerPointer {
public:
    std::shared_ptr<T> mPtr;

    NonOwnerPointer(T& a1) {
        mPtr = std::make_shared<T>(a1);
    }
};