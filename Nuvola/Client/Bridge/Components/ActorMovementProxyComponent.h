#pragma once

#include <memory>

#include <gsl/gsl>

#include "../IActorMovementProxy.h"

class ActorMovementProxyComponent
{
	std::shared_ptr<IActorMovementProxy> movementProxy;

public:
	ActorMovementProxyComponent(gsl::not_null<std::shared_ptr<IActorMovementProxy>> proxy)
	{
		this->movementProxy = proxy;
	}

	std::shared_ptr<IActorMovementProxy> getMovementProxy()
	{
		return this->movementProxy;
	}
};