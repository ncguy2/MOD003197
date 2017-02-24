//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_WINDMANAGER_H
#define FIRESIM_WINDMANAGER_H

#include <string>
#include <map>

enum WindDirection {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
    FINAL_WINDDIRECTION_ENTRY
};

enum WindSpeed {
    NONE,
    SLOW,
    FAST,
    FINAL_WINDSPEED_ENTRY
};

class WindManager {
public:

    void Randomise();

    void SetDirection(WindDirection direction);
    void SetSpeed(WindSpeed speed);

    WindDirection GetDirection();
    WindSpeed GetSpeed();

    std::string GetDirectionString();
    std::string GetSpeedString();

    std::string translate(WindDirection direction);
    std::string translate(WindSpeed speed);

protected:
    WindDirection direction;
    WindSpeed speed;

    std::map<WindDirection, std::string> windDirectionLang = {
            {NORTH, "North"},
            {NORTH_EAST, "North-east"},
            {EAST, "East"},
            {SOUTH_EAST, "South-east"},
            {SOUTH, "South"},
            {SOUTH_WEST, "South-west"},
            {WEST, "West"},
            {NORTH_WEST, "North-west"},
            {FINAL_WINDDIRECTION_ENTRY, "Error, invalid wind direction"}
    };

    std::map<WindSpeed, std::string> windSpeedLang = {
            {NONE, "None"},
            {SLOW, "Slow"},
            {FAST, "Fast"},
            {FINAL_WINDSPEED_ENTRY, "Error, invalid wind speed"}
    };

};

#endif //FIRESIM_WINDMANAGER_H
