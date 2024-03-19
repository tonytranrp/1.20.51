#pragma once

#include "WorldRenderer.h"

#include <optional>

class RenderController
{
	WorldRenderer worldRenderer;
	RenderController();
public:
	static RenderController* getInstance();

	void resetWorldRenderer();
	WorldRenderer& getWorldRenderer();

	void endFrame();
};