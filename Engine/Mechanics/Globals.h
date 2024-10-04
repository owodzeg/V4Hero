#ifndef GLOBALS_H
#define GLOBALS_H

#include <any>
#include <unordered_map>
#include <string>
#include <stdexcept>

class Globals {
public:
    // Store an object in the global container
    template<typename T>
    void set(int key, T value) {
        data_[key] = std::any(value);
    }

    // Retrieve an object from the global container
    template<typename T>
    T get(int key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            throw std::runtime_error("Key not found");
        }

        // Check the stored type before casting
        if (it->second.type() != typeid(T)) {
            throw std::runtime_error("Type mismatch: Requested type does not match stored type");
        }

        return std::any_cast<T>(it->second);
    }

    // Check the type of the object before retrieving
    const std::type_info& getType(int key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            throw std::runtime_error("Key not found");
        }

        return it->second.type();  // Returns the type_info of the stored object
    }

private:
    std::unordered_map<int, std::any> data_;  // Global storage
};

#endif // GLOBALS_H