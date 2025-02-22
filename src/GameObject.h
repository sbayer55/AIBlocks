#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <typeindex>
#include <concepts>
#include <unordered_map>
#include <ranges>
#include <sstream>

#include "Transform.h"

// Forward declaration
class GameObject;

class Component {
public:
    virtual ~Component() = default;

    virtual void update(float deltaTime) {
    }

    GameObject *gameObject{nullptr}; // Owner game object
};

class GameObject {
public:
    explicit GameObject(std::string name = "GameObject") :
        name_(std::move(name)),
        transform_(TRANSFORM_ZERO)
    {}

    ~GameObject() = default;

    template<typename T, typename... Args> requires std::derived_from<T, Component>
    T *add_component(Args &&... args) {
        // Throw if already has a component of that type
        if (const auto type_index = std::type_index(typeid(T)); components_.contains(type_index)) {
            throw std::runtime_error("Component of type " + std::string(typeid(T).name()) + " already exists");
        }

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T *componentPtr = component.get();
        componentPtr->gameObject = this;
        components_[std::type_index(typeid(T))] = std::move(component);
        return componentPtr;
    }

    [[nodiscard]]
    std::string get_component_names() const {
        const std::vector<std::string> names =  components_
            | std::views::keys
            | std::views::transform([](const std::type_index& idx) {
                return std::string(idx.name());
            })
            | std::ranges::to<std::vector>();
        std::stringstream ss;
        std::copy(
            names.begin(),
            std::prev(names.end()),
            std::ostream_iterator<std::string>(ss, ", ")
        );
        ss << names.back();
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os) const {
        os << "GameObject: { Name: " << name_ << ", Components: [" << get_component_names() << "] }";
        return os;
    }


    [[nodiscard]]
    std::string get_name() const {
        return name_;
    }

    void set_name(const std::string &name) {
        name_ = name;
    }

    [[nodiscard]]
    Transform& get_transform() {
        return transform_;
    }

    [[nodiscard]]
    const Transform& get_transform() const {
        return transform_;
    }


private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
    std::string name_;
    Transform transform_;
};

#endif //GAME_OBJECT_H
