#include "IniFile.h"

#include <Windows.h>

std::string IniFile::read(std::string_view key, std::string_view default_value) const {
    std::string s(MAX_PATH, '\0');
    GetPrivateProfileStringA("lang", key.data(), default_value.data(), s.data(), s.size(), m_path.string().c_str());
    return s;
}

unsigned IniFile::read_int(std::string_view key, int default_value) const {
    return GetPrivateProfileIntA("lang", key.data(), default_value, m_path.string().c_str());
}

void IniFile::write(std::string_view key, std::string_view value) const {
    WritePrivateProfileStringA("lang", key.data(), value.data(), m_path.string().c_str());
}

void IniFile::write_int(std::string_view key, int value) const {
    WritePrivateProfileStringA("lang", key.data(), std::to_string(value).c_str(), m_path.string().c_str());
}

std::string IniFile::operator[](std::string_view key) const {
    return read(key);
}
