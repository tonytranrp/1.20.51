#include "RenderController.h"

RenderController::RenderController()
{

}

static RenderController* instance = nullptr;
RenderController* RenderController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new RenderController();
	}
	return instance;
}

void RenderController::resetWorldRenderer()
{
	this->worldRenderer.reset();
}

WorldRenderer& RenderController::getWorldRenderer()
{
	return this->worldRenderer;
}

void RenderController::endFrame()
{
	this->worldRenderer.reset();
}
