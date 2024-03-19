#pragma once

#include "IRenderer.h"

#undef min
#undef max
#include <mutex>

class WorldRenderer : public IRenderer
{
	std::vector<Vertex> data;
	std::vector<Vertex> finalized;
	std::mutex dataMutex;
public:
	WorldRenderer();
	virtual ~WorldRenderer();
	virtual void reset() override;

	virtual bool worldToScreen(const Vector3<float>& in, Vector2<float>& out) const override;
	virtual void begin(VertexCommand) override;
	virtual void vertex(Vector2<float> pos, ColorF color) override;
	virtual void vertex(Vector3<float> pos, ColorF color) override;
	virtual void end(VertexCommand cmd, ColorF color, float thickness = 2.0f) override;

	virtual const std::vector<Vertex>* finalize();
};