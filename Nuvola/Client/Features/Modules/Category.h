#pragma once

#include "../Theme/ThemeIcon.h"
#include "Module.h"

#include <Data/Manager.h>

#include <string>
#include <vector>

class Category : public Manager<Module>
{
    ThemeIcon icon;
public:
    explicit Category(const std::string& name, ThemeIcon icon = ThemeIcon::INVALID);

    ThemeIcon getIcon();
};