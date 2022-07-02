#pragma once

#include "plugin.h"
#include "reframework/API.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

enum class EquipmentType : int32_t {
    Empty,
    Weapon,
    Armor,
    Talisman,
    LvBuffCage // ???
};

enum class Rarity : uint32_t {
    Rarity1 = 0x10100000,
    Rarity2 = 0x10100001,
    Rarity3 = 0x10100002,
    Rarity4 = 0x10100003,
    Rarity5 = 0x10100004,
    Rarity6 = 0x10100005,
    Rarity7 = 0x10100006,
    Rarity4_Novice = 0x10100007,
    Rarity3_Kinship = 0x10100008,
    Rarity2_Veteran = 0x10100009,
    Rarity2_Legacy = 0x1010000A,
    Rarity8 = 0x1010000B,
    Rarity9 = 0x1010000C,
    Rarity10 = 0x1010000D
};

struct Charm {
    EquipmentType type;
    uint32_t box_slot;
    uint32_t slots[3];
    uint32_t skills[2];
    uint32_t skill_levels[2];
    Rarity rarity;

    [[nodiscard]] std::string get_name(const std::function<std::string(uint32_t)>& skill_name_getter) const;
};

enum class Language : int32_t {
    JPN = 0,
    ENG = 1,
    FRE = 2,
    ITA = 3,
    GER = 4,
    SPA = 5,
    RUS = 6,
    POL = 7,
    POR = 10,
    KOR = 11,
    ZHT = 12,
    ZHS = 13,
    ARA = 21
};

struct ImFont;

class RiseDataEditor {
public:
    RiseDataEditor() = default;

    static std::shared_ptr<RiseDataEditor> get();
    // Stolen directly from REFramework
    void set_imgui_style();

    void render_ui();

private:
    bool initialize();

    static uint32_t* slot_count_to_slots(const uint32_t counts[3], uint32_t slots[3]);
    static uint32_t* slots_to_slot_count(const uint32_t slots[3], uint32_t counts[3]);

    [[nodiscard]] reframework::API::ManagedObject* get_empty_equipment_slot() const;
    void sell_charm(const Charm& c) const;

    void render_ui_charm_editor();
    void render_ui_player_editor() const;
    void render_ui_item_editor();
    void render_ui_loadout_editor() const;

    [[nodiscard]] std::string get_save_as_location() const;
    [[nodiscard]] std::string get_open_location() const;
    void change_language(Language language);

    void export_charms(const std::string& to, const std::vector<Charm>& charms);
    void export_items(const std::string& to, reframework::API::ManagedObject* itembox) const;
    std::vector<Charm> import_charms(const std::string& from);
    void import_items(const std::string& from, reframework::API::ManagedObject* itembox) const;

    static void change_language_hook(
        reframework::API::VMContext* vmctx, reframework::API::ManagedObject* this_, Language language, bool fade, void* _);
    static void action_hook(void* vmctx, reframework::API::ManagedObject* this_, void* action_arg_, uint8_t category, uint16_t action);

private:
    reframework::API::ManagedObject* m_data_manager{};
    reframework::API::ManagedObject* m_eq_box{};
    reframework::API::ManagedObject* m_item_box{};
    reframework::API::ManagedObject* m_inv_list{};
    reframework::API::ManagedObject* m_progress_manager{};
    reframework::API::ManagedObject* m_equip_data_manager{};
    reframework::API::TypeDefinition* m_data_shortcut{};
    reframework::API::Method* m_get_skill_name{};
    reframework::API::Method* m_get_max_skill_level{};
    reframework::API::Method* m_get_item_name{};
    reframework::API::Method* m_get_action_category_name{};
    reframework::API::Method* m_get_equip_name{};
    reframework::API::Method* m_get_weapon_name{};
    reframework::API::TypeDefinition* m_skill_id{};
    reframework::API::TypeDefinition* m_item_id{};
    uint32_t m_max_skill_id = 0;
    uint32_t m_min_item_id = 0;
    uint32_t m_max_item_id = 0;
    uint32_t m_invalid_item = 0;
    bool m_initialized = false;

    uint32_t m_selected_charm = 0;
    std::string m_item_filter{};
    std::vector<bool> m_selected_items;
    void* m_action_func = reinterpret_cast<void*>(0x140c087b0);
    void (*m_original_action_func)(void*, void*, void*, uint8_t, uint16_t){};
    void (*m_original_change_language)(reframework::API::VMContext*, reframework::API::ManagedObject*, Language, bool, void*){};

    ImFont* m_font_latin_cyrillic{};
    ImFont* m_font_chinese_japanese{};
    ImFont* m_font_arabic{};
    ImFont* m_font_korean{};
    ImFont* m_active_font{};

    Language m_active_language{};

    std::unordered_map<Language, const char*> m_language_file_map = {
        {Language::JPN, "jpn"},
        {Language::ENG, "eng"},
        {Language::FRE, "fre"},
        {Language::ITA, "ita"},
        {Language::GER, "ger"},
        {Language::SPA, "spa"},
        {Language::RUS, "rus"},
        {Language::POL, "pol"},
        {Language::POR, "por"},
        {Language::KOR, "kor"},
        {Language::ZHT, "zht"},
        {Language::ZHS, "zhs"},
        {Language::ARA, "ara"}
    };

    static constexpr const char* m_em_act_category[] = {
        "Normal",
        "Move",
        "Fly",
        "Moguri",
        "Attack",
        "Terrain",
        "Marionette",
        "Catch",
        "Grapple",
        "Damage",
        "Die",
        "Revival",
        "Max"
    };
    static constexpr unsigned short m_glyph_ranges_latin_cyrillic[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0100, 0x017E, // Polish
        0x0384, 0x03CE, // Greek
        0x0400, 0x0475, // Cyrillic
        0x2000, 0x206F, // General Punctuation
        0
    };
    static constexpr unsigned short m_glyph_ranges_chinese_japanese[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0x4e00, 0x9FAF, // CJK Ideograms
        0xFF00, 0xFFEF, // Half-width characters
        0,
    };
    static constexpr unsigned short m_glyph_ranges_arabic[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0600, 0x06FF, // Arabic
        0x2000, 0x206F, // General Punctuation
        0
    };
    static constexpr unsigned short m_glyph_ranges_korean[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3130, 0x318F, // Hangul Compatibility
        0xAC00, 0xD7AF, // Hangul (Korean)
        0
    };

    // UI Strings
#pragma region CollapsingHeaders
    std::string m_header_charm_editor;
    std::string m_header_player_editor;
    std::string m_header_itembox_editor;
    std::string m_header_loadout_editor;
#pragma endregion
#pragma region CharmEditor
    std::string m_label_charms;
    std::string m_label_none;
    std::string m_button_add_empty_charm;
    std::string m_title_error;
    std::string m_text_error_add_charm;
    std::string m_button_sell_charm;
    std::string m_button_ok;
    std::array<std::string, 14> m_rarity_text;
    std::string m_label_rarity;
    std::string m_label_skill;
    std::string m_label_level;
    std::string m_label_slot;
    std::string m_text_waiting_for_game;
    std::string m_button_export_charms;
    std::string m_button_import_charms;
#pragma endregion
#pragma region PlayerEditor
    std::string m_label_zenny;
    std::string m_label_points;
    std::string m_text_warning;
    std::string m_text_warning_hr;
    std::string m_label_hunter_rank;
    std::string m_label_hr_points;
#pragma endregion
#pragma region ItemboxEditor
    std::string m_button_add_all;
    std::string m_button_clear_itembox;
    std::string m_button_clear_selected;
    std::string m_button_delete_illegal;
    std::string m_button_export_items;
    std::string m_button_import_items;
    std::string m_text_warning_overwrite;
    std::string m_button_yes;
    std::string m_button_no;
    std::string m_label_itemlist_filter;
    std::string m_label_items;
    std::string m_label_index;
    std::string m_label_id;
    std::string m_label_item;
    std::string m_label_amount;
#pragma endregion
#pragma region LoadoutEditor
    std::string m_button_delete;
#pragma endregion

};
