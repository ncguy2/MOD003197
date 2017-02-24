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
    MenuItem SetAction(void* (*action)(Forest*));
    void SetEnabled(bool enabled);
    bool IsEnabled();
    void SetDisabledReason(std::string reason);
    void SetDisabled(std::string reason);

    std::string ToString();
    std::string GetDisabledReason();

    const std::string label;
protected:
    std::string disabledReason;
    bool enabled;
    void* (*action)(Forest*);
    MenuHost* host;
};

#endif //FIRESIM_MENUITEM_H
