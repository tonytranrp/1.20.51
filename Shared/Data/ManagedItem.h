#pragma once

#include <string>

class ManagedItem {
protected:
    std::string name;
public:
    explicit ManagedItem(std::string_view name);

    ManagedItem() = delete;
    ManagedItem(const ManagedItem&) = delete;
    ManagedItem(ManagedItem&&) = delete;
    void operator=(const ManagedItem&) = delete;
    void operator=(ManagedItem&&) = delete;

    virtual ~ManagedItem() = default;

	[[nodiscard]] virtual std::string getName() const;
    virtual bool isManager();
};
