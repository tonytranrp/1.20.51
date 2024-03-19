#include "Category.h"

Category::Category(const std::string& name, ThemeIcon icon) : Manager<Module>(name) {
    this->icon = icon;
}

ThemeIcon Category::getIcon()
{
    return this->icon;
}