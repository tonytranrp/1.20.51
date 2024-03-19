#include "BlockRenderer.h"

#include <BlockInstanceVS.inc>
#include <DefaultPS.inc>

#include <BufferHelpers.h>
#include <GeometricPrimitive.h>
#include <SimpleMath.h>

std::vector<DirectX::VertexPosition> BlockRenderer::generateCubeVertices(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
{
	std::vector<DirectX::VertexPosition> vertices;
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, max.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, min.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ max.x, min.y, max.z });
	vertices.emplace_back(DirectX::SimpleMath::Vector3{ min.x, min.y, max.z });
	return vertices;
}


BlockRenderer::BlockRenderer(ID3D11DeviceContext* pContext)
{
	//Reset the block renderer and store the context and device
	this->reset();
	this->pContext = pContext;
	pContext->GetDevice(&pDevice);

	//Make the element descriptions for the instanced shader
	D3D11_INPUT_ELEMENT_DESC elementDescs[] =
	{
		//Vertex data
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

		//Instance data
		{"InstPos", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"InstCol", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};

	//Create the input layout for the shader
	HRESULT hresult = pDevice->CreateInputLayout(elementDescs, ARRAYSIZE(elementDescs), g_vertex_main, sizeof(g_vertex_main), &this->inputLayout);
	assert(SUCCEEDED(hresult));

	//Create the shader instances
	hresult = pDevice->CreateVertexShader(g_vertex_main, sizeof(g_vertex_main), nullptr, &this->vertexShader);
	assert(SUCCEEDED(hresult));

	hresult = pDevice->CreatePixelShader(g_pixel_main, sizeof(g_pixel_main), nullptr, &this->pixelShader);
	assert(SUCCEEDED(hresult));

	//Generate the cube vertices that the instanced rendering will use
	constexpr DirectX::SimpleMath::Vector3 dxposmin(0.0f, 0.0f, 0.0f);
	constexpr DirectX::SimpleMath::Vector3 dxposmax(1.0f, 1.0f, 1.0f);
	this->cubeVertices = this->generateCubeVertices(dxposmin, dxposmax);

	//Create the desc for the buffer
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(DirectX::VertexPosition) * cubeVertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	//Create the sub resource info
	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = cubeVertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the buffer, the data is copied from the sub resource
	hresult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
	assert(SUCCEEDED(hresult));
};

void BlockRenderer::reset()
{
	this->instances.resize(0);
	this->instancesDirty = true;
};

void BlockRenderer::updateInstanceBuffer() {
	if (this->instances.empty())
		return;

	//Release the previous instance if it exists
	if(this->instanceBuffer)
	{
		this->instanceBuffer->Release();
		this->instanceBuffer = nullptr;
	}

	//Create the new buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(BlockInstance) * this->instances.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = this->instances.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &this->instanceBuffer);
	assert(SUCCEEDED(hr));

	this->instancesDirty = false;
}

void BlockRenderer::computeAndCache(Vector3<int> blockPos, ColorF color)
{
	DirectX::SimpleMath::Vector3 blockPosDX(blockPos.x, blockPos.y, blockPos.z);
	DirectX::SimpleMath::Vector4 colorDX(color.x, color.y, color.z, color.w);
	this->instances.emplace_back(blockPosDX, colorDX);
	this->instancesDirty = true;
}

void BlockRenderer::draw()
{
	if (this->instances.empty())
		return;

	if(this->instancesDirty)
	{
		this->updateInstanceBuffer();
	}

	ID3D11RenderTargetView* currentRTV = nullptr;
	pContext->OMGetRenderTargets(1, &currentRTV, nullptr);


	pContext->IASetInputLayout(this->inputLayout);

	//Set the vertex shader
	pContext->VSSetShader(this->vertexShader, nullptr, 0);
	pContext->VSSetConstantBuffers(0, 1, &this->perFrameBuffer);

	//Set the pixel shader
	pContext->PSSetShader(this->pixelShader, nullptr, 0);

	//Bind the vertex buffer
	UINT stride = sizeof(DirectX::VertexPosition);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);

	//Bind the instance buffer
	stride = sizeof(BlockInstance);
	offset = 0;
	pContext->IASetVertexBuffers(1, 1, &this->instanceBuffer, &stride, &offset);

	const UINT vertexCount = this->cubeVertices.size();
	const UINT instanceCount = this->instances.size();

	pContext->DrawInstanced(vertexCount, instanceCount, 0, 0);
}

void BlockRenderer::updateTransform(DirectX::SimpleMath::Matrix world,
									DirectX::SimpleMath::Matrix view,
									DirectX::SimpleMath::Matrix proj) {
	if(this->perFrameBuffer == nullptr)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(PerFrameConstants); //Needs to be a multiple of 16, and sizeof(PerFrameConstants) is 204. 208 is the next multiple. //sizeof(PerFrameConstants);
		static_assert(sizeof(PerFrameConstants) % 16 == 0, "Must change the ByteWidth to be able to fix the struct at a multiple of 16!");
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		HRESULT hr = pDevice->CreateBuffer(&bufferDesc, nullptr, &this->perFrameBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create the constants struct to write to
	PerFrameConstants data;

	D3D11_MAPPED_SUBRESOURCE subresource;
	subresource.pData = &data;
	subresource.DepthPitch = 0;
	subresource.RowPitch = 0;

	//Lock the buffer for writing
	pContext->Map(this->perFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);

	//Write the new constants
	auto worldView = XMMatrixMultiply(world, view);
	data.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));

	//Copy the data
	std::memcpy(subresource.pData, &data, sizeof(data));

	//Unlock the buffer
	pContext->Unmap(this->perFrameBuffer, 0);
}
