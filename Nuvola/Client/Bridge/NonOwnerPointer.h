#pragma once

//Source: https://github.com/LiteLDev/LiteLoaderBDS/blob/50eb2650568b631679154190fc834bb89b81800b/LiteLoader/include/llapi/mc/Bedrock.hpp#L61

#include <memory>

template <typename T>
class NonOwnerPointer {
public:
    std::shared_ptr<T> mPtr;

    NonOwnerPointer(T& a1) {
        mPtr = std::make_shared<T>(a1);
    }
};