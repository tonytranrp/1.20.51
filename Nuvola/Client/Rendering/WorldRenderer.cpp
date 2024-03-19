#include "WorldRenderer.h"

#include "../Bridge/ClientInstance.h"

WorldRenderer::WorldRenderer() : IRenderer()
{

}

WorldRenderer::~WorldRenderer() = default;

void WorldRenderer::reset()
{
    std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->data.resize(0);
}

bool WorldRenderer::worldToScreen(const Vector3<float>& in, Vector2<float>& out) const
{
	ClientInstance* ci = Bridge::getClientInstance();
	return ci->worldToScreen(in, out);
}

void WorldRenderer::begin(VertexCommand cmd)
{
    std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->data.emplace_back(cmd, Vector3<float>(), ColorF(1,1,1,1));
}

void WorldRenderer::vertex(Vector2<float> pos, ColorF color)
{
	std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->data.emplace_back(VertexCommand::VERTEX2D, Vector3<float>{pos.x, pos.y, 0.0f});
}

void WorldRenderer::vertex(Vector3<float> pos, ColorF color)
{
    std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->data.emplace_back(VertexCommand::VERTEX, pos, color);
}

void WorldRenderer::end(VertexCommand command, ColorF color, float thickness)
{
    std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->data.emplace_back(command, Vector3<float>(), color, thickness);
}

const std::vector<Vertex>* WorldRenderer::finalize()
{
    std::lock_guard<std::mutex> dataLock(this->dataMutex);
	this->finalized = std::vector<Vertex>(this->data.begin(), this->data.end());
	return &this->finalized;
}