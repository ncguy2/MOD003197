//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MENUITEM_H
#define FIRESIM_MENUITEM_H

#include <string>
#include <functional>
#include <Forest.h>

class MenuHost;

/**
 * The items for the menu, contains an action which takes a Forest pointer.
 */
class MenuItem {
public:
    /**
     * @param label The text to display for the entry
     */
    MenuItem(std::string label);

    /**
     * The invokation of the action
     * @param forest The forest pointer
     */
    void Act(Forest* forest);
    void SetEnabled(bool enabled);
    bool IsEnabled();

    /**
     * Sets the reason that is displayed when the item is disabled
     * @param reason
     */
    void SetDisabledReason(std::string reason);

    /**
     * Helper function, sets the reason and disables the item
     * @param reason
     */
    void SetDisabled(std::string reason);

    /**
     * Sets the invoked action through a complex std::function instance. std::function is used to allow for anonymous functions to capture local variables.
     * @param action The function to invoke
     * @return The menu item instance for method chaining
     */
    MenuItem SetAction(std::function<void(Forest *)> action);

    /**
     * @return The label, with [Disabled] prefixed if the item is disabled
     */
    std::string ToString();
    std::string GetDisabledReason();

    /**
     * The label to display, immutable allows this to be public safely
     */
    const std::string label;
protected:
    std::string disabledReason;
    bool enabled;
    std::function<void(Forest *)> action;
    MenuHost* host;
};

#endif //FIRESIM_MENUITEM_H
