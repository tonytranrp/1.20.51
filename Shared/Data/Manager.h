#pragma once

#include "ManagedItem.h"

#include <memory>
#include <type_traits>
#include <vector>

template<class T = ManagedItem> class Manager : public ManagedItem {
    static_assert(std::is_base_of_v<ManagedItem, T>, "T must derive from ManagedItem");
protected:
    std::vector<std::shared_ptr<T>> items;
    std::mutex itemsMutex;
public:
    //Constructor
    explicit Manager(std::string_view name) : ManagedItem(name) {
        items = std::vector<std::shared_ptr<T>>();
    }
	~Manager() override {
		items.clear();
	}

    Manager() = delete;
    Manager(const Manager&) = delete;
    Manager(Manager&&) = delete;
    void operator=(const Manager&) = delete;
    void operator=(Manager&&) = delete;

    //Add item
    template<typename item_t, typename... args_t>
    std::shared_ptr<item_t> addItem(args_t&&... args) {
        static_assert(std::is_base_of_v<T, item_t>, "item_t must derive from T");
        std::shared_ptr<item_t> itemPtr = std::make_shared<item_t>(std::forward<args_t>(args)...);
        {
            std::lock_guard lock{itemsMutex};
            items.emplace_back(itemPtr);
        }
        //Must return a copy
        return itemPtr;
    }
    //Remove item at index
    void removeItem(int item) {
        items.erase(items.begin() + item);
    }
    //Remove item via pointer
    void removeItem(const std::shared_ptr<T>& item) {
        //For each item
        for (int i = 0; i < items.size(); i++) {
            //If item has the same pointer
            if (item == items.at(i)) {
                //Remove it
                removeItem(i);
                return;
            }
        }
    }
    //Get item at index
    const std::shared_ptr<T>& getItem(int index) {
        return items.at(index);
    }
    const std::shared_ptr<T>& getItem(std::string_view name) {
        //Loop through all items
        for (int i = 0; i < items.size(); i++) {
            //Get item at i
            if(auto currentItem = items.at(i))
            {
                //Check if the item has the name
                if (name == currentItem->getName()) {
                    //If it does, return it
                    return currentItem;
                }
            }
        }
        //Nothing was found, so return nullptr
        return nullptr;
    }
    const std::vector<std::shared_ptr<T>>& getItems() const {
        return this->items;
    }
    //It is a Manager, so make sure we set this to return true
    bool isManager() override {
        return true;
    }
};
