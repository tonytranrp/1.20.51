#include "PresentHook.h"

#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/GuiData.h"
#include "../../Bridge/LocalPlayer.h"
#include "../../Rendering/RenderController.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/RenderEvent.h"
#include "../../Features/Modules/ModuleManager.h"
#include "../../Features/Theme/ThemeManager.h"
#include "../../Rendering/BlockRenderer.h"
#include "../../Rendering/RenderTexture.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_3.h>

#include <CommonStates.h>
#include <DirectXColors.h>
#include <DirectXHelpers.h>
#include <DirectXMath.h>
#include <Effects.h>
#include <GeometricPrimitive.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>

#include <ShlObj_core.h>
#include <wrl/client.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Foundation.h>
#include <corewindow.h>

#include <kiero.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <imgui_custom.h>
#include <Fonts/FontRegistry.h>

#include <Graphics/imfx.h>
#include <mutex>
#include <queue>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace winrt::Windows;
using namespace Microsoft::WRL;

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

uint64_t buffersCounts = -1;
FrameContext* frameContext = nullptr;

extern ID3D12CommandQueue* d3d12CommandQueue;

ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandAllocator* allocator = nullptr;
std::unique_ptr<DirectX::CommonStates> states = nullptr;
std::unique_ptr<DirectX::BasicEffect> effect = nullptr;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch = nullptr;
std::unique_ptr<RenderTexture> worldRenderTexture;
ComPtr<ID3D11InputLayout> inputLayout;
ID3D11BlendState* blendState = nullptr;
std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;

bool imguiInit = false;
HWND window;
std::thread::id presentThreadId;
struct FrameTransforms
{
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	Vector3<float> origin;
};
std::queue<FrameTransforms> frameTransforms;
int transformDelay = 3;

static std::vector<Vertex> lastFrameData;
std::unique_ptr<BlockRenderer> nuvolaBlockRenderer;

extern bool exiting;
extern std::mutex initMutex;

auto __fastcall PresentHook::presentCallback(IDXGISwapChain3* pSwapChain, int syncInterval, int flags) -> HRESULT {
	if (exiting)
		return S_OK;

	//Check mutex
	initMutex.lock();
	initMutex.unlock();

	ID3D11Device* d3d11Device = nullptr;
	ID3D12Device* d3d12Device = nullptr;

	//goto exit;

	if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		float dpi = GetDpiForWindow(window);

		ID3D11DeviceContext* pContext = nullptr;
		d3d11Device->GetImmediateContext(&pContext);

		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		IDXGISurface* dxgiBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

		ID3D11RenderTargetView* mainRenderTargetView = nullptr;
		if (pBackBuffer != nullptr)
		{
			d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		}

		if (!imguiInit) {
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsLight();
			io.IniFilename = NULL;

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, pContext);

			FontRegistry::initAll();

			states = std::make_unique<DirectX::CommonStates>(d3d11Device);

			effect = std::make_unique<DirectX::BasicEffect>(d3d11Device);
			effect->SetVertexColorEnabled(true);

			void const* shaderByteCode;
			size_t byteCodeLength;

			effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			if (FAILED(
				d3d11Device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
					DirectX::VertexPositionColor::InputElementCount,
					shaderByteCode, byteCodeLength,
					inputLayout.ReleaseAndGetAddressOf())))
			{
				goto exit;
			}

			batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(pContext);
			nuvolaBlockRenderer = std::make_unique<BlockRenderer>(pContext);
			DXGI_SURFACE_DESC desc;
			dxgiBackBuffer->GetDesc(&desc);
			worldRenderTexture = std::make_unique<RenderTexture>(desc.Format);
			worldRenderTexture->SetDevice(d3d11Device);

			spriteBatch = std::make_unique<DirectX::SpriteBatch>(pContext);

			imguiInit = true;
		};

        //pContext->ClearRenderTargetView(mainRenderTargetView, DirectX::Colors::CornflowerBlue);


        /*batch->Begin();

        DirectX::VertexPositionColor v1(DirectX::SimpleMath::Vector3(400.f, 150.f, 0.f), DirectX::Colors::Yellow);
        DirectX::VertexPositionColor v2(DirectX::SimpleMath::Vector3(600.f, 450.f, 0.f), DirectX::Colors::Yellow);
        DirectX::VertexPositionColor v3(DirectX::SimpleMath::Vector3(200.f, 450.f, 0.f), DirectX::Colors::Yellow);
        batch->DrawTriangle(v1, v2, v3);

        batch->End();*/

		presentThreadId = std::this_thread::get_id();
		ImFX::NewFrame(d3d11Device, dxgiBackBuffer, dpi);

		pBackBuffer->Release();

		//Get module manager before newframe in case it hasn't loaded
		//modules may want to load fonts for ui in the constructors
		const auto moduleMgr = systems->getModuleManager();

		//Do ImGui rendering stuff
		ClientInstance* ci = Bridge::getClientInstance();
		if (!ci) {
			goto exit;
		}
		const GuiData* gd = ci->getGuiData();
		if (!gd) {
			goto exit;
		}
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = gd->resolution;

		ImGuiContext* g = ImGui::GetCurrentContext();
		g->HoveredIdAllowOverlap = true;

        systems->getThemeManager()->nextFrame();
		//#ifdef _DEBUG
		//		ThemeManager::getInstance()->reloadAllThemes();
		//#endif


        auto displaySize = ImGui::GetIO().DisplaySize;
        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(displaySize.x);
        viewport.Height = static_cast<float>(displaySize.y);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
		RECT displayRect;
		displayRect.left = 0;
		displayRect.top = 0;
		displayRect.right = displaySize.x;
		displayRect.bottom = displaySize.y;
		worldRenderTexture->SetWindow(displayRect);
		float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		auto worldRenderTarget = worldRenderTexture->GetRenderTargetView();
		pContext->OMSetRenderTargets(1, &worldRenderTarget, nullptr);
		pContext->ClearRenderTargetView(worldRenderTarget, clearColor);

        //effect->SetProjection(DirectX::XMMatrixOrthographicRH(displaySize.x, displaySize.y, 0.0f, 1.0f));
		FrameTransforms transform;
		while(frameTransforms.size() > transformDelay)
		{
			transform = frameTransforms.front();
			frameTransforms.pop();
		}
		DirectX::SimpleMath::Vector3 originDx(-transform.origin.x, -transform.origin.y, -transform.origin.z);
        DirectX::SimpleMath::Matrix world = reinterpret_cast<DirectX::SimpleMath::Matrix &&>(transform.worldMatrix);
		world.Translation(originDx);
        DirectX::SimpleMath::Matrix view = reinterpret_cast<DirectX::SimpleMath::Matrix &&>(transform.viewMatrix);
        DirectX::SimpleMath::Matrix proj = reinterpret_cast<DirectX::SimpleMath::Matrix &&>(transform.projMatrix);

        pContext->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
        pContext->OMSetDepthStencilState(states->DepthNone(), 0);
        pContext->RSSetState(states->CullNone());
        pContext->RSSetViewports(1, &viewport);

		nuvolaBlockRenderer->updateTransform(world, view, proj);
		nuvolaBlockRenderer->draw();

		effect->SetMatrices(world, view, proj);
        effect->Apply(pContext);
        pContext->IASetInputLayout(inputLayout.Get());

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		pContext->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);

		RenderController* controller = RenderController::getInstance();
		WorldRenderer& renderer = controller->getWorldRenderer();

		auto event = nes::make_holder<RenderEvent>(RenderTime::WORLD);
		controller->resetWorldRenderer();
		auto dispatcher = systems->getEventDispatcher();
		dispatcher->trigger(event);

		static std::vector<DirectX::VertexPositionColor> immediateVertices;
		batch->Begin();
        for (auto& vert : *renderer.finalize())
        {
            switch (vert.cmd)
            {
            case VertexCommand::BEGIN_LINE:
            case VertexCommand::BEGIN_POLY:
            case VertexCommand::BEGIN_POLY_FILLED:
                break;
            case VertexCommand::VERTEX: {
                DirectX::XMFLOAT3 pos(vert.pos.x, vert.pos.y, vert.pos.z);
                DirectX::XMFLOAT4 color(vert.color.x, vert.color.y, vert.color.z, vert.color.w);
                DirectX::VertexPositionColor vertex(pos, color);
                immediateVertices.push_back(vertex);
                break;
            }
			case VertexCommand::VERTEX2D: {
				DirectX::XMVECTOR projectedPoint = DirectX::XMVectorSet(vert.pos.x, vert.pos.y, 0.0f, 1.0f);
				DirectX::XMVECTOR worldPosition = DirectX::XMVector3Unproject(projectedPoint, 0, 0, displaySize.x, displaySize.y, 0.0f, 1.0f, proj, view, world);

				DirectX::SimpleMath::Color color(vert.color.x, vert.color.y, vert.color.z, vert.color.w);
				DirectX::VertexPositionColor vertex(worldPosition, color);

				immediateVertices.push_back(vertex);
				break;
			}
            case VertexCommand::END_LINE:
                batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, immediateVertices.data(), immediateVertices.size());
                immediateVertices.clear();
                break;
            case VertexCommand::END_POLY:
            case VertexCommand::END_POLY_FILLED:
                batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, immediateVertices.data(), immediateVertices.size());
                immediateVertices.clear();
                break;
            default:
                break;
            }
        }
		batch->End();
		controller->endFrame();

		DirectX::SimpleMath::Vector4 blockRenderColor(1.0f, 1.0f, 1.0f, 1.0f);
		spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Immediate, states->NonPremultiplied());
		spriteBatch->Draw(worldRenderTexture->GetShaderResourceView(), displayRect, blockRenderColor);
		spriteBatch->End();

		/*effect->SetMatrices(world, view, proj);
		pContext->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		pContext->OMSetDepthStencilState(states->DepthDefault(), 0);
		pContext->RSSetState(states->CullNone());
		pContext->RSSetViewports(1, &viewport);
		effect->Apply(pContext);*/

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImFont* font = FontRegistry::getFont(FontRegistry::FontID::REGULAR);
		if (font != nullptr)
			ImGui::PushFont(font);

		auto ure = nes::make_holder<RenderEvent>(RenderTime::UNDERLAY);
		dispatcher->trigger(ure);
		auto mre = nes::make_holder<RenderEvent>(RenderTime::MENU);
		dispatcher->trigger(mre);
		auto ore = nes::make_holder<RenderEvent>(RenderTime::OVERLAY);
		dispatcher->trigger(ore);

		if (font != nullptr)
			ImGui::PopFont();

		ImGui::EndFrame();
		ImGui::Render();

		//Finish ImGui draw
		if (mainRenderTargetView != nullptr)
			pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(drawData);
		ImFX::EndFrame();

		pContext->Flush();
		//dxgiBackBuffer->Release();
		pContext->Release();
		if (mainRenderTargetView)
			mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device))))
	{
		//Clear any buffers, some nvidia drivers block threads waiting for buffers to delete I guess
		pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		//Remove the device to trigger d3d11 fallback
		static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
	}

exit:
	if (funcOriginal == 0)
		return S_OK;
	return PLH::FnCast(funcOriginal, &presentCallback)(pSwapChain, syncInterval, flags);
}

PresentHook::PresentHook() : IPatch("DXGI::Present") {
}

bool PresentHook::apply() {
	UI::Core::CoreWindow cw = ApplicationModel::Core::CoreApplication::MainView().CoreWindow();
	winrt::com_ptr<ICoreWindowInterop> interop;
	winrt::check_hresult(winrt::get_unknown(cw)->QueryInterface(interop.put()));
	winrt::check_hresult(interop->get_WindowHandle(&window));

	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
	return kiero::bind(index, (void**)&funcOriginal, presentCallback) == kiero::Status::Success;
}

bool PresentHook::manualCleanup() {
	cleaned = true;
	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
	kiero::unbind(index);
	return true;
}