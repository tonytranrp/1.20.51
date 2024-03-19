#include "GeometryBuilder.h"

GeometryBuilder::GeometryBuilder(IRenderer* renderer)
{
	this->renderer = renderer;
}

GeometryBuilder::~GeometryBuilder()
{

}

void GeometryBuilder::buildAABB(AABB<float> box, ColorF color, bool filled, float thickness)
{
	Vector3<float> dxposmin = box.firstCorner;
	Vector3<float> dxposmax = box.secondCorner;

    VertexCommand beginCmd = filled ? VertexCommand::BEGIN_POLY_FILLED : VertexCommand::BEGIN_POLY;
    VertexCommand endCmd = filled ? VertexCommand::END_POLY_FILLED : VertexCommand::END_POLY;

    this->renderer->begin(beginCmd);

	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmax.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmin.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmax.x, dxposmin.y, dxposmax.z}, color);
	this->renderer->vertex(Vector3<float>{dxposmin.x, dxposmin.y, dxposmax.z}, color);

    this->renderer->end(endCmd, color, thickness);
}

void GeometryBuilder::buildBlock(const Vector3<float>& blockPos, ColorF color, bool filled, float thickness)
{
    AABB<float> blockBox = {
        blockPos,
        blockPos + Vector3<float>(1, 1, 1)
    };
    this->buildAABB(blockBox, color, filled, thickness);
}