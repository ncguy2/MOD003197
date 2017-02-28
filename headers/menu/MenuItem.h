//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MENUITEM_H
#define FIRESIM_MENUITEM_H

#include <string>
#include <functional>
#include <Forest.h>

class MenuHost;

class MenuItem {
public:
    MenuItem(std::string label);
    void Act(Forest* forest);
    void SetEnabled(bool enabled);
    bool IsEnabled();
    void SetDisabledReason(std::string reason);
    void SetDisabled(std::string reason);
    MenuItem SetAction(std::function<void(Forest *)> action);

    std::string ToString();
    std::string GetDisabledReason();

    const std::string label;
protected:
    std::string disabledReason;
    bool enabled;
    std::function<void(Forest *)> action;
    MenuHost* host;

};

#endif //FIRESIM_MENUITEM_H
