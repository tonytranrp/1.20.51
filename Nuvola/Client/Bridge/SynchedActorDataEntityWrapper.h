#pragma once

#include "SynchedActorData.h"

class SynchedActorDataEntityWrapper
{
	SynchedActorData* synchedData;

public:
	[[nodiscard]] inline SynchedActorData* getData()
	{
		return this->synchedData;
	}
};