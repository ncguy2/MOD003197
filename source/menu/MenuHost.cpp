//
// Created by Guy on 24/02/2017.
//

#include <menu/MenuHost.h>
#include <iostream>
#include <Utils.h>
#include <Config.h>

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
        std::string text = utils::itos(i+1) + ") " + item.ToString();
#if USE_COLOURS
        if(item.IsEnabled()) utils::outputColouredText(text, true, WHITE, BLACK, true);
        else utils::outputColouredText(text, true, GREY, BLACK, true);
#else
        std::cout << text << std::endl;
#endif
//        std::cout << (i+1) << ") " << item.ToString() << std::endl;
    }
//    utils::setConsoleColour(WHITE, BLACK);
    utils::outputEmptyLine();
    std::cout << "Please select the renderer to employ." << std::endl;
}

void MenuHost::ProcessInput() {
    int key = utils::processInput();
    bool suppressMsg = false;
    switch(key) {
        case 0x31: if(ProcessItem(0)) return; break;
        case 0x32: if(ProcessItem(1)) return; break;
        case 0x33: if(ProcessItem(2)) return; break;
        case 0x34: if(ProcessItem(3)) return; break;
        case 0x35: if(ProcessItem(4)) return; break;
        case 0x36: if(ProcessItem(5)) return; break;
        case 0x37: if(ProcessItem(6)) return; break;
        case 0x38: if(ProcessItem(7)) return; break;
        case 0x39: if(ProcessItem(8)) return; break;
        case 0x1B: case 0x45: alive = false; return; // ESCAPEB
        case 0x42: // B key
            std::cout << "Block renderer "
                      << (Configuration::Instance().ToggleUseBlockRenderer() ? "enabled" : "disabled")
                      << std::endl;
            suppressMsg = true;
            break;
        case 0x52: return; // R key
        default:break;
    }
    if(!suppressMsg)
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



