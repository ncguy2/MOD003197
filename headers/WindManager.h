//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_WINDMANAGER_H
#define FIRESIM_WINDMANAGER_H

#include <string>
#include <map>

/**
 * The possible directions for wind
 */
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

/**
 * The possible speeds for wind
 */
enum WindSpeed {
    NONE,
    SLOW,
    FAST,
    FINAL_WINDSPEED_ENTRY
};

class WindManager {
public:

    /**
     * Sets the speed and direction to random values
     */
    void Randomise();

    void SetDirection(WindDirection direction);
    void SetSpeed(WindSpeed speed);

    WindDirection GetDirection();
    WindSpeed GetSpeed();

    /**
     * Invokes translate(WindDirection) with the current direction
     * @return The translated name for the current direction
     */
    std::string GetDirectionString();
    /**
     * Invokes translate(WindSpeed) with the current speed
     * @return The translated name for the current speed
     */
    std::string GetSpeedString();

    /**
     * Translates the provided direction to a string representation
     * @param direction The direction to translate
     * @return The string name of the direction
     */
    std::string translate(WindDirection direction);
    /**
     * Translates the provided speed to a string representation
     * @param speed The speed to translate
     * @return The string name of the speed
     */
    std::string translate(WindSpeed speed);

protected:
    WindDirection direction;
    WindSpeed speed;

    /**
     * Translation map for wind direction
     */
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

    /**
     * Translation map for wind speed
     */
    std::map<WindSpeed, std::string> windSpeedLang = {
            {NONE, "None"},
            {SLOW, "Slow"},
            {FAST, "Fast"},
            {FINAL_WINDSPEED_ENTRY, "Error, invalid wind speed"}
    };

};

#endif //FIRESIM_WINDMANAGER_H
