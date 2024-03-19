#pragma once

#include "Vertex.h"

#include <Math/Math.h>

class IRenderer
{
public:
	IRenderer() = default;
	virtual ~IRenderer() = default;
	virtual void reset() {};

	virtual bool worldToScreen(const Vector3<float>& in, Vector2<float>& out) const { return false; };
	virtual void begin(VertexCommand) {};
	virtual void vertex(Vector2<float> pos, ColorF color) {};
	virtual void vertex(Vector3<float> pos, ColorF color) {};
	virtual void end(VertexCommand cmd, ColorF color, float thickness = 2.0f) {};

	virtual const std::vector<Vertex>* finalize() { return nullptr; };
};