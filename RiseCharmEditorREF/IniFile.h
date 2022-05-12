#pragma once

#include <filesystem>
#include <string>

class IniFile {
public:
    explicit IniFile(const std::filesystem::path& path) : m_path(absolute(path)) {}

    [[nodiscard]] std::string read(std::string_view key, std::string_view default_value = "") const;
    [[nodiscard]] unsigned read_int(std::string_view key, int default_value = 0) const;
    void write(std::string_view key, std::string_view value) const;
    void write_int(std::string_view key, int value) const;

    std::string operator[](std::string_view key) const;

private:
    std::filesystem::path m_path;
};
