#include "../../IRC/ChatClient.h"
#include "IRC.h"

#include "../../../Bridge/Nuvola/Bridge.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/GuiData.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/ChatEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Preferences/Preferences.h"
#include "../Render/Notifications.h"
#include "../ModuleManager.h"

static std::string ircPrefix = "&3[&dIRC&3]&r ";
IRC::IRC() : Module("IRC", "Controls if the IRC feature in Nuvola is enabled.")
{
	this->showJoinLeave = this->addItem<Setting>("Show join/leave messages", "When enabled you will be able to see who connects and disconnects from the IRC channel.", SettingType::TOGGLE, true, false, true);
	this->setEnabled(true);
}

void IRC::onEnable()
{
	this->lastConn = 0;
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ChatEvent, &IRC::onChatEvent>(this);
	dispatcher->listen<LevelTickEvent, &IRC::onLevelTickEvent>(this);
}

void IRC::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ChatEvent, &IRC::onChatEvent>(this);
	dispatcher->deafen<LevelTickEvent, &IRC::onLevelTickEvent>(this);
	auto chatClient = systems->getChatClient();
	chatClient->disconnect();
}

void IRC::onTick() {
	const auto chatClient = systems->getChatClient();
	const auto prefs = systems->getPreferences();
	if (!chatClient->isAlive())
	{
		const auto timeNow = time(nullptr);
		const auto timeDelta = difftime(timeNow, this->lastConn);
		if (timeDelta >= 15)
		{
			if(chatClient->connect())
			{
				//Register the event callbacks
				chatClient->onClientException([](auto error){
					const auto notifs = systems->getModuleManager()->findModule<Notifications>();
					notifs->notify(error, "IRC Error");
				});
				chatClient->onServerError([](auto error){
					const auto ci = Bridge::getClientInstance();
					if (ci == nullptr) return;
					const auto gd = ci->getGuiData();
					if (gd == nullptr) return;

					auto irc = systems->getModuleManager()->findModule<IRC>();
					auto& queueLock = irc->queueLock;
					auto& messageQueue = irc->messageQueue;
					std::lock_guard<std::mutex> guard(queueLock);
					messageQueue.emplace_back(ircPrefix + error.data);
				});
				chatClient->onDisconnect([](){
					const auto ci = Bridge::getClientInstance();
					if (ci == nullptr) return;
					const auto gd = ci->getGuiData();
					if (gd == nullptr) return;

					auto irc = systems->getModuleManager()->findModule<IRC>();
					auto& queueLock = irc->queueLock;
					auto& messageQueue = irc->messageQueue;
					std::lock_guard<std::mutex> guard(queueLock);
					messageQueue.emplace_back(ircPrefix + "&cDisconnected");
				});
				chatClient->onConnect([]() {
					const auto ci = Bridge::getClientInstance();
					if (ci == nullptr) return;
					const auto gd = ci->getGuiData();
					if (gd == nullptr) return;

					auto irc = systems->getModuleManager()->findModule<IRC>();
					auto& queueLock = irc->queueLock;
					auto& messageQueue = irc->messageQueue;
					std::lock_guard<std::mutex> guard(queueLock);
					messageQueue.emplace_back("&aIRC connected! Please wait for authorization...");
				});
				chatClient->onLoginComplete([]() {
					auto prefs = systems->getPreferences();
					auto chatClient = systems->getChatClient();
					//Create the set mod operation
					const ChatClient::ChatOperation modOperation(ChatClient::OpCodes::SET_MOD, "&6Nuvola", true);
					chatClient->sendOperationAuto(modOperation);

					std::string ircName = "Anonymous";
					//Set the display name
					if (!prefs->ircName.empty())
					{
						ircName = prefs->ircName;
					}
					const ChatClient::ChatOperation nameOperation(ChatClient::OpCodes::SET_NAME, ircName, true);
					chatClient->sendOperationAuto(nameOperation);

					auto irc = systems->getModuleManager()->findModule<IRC>();
					auto& queueLock = irc->queueLock;
					auto& messageQueue = irc->messageQueue;
					std::lock_guard<std::mutex> guard(queueLock);
					messageQueue.emplace_back("&aAuthorization complete! Prefix messages with # to talk with other client users.");
				});

				auto commonCallback = [](auto data, bool successful)
				{
					auto irc = systems->getModuleManager()->findModule<IRC>();
					auto& queueLock = irc->queueLock;
					auto& messageQueue = irc->messageQueue;
					std::lock_guard<std::mutex> guard(queueLock);
					messageQueue.emplace_back(ircPrefix + data);
				};

				chatClient->when<ChatClient::OpCodes::INCOMING_CHAT>(commonCallback);
				chatClient->when<ChatClient::OpCodes::REQUEST_CONNECTED_COUNT>(commonCallback);
				chatClient->when<ChatClient::OpCodes::REQUEST_CONNECTED_NAMES>(commonCallback);
				chatClient->when<ChatClient::OpCodes::SET_NAME>(commonCallback);
				chatClient->when<ChatClient::OpCodes::SET_CHANNEL>(commonCallback);
				chatClient->when<ChatClient::OpCodes::RATELIMIT>(commonCallback);

				auto joinLeaveCallback = [&](auto data, bool successful)
				{
					auto irc = systems->getModuleManager()->findModule<IRC>();
					if(irc->showJoinLeave->getValueAs<bool>())
					{
						commonCallback(data, successful);
					}
				};
				chatClient->when<ChatClient::OpCodes::JOIN_CHANNEL>(joinLeaveCallback);
				chatClient->when<ChatClient::OpCodes::LEAVE_CHANNEL>(joinLeaveCallback);
			}
			lastConn = timeNow;
		}
	}
}

void IRC::onChatEvent(ChatEvent &event) {
	auto chatClient = systems->getChatClient();
	const std::string& message = event.getMessage();

	if (message[0] == '#') { //This is an irc message
		//Get the message without the prefix
		std::string noPrefix = message.substr(1);

		//Form the chat operation
		ChatClient::ChatOperation chatOperation(ChatClient::OpCodes::MESSAGE, std::move(noPrefix), true);

		//Send the operation
		chatClient->sendOperationAuto(chatOperation);

		//Cancel the chat from being sent
		event.setCancelled(true);
	}
}

void IRC::onLevelTickEvent(LevelTickEvent &event) {
	const auto ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	auto gd = ci->getGuiData();
	if (gd == nullptr) return;
	const auto lp = ci->getClientPlayer();
	if (lp == nullptr) return;

	auto chatClient = systems->getChatClient();
	if (lp->ticksAlive == 1 && chatClient->isAlive())
	{
		gd->displayClientMessage("&aIRC connected! Prefix messages with # to talk with other client users.");
	}

	std::lock_guard<std::mutex> guard(this->queueLock);
	for (const auto& message : this->messageQueue)
	{
		gd->displayClientMessage(message);
	}
	this->messageQueue.resize(0);
}