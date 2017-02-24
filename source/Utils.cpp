//
// Created by Guy on 21/02/2017.
//

#include <Utils.h>
#include <string>
#include <conio.h>

void utils::outputEmptyLine() {
    std::cout << std::endl;
}

void utils::outputEmptyLines(int lines) {
    for(int i = 0; i < lines; i++)
        outputEmptyLine();
}

void utils::clearScreen() {
#ifdef CLEAR_SAFELY
    std::cout << std::string(128, '\n');
#else
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
#endif
}

int utils::clamp(int value, int mn, int mx) {
    int min = std::min(mn, mx);
    int max = std::max(mn, mx);

    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

std::mt19937 rng(time(0));
int utils::random(int mn, int mx) {
    int min = std::min(mn, mx);
    int max = std::max(mn, mx);
    std::uniform_int_distribution<int> gen(min, max);
    return gen(rng);
}



int utils::processInput() {
//    std::string tmp;
//    std::getline(std::cin, tmp);
//    return tmp;

    return getch();
}

int utils::setConsoleColour(int foreground, int background) {

    if(foreground == -1) foreground = utils::console.foreground;
    if(background == -1) background = utils::console.background;

    console.foreground = foreground;
    console.background = background;
    return console.GetColourAttribute();
}

int utils::getConsoleAttribute() {
    return console.GetColourAttribute();
}

/**
 * (Taken from msdn https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx)
 * @param hConsole
 */
void utils::cls( HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

// Get the number of character cells in the current buffer.

    if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.

    if( !FillConsoleOutputCharacter( hConsole,        // Handle to console screen buffer
                                     (TCHAR) ' ',     // Character to write to the buffer
                                     dwConSize,       // Number of cells to write
                                     coordScreen,     // Coordinates of first cell
                                     &cCharsWritten ))// Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.

    if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
    {
        return;
    }

    // Set the buffer's attributes accordingly.

    if( !FillConsoleOutputAttribute( hConsole,         // Handle to console screen buffer
                                     csbi.wAttributes, // Character attributes to use
                                     dwConSize,        // Number of cells to set attribute
                                     coordScreen,      // Coordinates of first cell
                                     &cCharsWritten )) // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.

    SetConsoleCursorPosition( hConsole, coordScreen );
}

/**
 * (Same as above, taken from (http://stackoverflow.com/a/6487534)), also handles *nix systems
 * @param handle
 */
void utils::clsAlt(HANDLE handle) {
#if _WIN32
    COORD topLeft = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD written;

    GetConsoleScreenBufferInfo(handle, &info);
    FillConsoleOutputCharacterA(handle, ' ', info.dwSize.X * info.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
                               info.dwSize.X * info.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(handle, topLeft);
#else
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
#endif
}

float utils::distanceTo(utils::Point a, utils::Point b) {
    return distanceTo(a.x, a.y, b.x, b.y);
}

float utils::distanceTo(int ax, int ay, int bx, int by) {
    float dx = std::abs(ax - bx);
    float dy = std::abs(ay - by);
    return std::sqrt((dx*dx) + (dy*dy));
}
