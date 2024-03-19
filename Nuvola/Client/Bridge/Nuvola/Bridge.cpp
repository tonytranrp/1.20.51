#include "Bridge.h"

static ClientInstance* instance = nullptr;
void Bridge::setClientInstance(ClientInstance* inst)
{
	instance = inst;
}
ClientInstance* Bridge::getClientInstance()
{
	return instance;
}