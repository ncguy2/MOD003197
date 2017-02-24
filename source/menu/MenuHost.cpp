//
// Created by Guy on 24/02/2017.
//

#include <menu/MenuHost.h>
#include <iostream>
#include <Utils.h>

MenuHost::MenuHost(Forest* forest) : forest(forest), alive(true) {}

void MenuHost::Process() {
    while(alive) {
        DrawMenu();
        ProcessInput();
    }
}

void MenuHost::DrawMenu() {
    utils::clearScreen();
    std::cout << "Fire simulation" << std::endl;
    std::cout << "Please resize the window here, resizing while rendering can cause undefined behaviours" << std::endl;
    std::cout << "(Shift + R redraws this menu, in the event that resizing causes it to disappear)" << std::endl;

    utils::outputEmptyLine();
    utils::outputEmptyLine();
    for(int i = 0; i < this->menuItems.size(); i++) {
        MenuItem item = this->menuItems[i];
        std::cout << (i+1) << ") " << item.ToString() << std::endl;
    }
    utils::outputEmptyLine();
    std::cout << "Please select the renderer to employ." << std::endl;
}

void MenuHost::ProcessInput() {
    int key = utils::processInput();
    switch(key) {
        case 0x31: case VK_NUMPAD1: if(ProcessItem(0)) return; break;
        case 0x32: case VK_NUMPAD2: if(ProcessItem(1)) return; break;
        case 0x33: case VK_NUMPAD3: if(ProcessItem(2)) return; break;
        case 0x34: case VK_NUMPAD4: if(ProcessItem(3)) return; break;
        case 0x35: case VK_NUMPAD5: if(ProcessItem(4)) return; break;
        case 0x36: case VK_NUMPAD6: if(ProcessItem(5)) return; break;
        case 0x37: case VK_NUMPAD7: if(ProcessItem(6)) return; break;
        case 0x38: case VK_NUMPAD8: if(ProcessItem(7)) return; break;
        case 0x39: case VK_NUMPAD9: if(ProcessItem(8)) return; break;
        case VK_ESCAPE: alive = false; return;
        case 0x52: return;
        default:break;
    }

    std::cout << "Invalid key detected, please select a valid numerical key" << std::endl;
    ProcessInput();
}

bool MenuHost::ProcessItem(int index) {
    if(this->menuItems.size() <= index) return false;
    MenuItem item = this->menuItems[index];
    if(!item.IsEnabled()) {
        std::cout << item.label << " is currently disabled, " << item.GetDisabledReason() << std::endl;
        ProcessInput();
        return true;
    }
    this->forest->Reset();
    item.Act(this->forest);
    return true;
}

void MenuHost::AddItem(MenuItem item) {
    this->menuItems.push_back(item);
}



