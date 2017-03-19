//
// Created by Guy on 21/02/2017.
//

#include <Defines.h>
#include <Utils.h>
#include <string>
#if IS_WINDOWS || _WIN32
#include <conio.h>
#endif
#include <cmath>
#include <sstream>

void utils::outputEmptyLine() {
    std::cout << std::endl;
}

void utils::outputEmptyLines(int lines) {
    for(int i = 0; i < lines; i++)
        outputEmptyLine();
}

void utils::clearScreen() {
    if(CLEAR_SAFELY)
        std::cout << std::string(128, '\n');
    else {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

int utils::clamp(int value, int mn, int mx) {
    int min = cpmin(mn, mx);
    int max = cpmax(mn, mx);

    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

std::mt19937 randomSeed(time(0));
int utils::random(int mn, int mx) {
    int min = cpmin(mn, mx);
    int max = cpmax(mn, mx);
    std::uniform_int_distribution<int> gen(min, max);
    int rand = gen(randomSeed);
    return rand;
}


int utils::processInput() {
#if _WIN32
    return getch();
#else
    return std::getchar();
#endif
}

int utils::setConsoleColour(int foreground, int background) {

    if(foreground == -1) foreground = utils::console.foreground;
    if(background == -1) background = utils::console.background;

    console.foreground = foreground;
    console.background = background;

#if _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), console.GetColourAttribute());
#endif

    return console.GetColourAttribute();
}

void utils::outputColouredText(std::string text, bool newLine, int foreground, int background, bool resetColour) {
#if _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), utils::setConsoleColour(foreground, background));
#else
    std::string fore = foreground > 7 ? "9" : "3";
    std::string back = background > 7 ? "10" : "4";
    std::string colPrefix = "\033[" + fore + utils::itos(foreground % 8) + ";" + back + utils::itos(background % 8) + "m";
    text = colPrefix + text;
#endif
    std::cout << text;
    if(newLine)
        std::cout << std::endl;

    if(resetColour) {
#if _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), utils::setConsoleColour(WHITE, BLACK));
#else
        std::cout << "\033[0m" << std::endl;
#endif
    }

}

int utils::getConsoleAttribute() {
    return console.GetColourAttribute();
}

/**
 * (Taken from msdn https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx)
 * @param hConsole
 */
#if _WIN32
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
#endif

/**
 * (Same as above, taken from (http://stackoverflow.com/a/6487534)), also handles *nix systems
 * @param handle
 */
#if _WIN32
void utils::clsAlt(HANDLE handle) {

    COORD topLeft = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD written;

    GetConsoleScreenBufferInfo(handle, &info);
    FillConsoleOutputCharacterA(handle, ' ', info.dwSize.X * info.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
                               info.dwSize.X * info.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(handle, topLeft);
}
#endif

float utils::distanceTo(utils::Point a, utils::Point b) {
    return distanceTo(a.x, a.y, b.x, b.y);
}

float utils::distanceTo(utils::Point a, int bx, int by) {
    return distanceTo(a.x, a.y, bx, by);
}

float utils::distanceTo(int ax, int ay, utils::Point b) {
    return distanceTo(ax, ay, b.x, b.y);
}


float utils::distanceTo(int ax, int ay, int bx, int by) {
    float dx = std::abs(ax - bx);
    float dy = std::abs(ay - by);
    return std::sqrt((dx*dx) + (dy*dy));
}

utils::Angle utils::angle(int ax, int ay, int bx, int by) {
    int dx = bx - ax;
    int dy = by - ay;
    double angRad = std::atan2(dy, dx) - std::atan2(1, 0);
    double angDeg = angRad * (180/M_PI);
    return Angle{angDeg, angRad};
}

std::string utils::itos(int i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

template<typename Out>
void utils::Split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while(std::getline(ss, item, delim))
        *(result++) = item;
}

std::vector<std::string> utils::Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
//utils::Split(s, delim);
    return elems;
}

std::string utils::ToString(unsigned const char* s) {
    std::stringstream ss;
    ss << s;
    return ss.str();
}

bool utils::stob(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::istringstream is(s);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

static unsigned long x=123456789, y=362436069, z=521288629;
unsigned long utils::rng::xorshf96() {
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

static unsigned int g_seed = 1;
void utils::rng::fast_srand(int seed) {
    g_seed = seed;
}

int utils::rng::fastrand() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
