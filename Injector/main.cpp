#include <Auth/User.h>
#include <Graphics/imfx.h>
#include <Files/Files.h>
#include <Registry/Registry.h>

#include <Fonts/FontRegistry.h>

#include "Pages/AboutPage.h"
#include "Pages/AccountPage.h"
#include "Pages/InjectPage.h"
#include "Pages/SettingsPage.h"
#include "Pages/VersionsPage.h"

//#include "Discord/DiscordRPCIntegration.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <imgui_custom.h>
#include <imgui_internal.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d2d1_3.h>
#include <dwmapi.h>
#include <tchar.h>

#include <url.hpp>
#include <xorstr.hpp>

#include <vector>

// Data
static ID3D11Device* d3d11Device = NULL;
static ID3D11DeviceContext* d3d11DeviceContext = NULL;
static IDXGISwapChain* pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

static ID2D1Factory3* d2dFactory = nullptr;
static ID2D1Device* d2dDevice = nullptr;
static ID2D1DeviceContext* d2dDeviceContext = nullptr;
static ID2D1Effect* blurEffect = nullptr;
static IDXGIDevice* dxgiDevice = nullptr;
static ID2D1DCRenderTarget* d2dRenderTarget = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
bool CreateDeviceD2D(HWND hWnd);
void CleanupDeviceD3D();
void CleanupDeviceD2D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//State stuff
static int64_t curPage = 0;
static size_t lastPage = 0;
static std::vector<struct Page*> allPages;
static float pageTransAnim = 0;

//Auth info
User* currentUser = nullptr;

// Main code
int main(int argc, char** argv)
{
    // Start Discord RPC
    //DiscordRPCIntegration* RPC = new DiscordRPCIntegration();
   // RPC->Start();

    //Check if we are authenticating
    std::string session_id = "";
    std::string session_expires = "";

    std::string launch_cmd = argv[0];
    if (argc > 1)
        launch_cmd = argv[1];
    printf(xorstr_("Launch CMD: %s"), launch_cmd.c_str());
    if (launch_cmd.find(xorstr_("injector://auth")) != std::string::npos) {
        printf(xorstr_("Parsing auth info..."));
        //It was launched via the protocol, lets try to parse out the datas
        Url uri = Url(launch_cmd);
        Url::Query query = uri.query();
        for (auto& pair : query) {
            std::string key = pair.key();
            if (key == xorstr_("session_id")) {
                session_id = pair.val();
            }
            if (key == xorstr_("expires")) {
                session_expires = pair.val();
            }
        }
        printf(xorstr_("Auth info parsed"));

        char tempPathBuf[MAX_PATH];
        GetTempPathA(MAX_PATH, tempPathBuf);
        std::string tempPath(tempPathBuf);
        HANDLE authFile = Files::OpenFile(tempPath + AUTH_DAT);
        Files::SaveFile(authFile, (char*)session_id.c_str(), session_id.size());
        Files::CloseFile(authFile);

        exit(0);
    }


    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { 
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL,
        NULL,
        NULL,
        NULL,
        _T(xorstr_("Nuvola Injector")),
        NULL
    };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(
        wc.lpszClassName,
        _T(xorstr_("Nuvola Injector")),
        WS_POPUP,
        100,
        100,
        720,
        480,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 2;
    }
    if (!CreateDeviceD2D(hwnd)) {
        CleanupDeviceD2D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 3;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    //Add window transparency key
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0xFF, 0, 0xFF), 0, LWA_COLORKEY);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(d3d11Device, d3d11DeviceContext);
    HDC hdc = GetDC(hwnd);

    size_t idx = 0;
    allPages.push_back(new AccountPage(idx++));
    allPages.push_back(new InjectPage(idx++));
    //allPages.push_back(new VersionsPage(idx++));
    allPages.push_back(new SettingsPage(idx++));
    allPages.push_back(new AboutPage(idx++));


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    FontRegistry::initAll();
    io.Fonts->Build();
    ImFont* uiFont = FontRegistry::getFont(FontRegistry::FontID::REGULAR);

#ifdef SSPROOF
    if (MessageBoxA(0, xorstr_("Some screenshare tools use explorer.exe's run history to detect cheats. Restarting explorer.exe can mitigate this."), xorstr_("Restart explorer.exe?"), MB_YESNO) == IDYES) {
        std::thread killThread([] {
            system(xorstr_("taskkill /f /im explorer.exe"));
            system(xorstr_("explorer.exe"));
        });
        killThread.detach();
    }
#endif

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(1, 0, 1, 1);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                //Cleanup RPC and exit
                //RPC->Shutdown();
                //delete RPC;
                done = true;
            }
        }
        if (done)
            break;

        if (currentUser == nullptr) {
            char tempPathBuf[MAX_PATH];
            GetTempPathA(MAX_PATH, tempPathBuf);
            std::string tempPath(tempPathBuf);
            std::string authFilePath = tempPath + AUTH_DAT;
            HANDLE authFile = Files::OpenFile(authFilePath);
            if (authFile != NULL && std::filesystem::exists(authFilePath)) {
                std::pair<char*, size_t> fileContent = Files::ReadFile(authFile);
                Files::CloseFile(authFile);
                if (fileContent.second > 0) {
                    std::string session_id = std::string(fileContent.first, fileContent.second);
                    try {
                        currentUser = new User(session_id);
                    }
                    catch (std::exception& ex)
                    {
                        /* If the user couldnt be found, delete the file and ask the user if they want to try again */
                        if (Files::DeleteFile(authFilePath))
                        {
                            MessageBoxA(nullptr, "An authentication error occoured. The injector will now close, and you should not see this message again.", "Authentication Error", MB_OK);
                        }
                        else
                        {
                            MessageBoxA(nullptr, "An authentication error occoured. Try running the injector as admin.", "Authentication Error", MB_OK);
                            std::string currentExe = Files::GetExecPath();
                            ShellExecuteA(0, "runas", currentExe.c_str(), 0, 0, SW_SHOW);
                        }
                        exit(0);
                    }
                }
                //Files::DeleteFile(authFilePath);
            }
        }

        RECT rcClient;
        GetClientRect(GetDesktopWindow(), &rcClient);

        //Create sizes for d2d
        D2D1_SIZE_U d2dScreenSize = D2D1::SizeU(
            rcClient.right - rcClient.left,
            rcClient.bottom - rcClient.top
        );
        D2D1_RECT_U d2dSourceRect = D2D1::RectU(
            rcClient.right - rcClient.left,
            rcClient.bottom - rcClient.top
        );
        D2D1_POINT_2U d2dDestPos = D2D1::Point2U(0, 0);

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        float dpi = GetDpiForWindow(hwnd);
        float darkTheme = 1.0f;

        IDXGISurface* backBuffer = nullptr;
        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        ImFX::NewFrame(d3d11Device, backBuffer, dpi);

        //Setup the window
        ImGui::PushItemFlag(ImGuiButtonFlags_AllowItemOverlap, true);
        ImGui::PushItemFlag(ImGuiHoveredFlags_AllowWhenOverlapped, true);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0 - darkTheme, 1.0 - darkTheme, 1.0 - darkTheme, 1));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0 - darkTheme, 1.0 - darkTheme, 1.0 - darkTheme, 0.95));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(darkTheme, darkTheme, darkTheme, 0.1));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0 - darkTheme, 1.0 - darkTheme, 1.0 - darkTheme, 0.8));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(darkTheme, darkTheme, darkTheme, 0.8));
        //ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0, 0.0, 0.0, 0.8));
        ImGui::PushStyleColor(ImGuiCol_WindowShadow, ImVec4(0.0, 0.0, 0.0, 0.0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 8.0f);
        ImGui::GetStyle().WindowShadowSize = 0;
        ImGui::GetStyle().WindowShadowOffsetDist = 0;
        ImGui::GetStyle().AntiAliasedFill = false;
        ImGui::GetStyle().AntiAliasedLines = false;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin(xorstr_("###INJECTORWINDOW"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
        ImGui::PushFont(uiFont);

        //Re-enable antialiasing for the window content
        //ImGui::GetStyle().AntiAliasedFill = true;
        //ImGui::GetStyle().AntiAliasedLines = true;

        //Set the default font scale for "Nuvola Injector" text
        ImGui::SetWindowFontScale(2.0f);

        //Get drawlist
        auto* drawList = ImGui::GetWindowDrawList();
        auto* bgDrawList = ImGui::GetBackgroundDrawList();
        //Get the imgui window pos
        Vector2<float> winPos = ImGui::GetWindowPos();
        Vector2<float> winSize = ImGui::GetWindowSize();
        Vector2<float> closePos = Vector2<float>(winSize.x - 20, 20) + winPos;
        ImU32 hitboxCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0, 0.0, 0.0, 0.8));
        ImU32 closeCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.95, 0.25, 0.25, 0.8));
        Vector2<float> hitboxMin = closePos + Vector2<float>(-8, -8);
        Vector2<float> hitboxMax = closePos + Vector2<float>(8, 8);
        if (ImGui::IsMouseHoveringRect(hitboxMin, hitboxMax)) {
            closeCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.7, 0.0, 0.0, 1.0));
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                done = true;
            }
        }
        //Draw close button
        drawList->AddCircleFilled(closePos, 10, closeCol);

        ImGui::Text(xorstr_("Nuvola Injector"));

        //Set font scale for page selection & content text
        ImGui::SetWindowFontScale(1.0f);

        for (IPage* page : allPages) {
            if (page->DrawSelection(winSize, winSize.x / 4, 1.0f, darkTheme, curPage)) {
                if (pageTransAnim != 0) {
                    pageTransAnim = 0;
                }
            }
        }

        pageTransAnim += io.DeltaTime * 4;
        if (pageTransAnim >= 1) {
            pageTransAnim = 1;
            lastPage = curPage;
        }

        ImGui::SetCursorPosY(0);

        size_t pageIdx = 0;
        for (IPage* page : allPages) {
            float pageOffY = (winSize.y * pageIdx) - (winSize.y * curPage);
            if (curPage > lastPage)
                pageOffY += (winSize.y * (curPage - lastPage)) * ImGui::easeInOutCubic(1 - pageTransAnim);
            else
                pageOffY -= (winSize.y * (lastPage - curPage)) * ImGui::easeInOutCubic(1 - pageTransAnim);
            page->DrawPage(winSize, winSize.x*(1.0f/4.0f), 1.0f, pageOffY, curPage);
            pageIdx++;
        }

        ImGui::PopFont();
        ImGui::End();
        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(13);
        ImGui::PopItemFlag();
        ImGui::PopItemFlag();
        ImFX::EndFrame();

        // Rendering
        ImGui::Render();

        /*ID3D11BlendState* g_pBlendStateNoBlend = NULL;
        D3D11_BLEND_DESC BlendState;
        ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC1));
        BlendState.RenderTarget[0].BlendEnable = FALSE;
        BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        d3d11Device->CreateBlendState(&BlendState, &g_pBlendStateNoBlend);

        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        UINT sampleMask = 0xffffffff;
        d3d11DeviceContext->OMSetBlendState(g_pBlendStateNoBlend, blendFactor, sampleMask);*/

        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        d3d11DeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        d3d11DeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        pSwapChain->Present(1, 0); // Present with vsync
        //pSwapChain->Present(0, 0); // Present without vsync

        //Release the blendstate
        //g_pBlendStateNoBlend->Release();
    }


    //Remove auth protocol
    if (Registry::DeleteAll(HKEY_CLASSES_ROOT, xorstr_("injector"))) {
        printf(xorstr_("Cleaned registry"));
    }
    else {
        printf(xorstr_("Didn't clean registry (might not have been dirty)"));
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    //ImGui::DestroyContext();

    CleanupDeviceD2D();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 280;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &d3d11Device, &featureLevel, &d3d11DeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

bool CreateDeviceD2D(HWND hwnd) {
    /*
    * SETUP DIRECT2D STUFF
    */
    //Create d2d & dxgi device(s)
    D2D1_FACTORY_OPTIONS options;
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
    D2D1_FACTORY_TYPE factoryType = D2D1_FACTORY_TYPE_SINGLE_THREADED;
    if (SUCCEEDED(D2D1CreateFactory(factoryType, __uuidof(ID2D1Factory3), &options, reinterpret_cast<void**>(&d2dFactory)))) {
        d3d11Device->QueryInterface<IDXGIDevice>(&dxgiDevice);
        if (dxgiDevice == nullptr) {
            return false;
        }
        d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
        dxgiDevice->Release();
        if (d2dDevice == nullptr) {
            return false;
        }
        d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
        if (d2dDeviceContext == nullptr) {
            return false;
        }

        //Create blur
        d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
        if (blurEffect == nullptr) {
            return false;
        }

        auto rt_props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), 0, 0, D2D1_RENDER_TARGET_USAGE_NONE);
        d2dFactory->CreateDCRenderTarget(&rt_props, &d2dRenderTarget);
        return true;
    }
    return false;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain) { pSwapChain->Release(); pSwapChain = NULL; }
    if (d3d11DeviceContext) { d3d11DeviceContext->Release(); d3d11DeviceContext = NULL; }
    if (d3d11Device) { d3d11Device->Release(); d3d11Device = NULL; }
}

void CleanupDeviceD2D()
{
    if (blurEffect) { blurEffect->Release(); blurEffect = NULL; }
    if (d2dDeviceContext) { d2dDeviceContext->Release(); d2dDeviceContext = NULL; }
    if (d2dDevice) { d2dDevice->Release(); d2dDevice = NULL; }
    if (d2dFactory) { d2dFactory->Release(); d2dFactory = NULL; }
    if (d2dRenderTarget) { d2dRenderTarget->Release(); d2dRenderTarget = NULL; };
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
static POINTS drag_start;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (d3d11Device != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        drag_start = MAKEPOINTS(lParam);
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON) {
            const auto points = MAKEPOINTS(lParam);
            RECT rect;

            GetWindowRect(hWnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            rect.left += points.x - drag_start.x;
            rect.top += points.y - drag_start.y;

            if (drag_start.x >= 0 &&
                drag_start.x <= width &&
                drag_start.y >= 0 &&
                drag_start.y <= height)
                SetWindowPos(hWnd, 0, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}