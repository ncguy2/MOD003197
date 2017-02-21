//
// Created by Guy on 21/02/2017.
//

#include <Utils.h>

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

void utils::waitForKeypress() {
    std::string tmp;
    std::getline(std::cin, tmp);
}
