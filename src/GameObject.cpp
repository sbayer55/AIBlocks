#include "GameObject.h"
#include <typeindex>
#include <typeinfo>
#include <sstream>
#include <ranges>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <vector>

void GameObject::add_component(std::unique_ptr<Component> component) {
    const auto type_index = std::type_index(typeid(*component));

    if (components_.contains(type_index)) {
        throw std::runtime_error("Component of type " + std::string(typeid(*component).name()) + " already exists");
    }

    component->game_object = this;
    components_[type_index] = std::unique_ptr<Component>(component.release());
}

[[nodiscard]]
std::string GameObject::get_component_names() const {
    const std::vector<std::string> names = components_
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

std::ostream& GameObject::operator<<(std::ostream& os) const {
    os << "GameObject: { Name: " << name_ << ", Components: [" << get_component_names() << "] }";
    return os;
}


[[nodiscard]]
std::string GameObject::get_name() const {
    return name_;
}
