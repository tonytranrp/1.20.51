#pragma once

#include "ScreenData.h"

#include "Data/ManagedItem.h"

#include <string>

class Screen : public ManagedItem
{
public:
    Screen(std::string name);
    virtual void initialize(ScreenData*) = 0;
    virtual void render(float deltaTime) = 0;
    virtual void cleanup() = 0;
};