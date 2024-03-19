#include "ManagedItem.h"

std::string ManagedItem::getName() const {
    return name;
}

ManagedItem::ManagedItem(const std::string_view name) {
    this->name = name;
}

bool ManagedItem::isManager() {
    return false;
}