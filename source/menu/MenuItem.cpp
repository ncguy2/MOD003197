//
// Created by Guy on 24/02/2017.
//

#include <menu/MenuItem.h>
#include <Forest.h>

MenuItem::MenuItem(std::string label) : label(label), enabled(true) {}

void MenuItem::Act(Forest* forest) {
    if(this->action != nullptr)
        this->action(forest);
}

MenuItem MenuItem::SetAction(std::function<void(Forest*)> action) {
    this->action = action;
    return *this;
}

void MenuItem::SetEnabled(bool enabled) {
    this->enabled = enabled;
}


std::string MenuItem::ToString() {
    return label + (enabled ? "" : " [Disabled]");
}

bool MenuItem::IsEnabled() {
    return enabled;
}

void MenuItem::SetDisabledReason(std::string reason) {
    disabledReason = reason;
}

std::string MenuItem::GetDisabledReason() {
    if(disabledReason.length() > 0)
        return disabledReason;
    return "No reason provided";
}

void MenuItem::SetDisabled(std::string reason) {
    SetEnabled(false);
    SetDisabledReason(reason);
}
