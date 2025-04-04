#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <typeindex>
#include <unordered_map>
#include <ranges>
#include <sstream>

#include "Transform.h"

class GameObject;

class Component {
public:
    virtual ~Component() = default;

    virtual void update(float deltaTime) {}

    GameObject* game_object{nullptr}; // Owner game object
};

class GameObject {
public:
    Transform transform;

    explicit GameObject(std::string name = "GameObject") :
        name_(std::move(name)),
        transform(TRANSFORM_ZERO)
    { }

    ~GameObject() = default;

    void add_component(std::unique_ptr<Component> component);

    [[nodiscard]]
    std::string get_component_names() const;

    std::ostream& operator<<(std::ostream& os) const;

    [[nodiscard]]
    std::string get_name() const;

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component> > components_;
    std::string name_;
};

#endif //GAME_OBJECT_H
