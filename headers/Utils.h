//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_UTILS_H
#define FIRESIM_UTILS_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <windows.h>
#include <wincon.h>
#include "Colours.h"

class ConsoleRef {
public:
    int foreground = WHITE;
    int background = BLACK;

    int GetColourAttribute() {
        return foreground + background * 16;
    }
};

/**
 * Header-only utilities
 */
namespace utils {

    struct Point {
        int x, y;
    };
    struct Angle {
        double degrees;
        double radians;
    };

    static ConsoleRef console;

    float distanceTo(utils::Point a, utils::Point b);
    float distanceTo(utils::Point a, int bx, int by);
    float distanceTo(int ax, int ay, utils::Point b);
    float distanceTo(int ax, int ay, int bx, int by);
    Angle angle(int ax, int ay, int bx, int by);
    void outputEmptyLine();
    void outputEmptyLines(int lines);

    /**
     * Clears the console of all contents
     *
     * If CLEAR_SAFELY is defined, the console is filled with new line characters.
     * If CLEAR_SAFELY is not used, std::system is used with the correct command to clear the respective console
     */
    void clearScreen();

    void cls(HANDLE handle);
    void clsAlt(HANDLE handle);

    std::string itos(int i);

    /**
     * Clamp "polyfill" to remove c++17 dependency
     * @param value The value to clamp
     * @param min The minimum boundary (inclusive)
     * @param max The maximum boundary (inclusive)
     * @return The clamped value
     */
    int clamp(int value, int mn, int mx);

    /**
     * Generates a random number from a uniform distribution between the provided values
     * @param mn The first bound
     * @param mx The second bound
     * @return A random number between the provided bounds
     */
    int random(int mn, int mx);

    /**
     * Blocks execution until a keypress is detected
     *
     * Currently only continues on detection of ENTER
     */
    int processInput();
    int setConsoleColour(int foreground = -1, int background = -1);

    int getConsoleAttribute();
}

#endif //FIRESIM_UTILS_H
