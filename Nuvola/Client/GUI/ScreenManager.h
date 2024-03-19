#pragma once

#include "Screen.h"
#include "ScreenData.h"

#include "../System.h"

#include <Data/Manager.h>

class ScreenManager : public ::System<Manager<Screen>>
{
	std::vector<std::shared_ptr<Screen>> screenStack;
public:
	ScreenManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

	template<typename T>
	void registerScreen()
	{
		this->addItem<T>();
	}

	std::shared_ptr<Screen> getActiveScreen();
	void pushScreen(std::shared_ptr<Screen> screen, ScreenData* data);
	bool pushScreen(std::string_view screenName, ScreenData* data);
	template<typename T>
	bool pushScreen(ScreenData* data)
	{
		for (auto& screen : this->getItems())
		{
			std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(screen);
			if (casted != nullptr)
			{
				this->pushScreen(casted, data);
				return true;
			}
		}
		return false;
	}
	void popScreen();
};