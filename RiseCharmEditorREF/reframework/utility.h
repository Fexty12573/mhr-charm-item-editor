#pragma once

#include "API.hpp"
#include "API.hpp"
#include "API.hpp"
#include "API.hpp"

#include <memory>
#include <string>

class SystemString {
public:
    void* object_info;  // 0x0000
    uint32_t ref_count; // 0x0008
    int16_t _0;         // 0x000C
    char _1[2];         // 0x000E
    int32_t size;       // 0x0010
    wchar_t data[256];  // 0x0014
};

namespace utility {

template <class T = reframework::API::ManagedObject*, class... Args>
T call(reframework::API::ManagedObject* obj, std::string_view name, Args... args) {
    return obj->call<T>(name, reframework::API::get()->get_vm_context(), obj, args...);
}

template <class T = reframework::API::ManagedObject*, class... Args> T call(reframework::API::Method* method, Args... args) {
    return method->call<T>(reframework::API::get()->get_vm_context(), args...);
}

std::string narrow(std::wstring_view str);

std::wstring widen(std::string_view str);

template<class... Args> std::string str_call(reframework::API::ManagedObject* obj, std::string_view name, Args... args) {
    const SystemString* res = obj->call<SystemString*>(name, reframework::API::get()->get_vm_context(), obj, args...);
    return narrow(res->data);
}

template <class... Args> std::string str_call(reframework::API::Method* method, Args... args) {
    const SystemString* res = method->call<SystemString*>(reframework::API::get()->get_vm_context(), args...);
    return narrow(res->data);
}

template <class T = uint32_t> std::string get_enum_name(std::string_view name, T value) {
    const auto& api = reframework::API::get();
    const auto type = api->tdb()->find_type(name);
    const auto inst = type->create_instance();

    *inst->get_field<T>("value__") = value;
    return narrow(call<SystemString*>(inst, "ToString()")->data);
}

template <class T = uint32_t> std::string get_enum_name(reframework::API::TypeDefinition* type, T value) {
    const auto inst = type->create_instance();
    *inst->get_field<T>("value__") = value;

    return narrow(call<SystemString*>(inst, "ToString()")->data);
}

template <class T = uint32_t> std::string get_enum_name(reframework::API::ManagedObject* value) {
    return narrow(call<SystemString*>(value, "ToString()")->data);
}

reframework::API::ManagedObject* get_main_view();

reframework::API::ManagedObject* get_current_scene();

reframework::API::ManagedObject* get_primary_camera();

using Pattern = std::vector<std::int16_t>;

Pattern make_pattern(std::string_view pattern);

std::vector<void*> scanmem(const Pattern& pattern);

} // namespace utility
