#include "RiseDataEditor.h"

#include "imgui/font_robotomedium.hpp"
#include "imgui/InsanBold_Arabic.hpp"
#include "imgui/MYingHeiPRC_ZhJp.hpp"
#include "imgui/SeolSansRegular_Korean.hpp"
#include "reframework/API.hpp"
#include "reframework/utility.h"

#include <filesystem>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <MinHook.h>
#include <Windows.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <regex>
#include <vector>

using namespace reframework;

std::shared_ptr<RiseDataEditor> RiseDataEditor::get() {
    static auto instance = std::make_shared<RiseDataEditor>();
    return instance;
}

void RiseDataEditor::render_ui() {
    if (!m_initialized) {
        if (!initialize()) {
            if (ImGui::Begin("Charm Editor##init")) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Initializing...");
                ImGui::End();
            }

            return;
        }
    }

    ImGui::PushFont(m_active_font);

    if (m_data_manager) {
        m_eq_box = *m_data_manager->get_field<API::ManagedObject*>("_PlEquipBox");
        m_item_box = *m_data_manager->get_field<API::ManagedObject*>("_PlItemBox");

        m_inv_list = *m_eq_box->get_field<API::ManagedObject*>("_WeaponArmorInventoryList");
        if (!m_inv_list) {
            if (ImGui::Begin("Charm Editor##error")) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                ImGui::TextWrapped("Error Occurred. Cannot read Equipment Box");
                ImGui::PopStyleColor();
                ImGui::End();
            }

            ImGui::PopFont();
            return;
        }
    }

    if (API::get()->reframework()->is_drawing_ui()) {
        if (ImGui::Begin("Rise Data Editor")) {
            if (ImGui::CollapsingHeader(m_header_charm_editor.c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {7.0f, 0.2f});

                render_ui_charm_editor();

                ImGui::PopStyleVar();
            }
            if (ImGui::CollapsingHeader(m_header_player_editor.c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {7.0f, 0.2f});

                render_ui_player_editor();

                ImGui::PopStyleVar();
            }
            if (ImGui::CollapsingHeader(m_header_itembox_editor.c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {7.0f, 0.2f});

                render_ui_item_editor();

                ImGui::PopStyleVar();
            }
            if (ImGui::CollapsingHeader(m_header_loadout_editor.c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {7.0f, 0.2f});

                render_ui_loadout_editor();

                ImGui::PopStyleVar();
            }

            ImGui::End();
        }
    }

    ImGui::PopFont();
}

bool RiseDataEditor::initialize() {
    const auto& api = API::get();

    m_data_manager = api->get_managed_singleton("snow.data.DataManager");
    if (!m_data_manager) {
        return false;
    }

    m_eq_box = *m_data_manager->get_field<API::ManagedObject*>("_PlEquipBox");
    if (!m_eq_box) {
        return false;
    }

    m_inv_list = *m_eq_box->get_field<API::ManagedObject*>("_WeaponArmorInventoryList");
    if (!m_inv_list) {
        return false;
    }

    m_item_box = *m_data_manager->get_field<API::ManagedObject*>("_PlItemBox");
    if (!m_item_box) {
        return false;
    }

    m_progress_manager = api->get_managed_singleton("snow.progress.ProgressManager");
    if (!m_progress_manager) {
        return false;
    }

    m_data_shortcut = api->tdb()->find_type("snow.data.DataShortcut");
    if (!m_data_shortcut) {
        return false;
    }

    m_equip_data_manager = api->get_managed_singleton("snow.data.EquipDataManager");
    if (!m_equip_data_manager) {
        return false;
    }

    m_get_skill_name = m_data_shortcut->find_method("getName(snow.data.DataDef.PlEquipSkillId)");
    if (!m_get_skill_name) {
        return false;
    }

    m_get_item_name = m_data_shortcut->find_method("getName(snow.data.ContentsIdSystem.ItemId)");
    if (!m_get_item_name) {
        return false;
    }

    m_get_max_skill_level = m_data_shortcut->find_method("getMaxLv");
    if (!m_get_max_skill_level) {
        return false;
    }

    m_get_equip_name = m_data_shortcut->find_method("getName(snow.data.DataDef.PlArmorId)");
    if (!m_get_equip_name) {
        return false;
    }

    m_get_weapon_name = m_data_shortcut->find_method("getName(snow.data.ContentsIdSystem.WeaponId)");
    if (!m_get_weapon_name) {
        return false;
    }

    m_skill_id = api->tdb()->find_type("snow.data.DataDef.PlEquipSkillId");
    if (!m_skill_id) {
        return false;
    }

    if (const auto max = m_skill_id->find_field("Max")) {
        m_max_skill_id = max->get_data<uint8_t>(nullptr, true);
    } else {
        return false;
    }

    m_item_id = api->tdb()->find_type("snow.data.ContentsIdSystem.ItemId");
    if (!m_item_id) {
        return false;
    }

    if (const auto max = m_item_id->find_field("I_Normal_Max")) {
        m_max_item_id = max->get_data<uint32_t>(nullptr, true);
    } else {
        return false;
    }

    if (const auto invalid = m_item_id->find_field("I_Unclassified_None")) {
        m_invalid_item = invalid->get_data<uint32_t>(nullptr, true);
    } else {
        return false;
    }

    if (const auto min = m_item_id->find_field("I_Normal_0000")) {
        m_min_item_id = min->get_data<uint32_t>(nullptr, true);
    } else {
        return false;
    }

    if (const auto change_language = api->tdb()->find_method("snow.gui.MessageManager", "requestLoadFont")) {
        MH_Initialize();

        if (MH_CreateHook(change_language->get_function_raw(), change_language_hook, reinterpret_cast<void**>(&m_original_change_language)) != MH_OK) {
            OutputDebugStringA("[FEXTY] Failed to create hook");
            return false;
        }

        if (MH_EnableHook(change_language->get_function_raw()) != MH_OK) {
            OutputDebugStringA("[FEXTY] Failed to enable hook");
            return false;
        }
    }

    m_initialized = true;
    return m_initialized;
}

void RiseDataEditor::set_imgui_style() {
    ImGui::StyleColorsDark();

    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.WindowBorderSize = 2.0f;
    style.WindowPadding = ImVec2(2.0f, 0.0f);

    auto& colors = ImGui::GetStyle().Colors;
    // Window BG
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Navigatation highlight
    colors[ImGuiCol_NavHighlight] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

    // Checkbox
    colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.5505f, 0.551f, 1.0f);

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.211f, 0.210f, 0.25f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.8f, 0.805f, 0.81f, 1.0f};

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{0.2f, 0.205f, 0.21f, 0.0f};
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ResizeGripActive] = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.55f, 0.5505f, 0.551f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};

    const auto& fonts = ImGui::GetIO().Fonts;

    fonts->Clear();
    m_font_latin_cyrillic = fonts->AddFontFromMemoryCompressedTTF(
        RobotoMedium_compressed_data, 
        RobotoMedium_compressed_size, 
        16.0f, 
        nullptr, 
        m_glyph_ranges_latin_cyrillic
    );

    m_font_chinese_japanese = fonts->AddFontFromMemoryCompressedTTF(
        MYingHeiPRC_compressed_data, 
        MYingHeiPRC_compressed_size, 
        16.0f, 
        nullptr, 
        m_glyph_ranges_chinese_japanese
    );

    m_font_arabic = fonts->AddFontFromMemoryCompressedTTF(
        InsanBold_compressed_data,
        InsanBold_compressed_size,
        16.0f,
        nullptr,
        m_glyph_ranges_arabic
    );

    m_font_korean = fonts->AddFontFromMemoryCompressedTTF(
        SeolSansRegular_compressed_data,
        SeolSansRegular_compressed_size,
        16.0f,
        nullptr,
        m_glyph_ranges_korean
    );

    fonts->Build();

    m_active_font = m_font_latin_cyrillic;
    change_language(Language::ENG);
}

uint32_t* RiseDataEditor::slot_count_to_slots(const uint32_t counts[3], uint32_t slots[3]) {
    uint32_t _counts[3] = {counts[0], counts[1], counts[2]};

    for (auto i = 0; i < 3; i++) {
        if (_counts[2]) {
            --_counts[2];
            slots[i] = 3;
        } else if (_counts[1]) {
            --_counts[1];
            slots[i] = 2;
        } else if (_counts[0]) {
            --_counts[0];
            slots[i] = 1;
        } else {
            slots[i] = 0;
        }
    }

    return slots;
}

uint32_t* RiseDataEditor::slots_to_slot_count(const uint32_t slots[3], uint32_t counts[3]) {
    counts[0] = (slots[0] == 1) + (slots[1] == 1) + (slots[2] == 1);
    counts[1] = (slots[0] == 2) + (slots[1] == 2) + (slots[2] == 2);
    counts[2] = (slots[0] == 3) + (slots[1] == 3) + (slots[2] == 3);

    return counts;
}

API::ManagedObject* RiseDataEditor::get_empty_equipment_slot() const {
    if (!m_initialized) {
        return nullptr;
    }

    return utility::call(m_eq_box, "findEmptyInventory", 0);
}

void RiseDataEditor::sell_charm(const Charm& c) const {
    const auto entry = utility::call(m_inv_list, "get_Item", c.box_slot);
    utility::call(m_eq_box, "sellEquipment", entry);
}

void RiseDataEditor::render_ui_charm_editor() {
    auto get_charm = [](const API::ManagedObject* entry) -> Charm {
        Charm c{};

        const auto slotlist = *entry->get_field<API::ManagedObject*>("_TalismanDecoSlotNumList");
        uint32_t counts[3]{};

        counts[0] = utility::call<uint32_t>(slotlist, "get_Item", 1);
        counts[1] = utility::call<uint32_t>(slotlist, "get_Item", 2);
        counts[2] = utility::call<uint32_t>(slotlist, "get_Item", 3);

        slot_count_to_slots(counts, c.slots);

        const auto skilllist = *entry->get_field<API::ManagedObject*>("_TalismanSkillIdList");

        c.skills[0] = utility::call<uint32_t>(skilllist, "get_Item", 0);
        c.skills[1] = utility::call<uint32_t>(skilllist, "get_Item", 1);

        const auto levellist = *entry->get_field<API::ManagedObject*>("_TalismanSkillLvList");

        c.skill_levels[0] = utility::call<uint32_t>(levellist, "get_Item", 0);
        c.skill_levels[1] = utility::call<uint32_t>(levellist, "get_Item", 1);

        c.rarity = *entry->get_field<Rarity>("_IdVal");

        return c;
    };
    auto set_charm = [](const API::ManagedObject* entry, const Charm& charm) {
        // Make sure it actually gets saved as a charm
        *entry->get_field<EquipmentType>("_IdType") = EquipmentType::Talisman;

        const auto slotlist = *entry->get_field<API::ManagedObject*>("_TalismanDecoSlotNumList");

        uint32_t counts[3]{};
        slots_to_slot_count(charm.slots, counts);

        utility::call<>(slotlist, "set_Item", 1, counts[0]);
        utility::call<>(slotlist, "set_Item", 2, counts[1]);
        utility::call<>(slotlist, "set_Item", 3, counts[2]);

        const auto skilllist = *entry->get_field<API::ManagedObject*>("_TalismanSkillIdList");

        utility::call<>(skilllist, "set_Item", 0, charm.skills[0]);
        utility::call<>(skilllist, "set_Item", 1, charm.skills[1]);

        const auto levellist = *entry->get_field<API::ManagedObject*>("_TalismanSkillLvList");

        utility::call<>(levellist, "set_Item", 0, charm.skill_levels[0]);
        utility::call<>(levellist, "set_Item", 1, charm.skill_levels[1]);

        *entry->get_field<Rarity>("_IdVal") = charm.rarity;
    };
    auto get_rarity = [this](Rarity rarity) {
        return m_rarity_text[static_cast<uint32_t>(rarity) & ~0x10100000];
    };
    auto get_skillname = [this](uint32_t skill) {
        const auto str = utility::call<SystemString*>(m_get_skill_name, skill);
        return utility::narrow(str->data);
    };
    auto get_max_skill_level = [this](uint32_t skill) {
        return utility::call<int32_t>(m_get_max_skill_level, static_cast<uint8_t>(skill));
    };

    const auto count = utility::call<uint32_t>(m_inv_list, "get_Count");
    std::vector<Charm> charms;

    for (auto i = 0u; i < count; i++) {
        const auto entry = utility::call<API::ManagedObject*>(m_inv_list, "get_Item", i);
        const auto type = entry->get_field<EquipmentType>("_IdType");

        if (*type == EquipmentType::Talisman) {
            auto& c = charms.emplace_back(get_charm(entry));
            c.box_slot = i;
            c.type = *type;
        }
    }

    if (!charms.empty()) {
        if (m_selected_charm >= charms.size()) {
            m_selected_charm = static_cast<uint32_t>(charms.size() - 1);
        }

        ImGui::PushItemWidth(200.0f);
        const std::string preview = charms.empty() ? m_label_none : charms[m_selected_charm].get_name(get_skillname);
        if (ImGui::BeginCombo(m_label_charms.c_str(), preview.c_str())) {

            for (auto i = 0u; i < charms.size(); ++i) {
                const auto& c = charms[i];
                ImGui::PushID(static_cast<int>(i));

                if (ImGui::Selectable(c.get_name(get_skillname).c_str(), i == m_selected_charm))
                    m_selected_charm = i;

                ImGui::PopID();
            }

            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        auto& charm = charms[m_selected_charm];
        bool was_sold = false;

        ImGui::SameLine();
        if (ImGui::Button(m_button_add_empty_charm.c_str(), {ImGui::CalcTextSize(m_button_add_empty_charm.c_str()).x + 5, 0})) {
            const auto slot = get_empty_equipment_slot();
            if (!slot) {
                ImGui::OpenPopup(m_title_error.c_str());
            } else {
                set_charm(slot, {
                    .type = EquipmentType::Talisman,
                    .box_slot = 0,
                    .slots = {0, 0, 0},
                    .skills = {0, 0},
                    .skill_levels = {0, 0},
                    .rarity = Rarity::Rarity1
                });
            }
        }

        ImGui::SameLine();
        if (ImGui::Button(m_button_sell_charm.c_str(), {ImGui::CalcTextSize(m_button_sell_charm.c_str()).x + 5, 0})) {
            sell_charm(charm);
            was_sold = true;
        }

        ImGui::SameLine();
        if (ImGui::Button(m_button_export_charms.c_str(), {ImGui::CalcTextSize(m_button_export_charms.c_str()).x + 5, 0})) {
            const auto path = get_save_as_location();
            export_charms(path, charms);
        }

        ImGui::SameLine();
        if (ImGui::Button(m_button_import_charms.c_str(), {ImGui::CalcTextSize(m_button_import_charms.c_str()).x + 5, 0})) {
            const auto path = get_open_location();
            std::ranges::for_each(import_charms(path), [&](const auto& c) { set_charm(get_empty_equipment_slot(), c); });
        }

        if (ImGui::BeginPopupModal(m_title_error.c_str())) {
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), m_text_error_add_charm.c_str());

            if (ImGui::Button(m_button_ok.c_str())) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PushItemWidth(200.0f);
        if (ImGui::BeginCombo(m_label_rarity.c_str(), get_rarity(charm.rarity).c_str())) {
            for (auto i = 0; i < 11; ++i) {
                const auto r = static_cast<Rarity>(i | 0x10100000);

                if (ImGui::Selectable(get_rarity(r).c_str(), r == charm.rarity))
                    charm.rarity = r;
            }

            ImGui::EndCombo();
        }

        for (auto i = 0; i < 2; ++i) {
            ImGui::PushID(i + 10);
            auto& skill = charm.skills[i];

            if (ImGui::BeginCombo(fmt::format("{} {}", m_label_skill, i + 1).c_str(), get_skillname(skill).c_str())) {
                for (auto j = 0u; j < m_max_skill_id; ++j) {
                    if (ImGui::Selectable(get_skillname(j).c_str(), j == skill))
                        skill = j;
                }

                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::SliderInt(fmt::format("{}##level{}", m_label_level, i).c_str(), reinterpret_cast<int*>(&charm.skill_levels[i]), 0,
                get_max_skill_level(charm.skills[i]));

            ImGui::PopID();
        }

        ImGui::SliderInt((m_label_slot + " 1").c_str(), reinterpret_cast<int*>(&charm.slots[0]), 0, 3);
        ImGui::SliderInt((m_label_slot + " 2").c_str(), reinterpret_cast<int*>(&charm.slots[1]), 0, static_cast<int>(charm.slots[0]));
        ImGui::SliderInt((m_label_slot + " 3").c_str(), reinterpret_cast<int*>(&charm.slots[2]), 0, static_cast<int>(charm.slots[1]));

        ImGui::PopItemWidth();

        if (!was_sold) {
            set_charm(utility::call<API::ManagedObject*>(m_inv_list, "get_Item", charm.box_slot), charm);
        }
    } else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        ImGui::TextWrapped(m_text_waiting_for_game.c_str());
        ImGui::PopStyleColor();
    }
}

void RiseDataEditor::render_ui_player_editor() const {
    const auto money = utility::call(m_data_manager, "getHandMoney");
    const auto point = utility::call(m_data_manager, "getVillagePoint");

    constexpr int step = 100, step_fast = 10000;
    ImGui::InputScalar(m_label_zenny.c_str(), ImGuiDataType_S32, money->get_field<int32_t>("_Value"), &step, &step_fast);
    ImGui::InputScalar(m_label_points.c_str(), ImGuiDataType_U32, point->get_field<int32_t>("_Point"), &step, &step_fast);

    const auto savedata = utility::call(m_progress_manager, "get_SaveData");

    ImGui::TextColored({1, 0.3f, 0.3f, 1}, m_text_warning.c_str());
    ImGui::SameLine();
    ImGui::TextWrapped(m_text_warning_hr.c_str());

    ImGui::NewLine();
    ImGui::InputInt(m_label_hunter_rank.c_str(), savedata->get_field<int32_t>("_HunterRank"));
    ImGui::InputScalar(m_label_hr_points.c_str(), ImGuiDataType_U32, savedata->get_field<uint32_t>("_HunterRankPoint"), &step, &step_fast);
}

void RiseDataEditor::render_ui_item_editor() {
    auto get_itemname = [this](uint32_t id) {
        const auto str = utility::call<SystemString*>(m_get_item_name, id);
        return utility::narrow(str->data);
    };

    const auto items = *m_item_box->get_field<API::ManagedObject*>("_InventoryList");
    constexpr uint32_t step = 1, step_l = 10;
    bool clear_inv = false;

    if (ImGui::Button((m_button_add_all + " x1000").c_str())) {
        utility::call(m_data_manager, "addAllItemToItemBox1000");
    }

    ImGui::SameLine();
    if (ImGui::Button((m_button_add_all + " x9999").c_str())) {
        utility::call(m_data_manager, "addAllItemToItemBox9999");
    }

    ImGui::SameLine();
    if (ImGui::Button(m_button_clear_itembox.c_str())) {
        clear_inv = true;
    }

    ImGui::SameLine();
    if (ImGui::Button(m_button_clear_selected.c_str())) {
        for (auto i = 0u; i < m_selected_items.size(); ++i) {
            if (m_selected_items[i]) {
                utility::call(m_item_box, "clearInventory", i);
                m_selected_items[i] = false;
            }
        }
    }

    ImGui::InputText(m_label_itemlist_filter.c_str(), &m_item_filter);

    if (ImGui::Button(m_button_export_items.c_str())) {
        const auto path = get_save_as_location();
        if (!path.empty()) {
            export_items(path, items);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button(m_button_import_items.c_str())) {
        ImGui::OpenPopup((m_text_warning + "##overwrite items").c_str());
    }

    if (ImGui::BeginPopupModal((m_text_warning + "##overwrite items").c_str())) {
        ImGui::TextWrapped(m_text_warning_overwrite.c_str());

        if (ImGui::Button(m_button_yes.c_str())) {
            ImGui::CloseCurrentPopup();

            const auto path = get_open_location();
            import_items(path, items);
        }

        ImGui::SameLine();
        if (ImGui::Button(m_button_no.c_str())) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::BeginChild("##item list");

    if (ImGui::BeginTable(m_label_items.c_str(), 5, 
        ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_SizingFixedFit, {}, 880.0f)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("##item selected", ImGuiTableColumnFlags_NoResize);
        ImGui::TableSetupColumn(m_label_index.c_str(), 0, 40.0f);
        ImGui::TableSetupColumn(m_label_id.c_str(), 0, 200.0f);
        ImGui::TableSetupColumn(m_label_item.c_str(), 0, 400.0f);
        ImGui::TableSetupColumn(m_label_amount.c_str(), 0, 200.0f);
        ImGui::TableHeadersRow();

        static uint32_t last_clicked = 0;

        const auto count = utility::call<uint32_t>(items, "get_Count");
        if (count > m_selected_items.size()) {
            m_selected_items.resize(count);
        }

        for (auto i = 0u; i < count; ++i) {
            if (clear_inv) {
                utility::call(m_item_box, "clearInventory", i);
            }

            const auto item = utility::call(items, "get_Item", i);
            const auto data = *item->get_field<API::ManagedObject*>("_ItemCount");

            const auto id = data->get_field<uint32_t>("_Id");
            const auto amt = data->get_field<int32_t>("_Num");

            ImGui::PushID(static_cast<int>(i));
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            bool selected = m_selected_items[i];
            if (ImGui::Checkbox("##is selected", &selected)) {
                const auto& io = ImGui::GetIO();

                if (io.KeyShift) { // Allow multiselect with shiftclick
                    m_selected_items[i] = true;
                    m_selected_items[last_clicked] = true;

                    if (i > last_clicked) {
                        for (auto j = last_clicked; j < i; ++j) {
                            m_selected_items[j] = true;
                        }
                    } else if (i < last_clicked) {
                        for (auto j = i; j < last_clicked; ++j) {
                            m_selected_items[j] = true;
                        }
                    }
                } else if (io.KeyAlt) { // Allow multi-deselect with altclick
                    m_selected_items[i] = false;
                    m_selected_items[last_clicked] = false;

                    if (i > last_clicked) {
                        for (auto j = last_clicked; j < i; ++j) {
                            m_selected_items[j] = false;
                        }
                    } else if (i < last_clicked) {
                        for (auto j = i; j < last_clicked; ++j) {
                            m_selected_items[j] = false;
                        }
                    }
                } else {
                    m_selected_items[i] = selected;
                }

                last_clicked = i;
            }
            

            ImGui::TableNextColumn();

            ImGui::Text("%d", i);

            ImGui::TableNextColumn();

            ImGui::PushItemWidth(200.0f);
            ImGui::InputScalar("##item id", ImGuiDataType_U32, id, &step, &step_l);
            ImGui::PopItemWidth();

            ImGui::TableNextColumn();

            ImGui::PushItemWidth(400.0f);

            if (ImGui::BeginCombo("##item name", get_itemname(*id).c_str())) {
                for (auto j = m_min_item_id; j < m_max_item_id; ++j) {
                    const bool is_selected = i == *id;
                    
                    const auto name = get_itemname(j);
                    if (m_item_filter.empty() || name.contains(m_item_filter)) {
                        if (ImGui::Selectable(get_itemname(j).c_str(), is_selected)) {
                            *id = j;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            ImGui::TableNextColumn();

            ImGui::PushItemWidth(200.0f);
            ImGui::InputInt("##amount", amt);
            *amt = std::clamp(*amt, 0, 9999);

            ImGui::PopItemWidth();
            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    ImGui::EndChild();
}

void RiseDataEditor::render_ui_loadout_editor() const {
    const auto set_list = *m_equip_data_manager->get_field<API::ManagedObject*>("_PlEquipMySetList");
    const API::Method* get_armor_id = nullptr;
    
    const auto count = utility::call<uint32_t>(set_list, "get_Count");
    for (auto i = 0u; i < count; ++i) {
        const auto loadout = utility::call(set_list, "get_Item", i);
        if (!get_armor_id) {
            get_armor_id = loadout->get_type_definition()->find_method("getArmorId");
        }

        if (*loadout->get_field<bool>("_IsUsing")) {
            ImGui::PushID(static_cast<int32_t>(i));

            if (ImGui::TreeNode(utility::str_call(loadout, "get_Name").c_str())) {
                if (ImGui::Button(m_button_delete.c_str())) {
                    utility::call(loadout, "clear");
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }
    }
}

std::string RiseDataEditor::get_save_as_location() const {
    OPENFILENAMEA ofn{};
    char path[0xFFF]{};

    ofn.lStructSize = sizeof ofn;
    ofn.Flags = OFN_EXPLORER | OFN_DONTADDTORECENT;
    ofn.lpstrFilter = "JSON File\0*.json\0";
    ofn.nMaxFile = std::size(path);
    ofn.lpstrFile = path;

    if (GetSaveFileNameA(&ofn)) {
        return path;
    }

    return {};
}

std::string RiseDataEditor::get_open_location() const {
    OPENFILENAMEA ofn{};
    char path[0xFFF]{};

    ofn.lStructSize = sizeof ofn;
    ofn.Flags = OFN_EXPLORER | OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
    ofn.lpstrFilter = "JSON File\0*.json\0";
    ofn.nMaxFile = std::size(path);
    ofn.lpstrFile = path;

    if (GetOpenFileNameA(&ofn)) {
        return path;
    }

    return {};
}

void RiseDataEditor::change_language(Language language) {
    m_active_language = language;

    std::string filename = fmt::format("./reframework/plugins/rise_charm_editor/language/{}_ed_ref.json", m_language_file_map.at(language));
    OutputDebugStringA(fmt::format("[FEXTY] Attempting to open {}", filename).c_str());

    nlohmann::json file = nlohmann::json::object();
    try {
        
        std::ifstream stream(filename);
        stream >> file;
    } catch (const std::exception& e) {
        API::get()->log_info(e.what());
        OutputDebugStringA(fmt::format("[FEXTY] {}", e.what()).c_str());
        return;
    }

    const auto& headers = file["Headers"];
    const auto& charm_editor = file["Charm Editor"];
    const auto& player_editor = file["Player Editor"];
    const auto& itembox_editor = file["Itembox Editor"];
    const auto& loadout_editor = file["Loadout Editor"];

    m_header_charm_editor = headers.value("m_header_charm_editor", "STR_NOT_FOUND");
    m_header_player_editor = headers.value("m_header_player_editor", "STR_NOT_FOUND");
    m_header_itembox_editor = headers.value("m_header_itembox_editor", "STR_NOT_FOUND");
    m_header_loadout_editor = headers.value("m_header_loadout_editor", "STR_NOT_FOUND");

    m_label_charms = charm_editor.value("m_label_charms", "STR_NOT_FOUND");
    m_label_none = charm_editor.value("m_label_none", "STR_NOT_FOUND");
    m_button_add_empty_charm = charm_editor.value("m_button_add_empty_charm", "STR_NOT_FOUND");
    m_title_error = charm_editor.value("m_title_error", "STR_NOT_FOUND") + "##error_add_charm";
    m_text_error_add_charm = charm_editor.value("m_text_error_add_charm", "STR_NOT_FOUND");
    m_button_sell_charm = charm_editor.value("m_button_sell_charm", "STR_NOT_FOUND");
    m_button_export_charms = charm_editor.value("m_button_export_charms", "STR_NOT_FOUND");
    m_button_import_charms = charm_editor.value("m_button_import_charms", "STR_NOT_FOUND");
    m_button_ok = charm_editor.value("m_button_ok", "STR_NOT_FOUND");
    m_label_rarity = charm_editor.value("m_label_rarity", "STR_NOT_FOUND");
    m_label_skill = charm_editor.value("m_label_skill", "STR_NOT_FOUND");
    m_label_level = charm_editor.value("m_label_level", "STR_NOT_FOUND");
    m_label_slot = charm_editor.value("m_label_slot", "STR_NOT_FOUND");
    m_text_waiting_for_game = charm_editor.value("m_text_waiting_for_game", "STR_NOT_FOUND");

    m_label_zenny = player_editor.value("m_label_zenny", "STR_NOT_FOUND");
    m_label_points = player_editor.value("m_label_points", "STR_NOT_FOUND");
    m_text_warning = player_editor.value("m_text_warning", "STR_NOT_FOUND") + ":";
    m_text_warning_hr = player_editor.value("m_text_warning_hr", "STR_NOT_FOUND");
    m_label_hunter_rank = player_editor.value("m_label_hunter_rank", "STR_NOT_FOUND");
    m_label_hr_points = player_editor.value("m_label_hr_points", "STR_NOT_FOUND");

    m_button_add_all = itembox_editor.value("m_button_add_all", "STR_NOT_FOUND");
    m_button_clear_itembox = itembox_editor.value("m_button_clear_itembox", "STR_NOT_FOUND");
    m_button_clear_selected = itembox_editor.value("m_button_clear_selected", "STR_NOT_FOUND");
    m_button_export_items = itembox_editor.value("m_button_export_items", "STR_NOT_FOUND");
    m_button_import_items = itembox_editor.value("m_button_import_items", "STR_NOT_FOUND");
    m_text_warning_overwrite = itembox_editor.value("m_text_warning_overwrite", "STR_NOT_FOUND");
    m_button_yes = itembox_editor.value("m_button_yes", "STR_NOT_FOUND");
    m_button_no = itembox_editor.value("m_button_no", "STR_NOT_FOUND");
    m_label_itemlist_filter = itembox_editor.value("m_label_itemlist_filter", "STR_NOT_FOUND");
    m_label_items = itembox_editor.value("m_label_items", "STR_NOT_FOUND");
    m_label_index = itembox_editor.value("m_label_index", "STR_NOT_FOUND");
    m_label_id = itembox_editor.value("m_label_id", "STR_NOT_FOUND");
    m_label_item = itembox_editor.value("m_label_item", "STR_NOT_FOUND");
    m_label_amount = itembox_editor.value("m_label_amount", "STR_NOT_FOUND");

    m_button_delete = loadout_editor.value("m_button_delete", "STR_NOT_FOUND");

    m_rarity_text[0] = m_label_rarity + " 1";
    m_rarity_text[1] = m_label_rarity + " 2";
    m_rarity_text[2] = m_label_rarity + " 3";
    m_rarity_text[3] = m_label_rarity + " 4";
    m_rarity_text[4] = m_label_rarity + " 5";
    m_rarity_text[5] = m_label_rarity + " 6";
    m_rarity_text[6] = m_label_rarity + " 7";
    m_rarity_text[7] = m_label_rarity + " 4 (Novice)";
    m_rarity_text[8] = m_label_rarity + " 3 (Kinship)";
    m_rarity_text[9] = m_label_rarity + R"( 12 (???))";
    m_rarity_text[10] = m_label_rarity + " 2 (Legacy)";
}

void RiseDataEditor::export_charms(const std::string& to, const std::vector<Charm>& charms) {
    nlohmann::json j = nlohmann::json::array();

    for (const auto& charm : charms) {
        j.push_back({
            {"Rarity", static_cast<uint32_t>(charm.rarity)},
            {"Skills", {charm.skills[0], charm.skills[1]}},
            {"SkillLevels", {charm.skill_levels[0], charm.skill_levels[1]}},
            {"Slots", {charm.slots[0], charm.slots[1], charm.slots[2]}}
        });
    }

    std::ofstream(to) << j.dump();
}

void RiseDataEditor::export_items(const std::string& to, API::ManagedObject* itembox) const {
    nlohmann::json j = nlohmann::json::array();
    const auto count = utility::call<uint32_t>(itembox, "get_Count");

    for (auto i = 0u; i < count; ++i) {
        const auto item = utility::call(itembox, "get_Item", i);
        const auto data = *item->get_field<API::ManagedObject*>("_ItemCount");

        const auto id = *data->get_field<uint32_t>("_Id");
        const auto amt = *data->get_field<int32_t>("_Num");

        j.push_back({{"id", id}, {"amount", amt}});
    }

    std::ofstream(to) << j.dump();
}

std::vector<Charm> RiseDataEditor::import_charms(const std::string& from) {
    nlohmann::json j = nlohmann::json::array();
    std::vector<Charm> charms;

    try {
        std::ifstream(from) >> j;
    } catch (const std::exception& e) {
        API::get()->log_info(e.what());
        return charms;
    }

    for (const auto& charm : j) {
        charms.push_back({.type = EquipmentType::Talisman,
            .box_slot = 0,
            .slots = {charm["Slots"][0], charm["Slots"][1]},
            .skills = {charm["Skills"][0], charm["Skills"][1]},
            .skill_levels = {charm["SkillLevels"][0], charm["SkillLevels"][0]},
            .rarity = charm.value("Rarity", Rarity::Rarity1)
        });
    }

    return charms;
}

void RiseDataEditor::import_items(const std::string& from, reframework::API::ManagedObject* itembox) const {
    nlohmann::json j = nlohmann::json::array();

    try {
        std::ifstream(from) >> j;
    } catch (const std::exception& e) {
        API::get()->log_info(e.what());
        return;
    }

    const auto count = utility::call<uint32_t>(itembox, "get_Count");

    for (auto i = 0u; i < count; ++i) {
        const auto item = utility::call(itembox, "get_Item", i);
        const auto data = *item->get_field<API::ManagedObject*>("_ItemCount");

        const auto id = data->get_field<uint32_t>("_Id");
        const auto amt = data->get_field<int32_t>("_Num");

        *id = j[i]["id"];
        *amt = j[i]["amount"];
    }
}

void RiseDataEditor::change_language_hook(API::VMContext* vmctx, API::ManagedObject* this_, Language language, bool fade, void* _) {
    const auto rise = get();

    switch (language) {
    case Language::JPN:
        rise->m_active_font = rise->m_font_chinese_japanese;
        break;
    case Language::ENG:
    case Language::FRE:
    case Language::ITA:
    case Language::GER:
    case Language::SPA:
    case Language::RUS:
    case Language::POL:
    case Language::POR:
        rise->m_active_font = rise->m_font_latin_cyrillic;
        break;
    case Language::KOR:
        rise->m_active_font = rise->m_font_korean;
        break;
    case Language::ZHT:
    case Language::ZHS:
        rise->m_active_font = rise->m_font_chinese_japanese;
        break;
    case Language::ARA:
        rise->m_active_font = rise->m_font_arabic;
        break;
    default:
        rise->m_active_font = rise->m_font_latin_cyrillic;
        break;
    }

    rise->change_language(language);

    return rise->m_original_change_language(vmctx, this_, language, fade, _);
}

void RiseDataEditor::action_hook(void* vmctx, API::ManagedObject* this_, void* action_arg_, uint8_t category, uint16_t action) {
    const auto rise = get();

    const auto cat = utility::get_enum_name("snow.enemy.EnemyDef.EmActCategory", category);
    const std::string_view tname = this_->get_type_info()->get_name();
    const std::regex r("snow.enemy.aifsm.Em([0-9]{3})_[0-9]{2}ActionSet([a-zA-Z]+)");
    std::cmatch match;

    if (std::regex_match(tname.data(), match, r)) {
        const auto enum_name = fmt::format("snow.enemy.em{}.Em{}Define.Em{}No", match[1].str(), match[1].str(), match[2].str());
        OutputDebugStringA(fmt::format("[FEXTY] {}", enum_name).c_str());
        const std::string action_name = utility::get_enum_name(enum_name, action);
        OutputDebugStringA(fmt::format("[FEXTY] Action: {}::{}", cat, action_name).c_str());
    }

    return rise->m_original_action_func(vmctx, this_, action_arg_, category, action);
}

std::string Charm::get_name(const std::function<std::string(uint32_t)>& skill_name_getter) const {
    return fmt::format(
        "{} {} | {} {} | {}-{}-{}", 
        skill_name_getter(skills[0]), skill_levels[0],
        skill_name_getter(skills[1]), skill_levels[1],
        slots[0], slots[1], slots[2]
    );
}
