#pragma once

#include "../Client/Bridge/Actor.h"

#include <set>
#include <string>
#include <vector>

namespace TargetUtils {
	Actor* getClosestActorFromVector(Actor* first, const std::vector<Actor*>& actors);
	std::vector<Actor*> findTargetMulti(Actor* sourceActor, const std::set<std::string>& selected, float reach = 3.0f, bool bots = false, bool teams = false);
	Actor* findTarget(Actor* sourceActor, const std::set<std::string>& selected, float reach = 3.0f, bool bots = false, bool teams = false);
}