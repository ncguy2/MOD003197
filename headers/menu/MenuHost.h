//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MENUHOST_H
#define FIRESIM_MENUHOST_H

#include <vector>
#include "MenuItem.h"

/**
 * The manager for the main menu
 */
class MenuHost {
public:
    /**
     * @param forest The primary forest pointer
     */
    MenuHost(Forest* forest);

    /**
     * Single wrapper function for the three primary steps
     */
    void Process();

    /**
     * Draws the menu in the console
     */
    void DrawMenu();
    /**
     * Blocks the thread until an input is given, then determines what to do with the input
     */
    void ProcessInput();
    /**
     * Invokes the action of the menu item at the specified index
     * @param index The item to invoke
     * @return Whether the invokation was successful
     */
    bool ProcessItem(int index);

    /**
     * Adds an item to the menu
     * @param item The item to add
     */
    void AddItem(MenuItem item);

protected:
    /**
     * Whether the menu host should keep looping
     */
    bool alive;
    /**
     * The forest reference
     */
    Forest* forest;

    /**
     * The vector containing all menu items, vector used to allow for rapid iterations
     */
    std::vector<MenuItem> menuItems;
};

#endif //FIRESIM_MENUHOST_H
