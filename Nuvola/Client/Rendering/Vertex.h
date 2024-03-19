#pragma once

#include <Math/Math.h>

enum class VertexCommand
{
	BEGIN_LINE,
	BEGIN_POLY,
	BEGIN_POLY_FILLED,
	END_LINE,
	END_POLY,
	END_POLY_FILLED,
	VERTEX,
	VERTEX2D
};

struct Vertex
{
	VertexCommand cmd = VertexCommand::VERTEX;
	Vector3<float> pos;
	ColorF color = ColorF(1,1,1,1);
	float thickness = 2.0f;
};