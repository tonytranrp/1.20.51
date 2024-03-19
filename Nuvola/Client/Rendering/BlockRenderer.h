#pragma once

#include "VertexBuffer.h"

#include <Math/Math.h>

#include <Effects.h>
#include <GeometricPrimitive.h>
#include <PrimitiveBatch.h>
#include <SimpleMath.h>
#include <VertexTypes.h>

#include <mutex>

struct BlockInstance
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Color color;
};

struct PerFrameConstants
{
	DirectX::SimpleMath::Matrix worldViewProj;
};

class BlockRenderer
{
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	std::vector<DirectX::VertexPosition> cubeVertices;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* instanceBuffer = nullptr;
	std::vector<BlockInstance> instances;
	bool instancesDirty = false;
	ID3D11Buffer* perFrameBuffer = nullptr;
	std::mutex dataLock;

	static std::vector<DirectX::VertexPosition> generateCubeVertices(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max);
public:
	BlockRenderer(ID3D11DeviceContext* pContext);

	void reset();
	void updateInstanceBuffer();
	void computeAndCache(Vector3<int> blockPos, ColorF color);
	void updateTransform(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void draw();
};