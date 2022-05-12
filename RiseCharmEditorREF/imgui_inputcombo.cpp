#include "imgui_inputcombo.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace ImGui {

static float CalcMaxPopupHeightFromItemCount(int items_count) {
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

static bool OpenPopupOnItemClick_(const char* str_id, ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonLeft) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const int mouse_button = (flags & ImGuiPopupFlags_MouseButtonMask_);
    if (IsMouseReleased(mouse_button) && IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        const ImGuiID id = str_id ? window->GetID(str_id) : g.LastItemData.ID; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
        IM_ASSERT(id != 0);   // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
        OpenPopupEx(id, flags);
        return true;
    }

    return false;
}

bool BeginInputCombo(const char* label, std::string& buffer, const char* preview, ImGuiComboFlags flags) {
    PushID(label);

    auto& ctx = *GImGui;

    if (!IsPopupOpen("##open_combo")) {
        buffer = preview;
    }

    InputText(label, &buffer);
    if (OpenPopupOnItemClick_("##open_combo")) {
        buffer = preview;
    }

    const auto pos = GetItemRectMin();
    const auto size = GetItemRectSize();
    float x_size = 0.0f;

    if (!(flags & ImGuiComboFlags_NoArrowButton)) {
        SameLine(0, 0);
        if (ArrowButton("##open_combo", ImGuiDir_Down)) {
            OpenPopup("combobox");
            buffer = preview;
        }

        const auto size_popup = GetItemRectSize();
        x_size = size_popup.x;
    }

    if ((flags & ImGuiComboFlags_HeightMask_) == 0) {
        flags |= ImGuiComboFlags_HeightRegular;
    }

    int max_items = -1;
    if (flags & ImGuiComboFlags_HeightRegular) {
        max_items = 8;
    } else if (flags & ImGuiComboFlags_HeightSmall) {
        max_items = 4;
    } else if (flags & ImGuiComboFlags_HeightLarge) {
        max_items = 20;
    }

    SetNextWindowSizeConstraints({size.x + x_size, 0.0f}, {FLT_MAX, CalcMaxPopupHeightFromItemCount(max_items)});

    if (!IsPopupOpen("##open_combo")) {
        ctx.NextWindowData.ClearFlags();
        PopID();
        return false;
    }

    SetNextWindowPos({pos.x, pos.y + size.y});

    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ctx.Style.FramePadding.x, ctx.Style.WindowPadding.y));
    Begin("##open_combo", nullptr, window_flags);
    PopStyleVar();
    PopID();

    return true;
}

void EndInputCombo() {
    return EndPopup();
}

}
