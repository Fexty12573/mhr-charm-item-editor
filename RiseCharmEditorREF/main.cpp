#include <Windows.h>

#include <mutex>

#include "plugin.h"
#include "RiseDataEditor.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

#include "rendering/d3d11.hpp"
#include "rendering/d3d12.hpp"

using namespace reframework;
using REGenericFunction = void*(*)(...);
std::mutex g_mutex;

void initialize() {
    if (g_initialized) {
        return;
    }

    if (!IMGUI_CHECKVERSION()) {
        OutputDebugStringA("[FEXTY] stupid imgui error");
        return;
    }
    ImGui::CreateContext();
    //ImGui::GetIO().IniFilename = "rise_charm_editor.ini";

    const auto renderer_data = API::get()->param()->renderer_data;

    DXGI_SWAP_CHAIN_DESC desc{};
    const auto swapchain = static_cast<IDXGISwapChain*>(renderer_data->swapchain);
    swapchain->GetDesc(&desc);

    if (!ImGui_ImplWin32_Init(desc.OutputWindow)) {
        return;
    }

    if (renderer_data->renderer_type == REFRAMEWORK_RENDERER_D3D11) {
        if (!g_d3d11.initialize()) {
            return;
        }
    } else if (renderer_data->renderer_type == REFRAMEWORK_RENDERER_D3D12) {
        if (!g_d3d12.initialize()) {
            return;
        }
    }

    RiseDataEditor::get()->set_imgui_style();

    g_initialized = true;
}

void render() {
    RiseDataEditor::get()->render_ui();
}

void get_some_item_names() {
    std::lock_guard l{g_mutex};
    if (!g_initialized) {
        initialize();
    }
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    render();

    ImGui::EndFrame();
    ImGui::Render();
}

void on_present() {
    std::lock_guard l{g_mutex};
    if (!g_initialized) {
        initialize();
    }

    const auto renderer = API::get()->param()->renderer_data;

    if (renderer->renderer_type == REFRAMEWORK_RENDERER_D3D11) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        render();

        ImGui::EndFrame();
        ImGui::Render();

        g_d3d11.render_imgui();
    } else if (renderer->renderer_type == REFRAMEWORK_RENDERER_D3D12) {
        const auto command_queue = static_cast<ID3D12CommandQueue*>(renderer->command_queue);

        if (command_queue == nullptr) {
            return;
        }

        ImGui_ImplDX12_NewFrame();
        g_d3d12.render_imgui();
    }
}

void on_device_reset() {
    const auto renderer_data = API::get()->param()->renderer_data;

    if (renderer_data->renderer_type == REFRAMEWORK_RENDERER_D3D11) {
        ImGui_ImplDX11_Shutdown();
        g_d3d11 = {};
    }

    if (renderer_data->renderer_type == REFRAMEWORK_RENDERER_D3D12) {
        ImGui_ImplDX12_Shutdown();
        g_d3d12 = {};
    }

    g_initialized = false;
}

bool on_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

    return !ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard;
}

extern "C" {

RE_EXPORT void reframework_plugin_required_version(REFrameworkPluginVersion* version) {
    version->major = REFRAMEWORK_PLUGIN_VERSION_MAJOR;
    version->minor = REFRAMEWORK_PLUGIN_VERSION_MINOR;
    version->patch = REFRAMEWORK_PLUGIN_VERSION_PATCH;

    version->game_name = "MHRISE";
}

RE_EXPORT bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam* param) {
    OutputDebugString(TEXT("[FEXTY] REFramework Initialize"));
    API::initialize(param);
    ImGui::CreateContext();

    const auto funcs = param->functions;
    funcs->on_message(reinterpret_cast<REFOnMessageCb>(on_message));
    funcs->on_device_reset(on_device_reset);
    funcs->on_present(on_present);
    funcs->on_pre_application_entry("BeginRendering", get_some_item_names);

    return true;
}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        OutputDebugString(TEXT("[FEXTY] DllMain ATTACH"));
    }

    return TRUE;
}
