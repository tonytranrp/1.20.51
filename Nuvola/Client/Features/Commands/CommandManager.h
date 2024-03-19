#pragma once

#include "../../Events/ChatEvent.h"
#include "../../System.h"
#include "Based/bcs.h"

#include "Command.h"

#include <Data/Manager.h>

struct CommandManager : public ::System<Manager<Command>> {
    CommandManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;
    const std::vector<std::shared_ptr<Command>>& getAllCommands() {
		return this->getItems();
	}
private:
	template<typename command_t, typename... args_t>
	void initialize(args_t... args)
	{
		auto command = this->addItem<command_t>(args...);
		command->initialize(dispatcher);
	}

	void onChatEvent(ChatEvent& event);

	bcs::command_dispatcher dispatcher;
};