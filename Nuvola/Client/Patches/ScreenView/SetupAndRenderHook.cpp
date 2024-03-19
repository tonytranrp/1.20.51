#include "SetupAndRenderHook.h"

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Bridge/ClientInstance.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/RenderEvent.h"
#include "../../Rendering/RenderController.h"

#include <queue>
#include <string>

struct FrameTransforms
{
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	Vector3<float> origin;
};
extern std::queue<FrameTransforms> frameTransforms;

void __fastcall SetupAndRenderHook::setupAndRenderCallback_1_17_10_4(class ScreenView* screenView, class MinecraftUIRenderContext* renderContext) {
	PLH::FnCast(setupAndRenderOriginal, setupAndRenderCallback_1_17_10_4)(screenView, renderContext);

	auto ci = Bridge::getClientInstance();
	if(ci == nullptr) return;
	auto hmdState = ci->getHMDState();
	if(hmdState == nullptr) return;
	FrameTransforms transform;
	transform.worldMatrix = hmdState->mLastLevelWorldMatrix;
	transform.viewMatrix = hmdState->mLastLevelViewMatrix;
	transform.projMatrix = hmdState->mLastLevelProjMatrix;
	LevelRenderer* lr = ci->getLevelRenderer();
	if(lr)
	{
		LevelRendererPlayer* lrp = lr->levelRendererPlayer;
		if(lrp)
		{
			transform.origin.x = lrp->mOrigin.x;
			transform.origin.y = lrp->mOrigin.y;
			transform.origin.z = lrp->mOrigin.z;
		}
	}
	frameTransforms.push(transform);
}

SetupAndRenderHook::SetupAndRenderHook() : IPatch::IPatch("ScreenView::setupAndRender") {
	this->addSignature(SigTable::ScreenView_setupAndRender);
}

bool SetupAndRenderHook::apply() {
	return this->autoPatch(setupAndRenderCallback_1_17_10_4, &setupAndRenderOriginal);
}