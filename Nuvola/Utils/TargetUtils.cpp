#include "TargetUtils.h"

#include "../Client/Bridge/Components/RuntimeIDComponent.h"
#include "../Client/Bridge/ClientInstance.h"
#include "../Client/Bridge/Level.h"
#include "../Client/Bridge/OwnerPtrT.h"

#include "../Client/Features/Modules/Misc/AntiBot.h"
#include "../Client/Features/Modules/ModuleManager.h"

#include <regex>

Actor* TargetUtils::getClosestActorFromVector(Actor* first, const std::vector<Actor*>& actors) {
	Actor* closest = nullptr;

	for (auto actor : actors) {
		//If the actor is the actor we are looking for, we don't want it
		if (actor == first) {
			continue;
		}
		//If closest isnt set, make it the first
		if (closest == nullptr) {
			closest = actor;
			continue;
		}
		//Calculate the distances
		float distCurrent = first->getPosition().distanceTo(actor->getPosition());
		float distClosest = first->getPosition().distanceTo(closest->getPosition());

		//If this actor is closer, make it the closest
		if (distCurrent < distClosest) {
			closest = actor;
		}
	}

	return closest;
}

std::vector<Actor*> TargetUtils::findTargetMulti(Actor* sourceActor, const std::set<std::string>& selected, float reach, bool bots, bool teams)
{
	//Get the level
	Level* level = sourceActor->level;
	std::vector<Actor*> results;

	//Remove the source actor from the list. We can't attack ourselves!
	//also check if we can attack at all
	std::shared_ptr<AntiBot> antiBot = systems->getModuleManager()->findModule<AntiBot>();
	auto actorOwners = sourceActor->entityContext.mEnttRegistry->view<ActorOwnerComponent>();
	for (auto& ownerEntt : actorOwners) {
		//why do I have to do this??
		if(!sourceActor->entityContext.mEnttRegistry->valid(ownerEntt))
			continue;

		Actor* actor = actorOwners.get<ActorOwnerComponent>(ownerEntt).actor;
		if (actor == sourceActor) {
			continue;
		}
		//Get the distance to the possible target
		float distance = sourceActor->getPosition().distanceTo(actor->getPosition());
		//Check if the distance is out of our reach
		if (distance > reach && reach >= 0) {
			continue;
		}
		if (!bots) {
			if (antiBot->isBot(actor)) {
				continue;
			}
		}
		if (teams) {
			std::string targetName = actor->getNameTag();
			std::string localName = sourceActor->getNameTag();

			if (targetName.length() > 2 && localName.length() > 2) {
				targetName = std::regex_replace(targetName, std::regex("\\�r"), "");
				localName = std::regex_replace(localName, std::regex("\\�r"), "");

				if (targetName.at(2) == localName.at(2)) {
					continue;
				}
			}
		}
		//All of this code is commented since we can only target players at the moment
		auto* idComponent = actor->entityContext.tryGetComponent<RuntimeIDComponent>();
		if (idComponent == nullptr)
		{
			continue;
		}
		auto yoink = idComponent->runtimeId;
		auto* defIdComponent = actor->entityContext.tryGetComponent<ActorDefinitionIdentifierComponent>();
		if (defIdComponent == nullptr)
		{
			continue;
		}
		ActorDefinitionIdentifier& defId = defIdComponent->identifier;
		//If the actor definition id is selected
		if (selected.contains(defId.canonicalName.get()))
		{
			//Dont remove it
			results.emplace_back(actor);
		}
	}

	return results;
}

Actor* TargetUtils::findTarget(Actor* sourceActor, const std::set<std::string>& selected, float reach, bool bots, bool teams)
{
	std::vector<Actor*> allActors = findTargetMulti(sourceActor, selected, reach, bots, teams);
	//Finally sort the vector for the closest actor
	return getClosestActorFromVector(sourceActor, allActors);
}
