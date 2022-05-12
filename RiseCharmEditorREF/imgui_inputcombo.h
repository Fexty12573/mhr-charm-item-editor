#pragma once

#include <imgui.h>
#include <string>

namespace ImGui {

bool BeginInputCombo(const char* label, std::string& buffer, const char* preview, ImGuiComboFlags flags = 0);
void EndInputCombo();

}
