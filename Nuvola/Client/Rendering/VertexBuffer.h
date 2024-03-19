#pragma once

#include <array>
#include <d3d11.h>
#include <VertexTypes.h>

template<typename vertex_t = DirectX::VertexPositionColor>
class VertexBuffer
{
	ID3D11Device* pDevice;
	ID3D11Buffer* buffer = nullptr;
	size_t vertexCount = 0;
public:
	template<size_t vertex_count>
	VertexBuffer(ID3D11Device* pDevice, std::array<vertex_t, vertex_count> vertices)
	{
		this->pDevice = pDevice;
		vertexCount = vertex_count;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = vertex_count * sizeof(vertex_t);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(vertex_t);

		D3D11_SUBRESOURCE_DATA initial;
		initial.pSysMem = vertices.data();
		initial.SysMemPitch = 0;
		initial.SysMemSlicePitch = 0;

		pDevice->CreateBuffer(&desc, &initial, &buffer);
	}

	const ID3D11Buffer* data() const
	{
		return this->buffer;
	}

	const size_t size() const
	{
		return this->vertexCount;
	}
};