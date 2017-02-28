//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_UTILS_H
#define FIRESIM_UTILS_H

#include <Defines.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include "Colours.h"

#if _WIN32
#include <windows.h>
#include <wincon.h>
#endif

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

#if _WIN32
    void cls(HANDLE handle);
    void clsAlt(HANDLE handle);
#endif // IS_WINDOWS
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


    /**
     * Writes coloured text to the console
     *
     * @param text The text to write
     * @param newLine Whether to automatically step to the next line after writing
     * @param foreground The text colour
     * @param background The background colour
     * @param resetColour Whether to reset the colour attribute after use (Only applicable in windows terminals)
     */
    void outputColouredText(std::string text, bool newLine = true, int foreground = -1, int background = -1, bool resetColour = true);

    int getConsoleAttribute();

    template <typename Out>
    void Split(const std::string &s, char delim, Out result);
    std::vector<std::string> Split(const std::string &s, char delim);

    std::string ToString(unsigned const char* s);

    namespace rng {

        /**
         * xorshf generator
         * Taken from http://stackoverflow.com/a/1640399
         */
        unsigned long xorshf96();

        /**
         * Intel fastrand
         * https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
         */
        void fast_srand(int seed);
        int fastrand();
    }

}

#endif //FIRESIM_UTILS_H
