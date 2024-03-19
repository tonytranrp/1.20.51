#pragma once

#include "imgui.h"

#include <optional>

class ID3D11Device;
class IDXGISurface;
struct ID2D1Bitmap1;

namespace ImFX {
    enum class FXMode
    {
        QUALITY,
        NORMAL,
        PERFORMANCE
    };

    void SetFXMode(FXMode mode);
    FXMode GetFXMode();
    bool NewFrame(ID3D11Device* d3d11Device, IDXGISurface* backBuffer, float dpi);
    bool EndFrame();
    bool CleanupFX();
    bool Begin(ImDrawList* drawList, bool dontCopy = false);
    bool End(bool composite = false);
    bool PushLayer(std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool PopLayer();
    bool AddBlur(float strength, std::optional<ImVec4> clipRect = std::optional<ImVec4>(), float rounding = ImGui::GetStyle().FrameRounding);
    bool AddColor(ImVec4 color, float rounding, std::optional<ImVec4> clipRect);
    bool AddColor(ImVec4 color, std::optional<ImVec4> rounding, std::optional<ImVec4> clipRect);
    bool AddColor(ImVec4 color, std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool AddColor(ImGuiCol color, std::optional<ImVec4> clipRect = std::optional<ImVec4>());
    bool AddShadow(float strength = 8.0f, ImVec4 color = ImVec4(0,0,0,1), std::optional<Vector4<float>> clipRect = std::optional<Vector4<float>>());
    bool AddDisplacementMap();
    ID2D1Bitmap1* UploadImageBase64(const std::string& imageDataB64);
    ID2D1Bitmap1* UploadImage(const std::vector<uint8_t>& imageData, std::optional<Vector2<int>> imageSize = {});
    bool DrawImage(ID2D1Bitmap1* image, std::optional<Vector4<float>> clipRect, float rounding = ImGui::GetStyle().FrameRounding);
    bool DrawImage(ID2D1Bitmap1* image, std::optional<Vector4<float>> clipRect, std::optional<Vector4<float>> rounding);
    void DeleteImage(ID2D1Bitmap1* image);
}