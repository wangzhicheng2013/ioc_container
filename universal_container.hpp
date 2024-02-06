#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <cassert>
template <typename DataType, typename... Args>
static auto new_object = [] (Args... args) { return new DataType(args...); };
class universal_container {
private:
    std::unordered_map<std::string, std::any>map_;
    size_t max_size_ = 100;
private:

    bool register_key(const std::string& key, std::any value) {
        if (map_.find(key) != map_.end()) {
            return false;
        }
        if (map_.size() >= max_size_) {
            return false;
        }
        map_[key] = value;
        return true;
    }
public:
    universal_container() = default;
    universal_container(const universal_container&) = delete;
    universal_container& operator = (const universal_container&) = delete;
public:
    template <class DataType, typename... Args>
    bool register_key(std::string key, Args...args) {
        auto* obj = new_object<DataType, Args...>(args...);
        if (nullptr == obj) {
            return false;
        }
        return register_key(key, obj);
    }

    template <class DataType>
    DataType* resolve_key(const std::string& key) {
        if (map_.find(key) == map_.end()) {
            return nullptr;
        }
        std::any& value = map_[key];
        try {
            return std::any_cast<DataType*>(value);
        }
        catch (const std::bad_any_cast& e) {
            std::cerr << "bad cast:" << e.what() << std::endl;
            return nullptr;
        }
    }  

    template <class DataType>
    void release_key(const std::string& key) {
        if (map_.find(key) == map_.end()) {
            return;
        }
        std::any& value = map_[key];
        try {
            delete (std::any_cast<DataType*>(value));
        }
        catch (const std::bad_any_cast& e) {
            std::cerr << "bad cast:" << e.what() << std::endl;
            assert(0);
        }
        map_.erase(key);
    }
};