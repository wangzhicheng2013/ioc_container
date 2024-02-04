#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <any>
class ioc_container {
private:
    std::unordered_map<std::string, std::any>creator_map_;
public:
    ioc_container() = default;
    ioc_container(const ioc_container&) = delete;
    ioc_container& operator = (const ioc_container&) = delete;
public:
    template <class T>
    void register_type(std::string key) {
        using I = T *;
        std::function<I()> fun = constructor<I, T>::invoke;
        register_type(key, fun);
    }
    template <class I, class T, typename... Ts>
    void register_type(std::string key) {
        std::function<I*(Ts...)> fun = constructor<I*, T, Ts...>::invoke;
        register_type(key, fun);
    }
    template <class I>
    I* resolve(std::string key) {
        if (creator_map_.find(key) == creator_map_.end()) {
            return nullptr;
        }
        std::any resolver = creator_map_[key];
        return (std::any_cast<std::function<I*()>>(resolver))();
    }
    template <class I>
    std::unique_ptr<I>resolve_uniqueptr(std::string key) {
        auto pointer = resolve<I>(key);
        return std::unique_ptr<I>(pointer);
    }
private:
    template <typename I, typename T, typename... Ts>
    struct constructor {
        static I invoke(Ts...args) {
            return I(new T(args...));
        }
    };
    void register_type(std::string key, std::any fun) {
        if (creator_map_.find(key) != creator_map_.end()) {
            return;
        }
        creator_map_[key] = fun;
    }
};