//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MENUHOST_H
#define FIRESIM_MENUHOST_H

#include <vector>
#include "MenuItem.h"

class MenuHost {
public:
    MenuHost(Forest* forest);
    void Process();

    void DrawMenu();
    void ProcessInput();
    bool ProcessItem(int index);

    void AddItem(MenuItem item);

protected:
    bool alive;
    Forest* forest;
    std::vector<MenuItem> menuItems;
};

#endif //FIRESIM_MENUHOST_H
