//
// Created by Guy on 24/02/2017.
//

#include <WindManager.h>
#include <Utils.h>
#include <Defines.h>

void WindManager::Randomise() {
#if ENABLE_WIND
    int d = utils::random(0, 1000);
    int s = utils::random(0, 1000);

    int dirMax = FINAL_WINDDIRECTION_ENTRY;
    int spdMax = FINAL_WINDSPEED_ENTRY;

    WindDirection dir = static_cast<WindDirection>(d % dirMax);
    WindSpeed speed = static_cast<WindSpeed>(s % spdMax);

    SetDirection(dir);
    SetSpeed(speed);
#else
    SetDirection(WindDirection::NORTH);
    SetSpeed(WindSpeed::NONE);
#endif
}

void WindManager::SetDirection(WindDirection direction) {
    this->direction = direction;
}

void WindManager::SetSpeed(WindSpeed speed) {
    this->speed = speed;
}

WindDirection WindManager::GetDirection() {
    return this->direction;
}

WindSpeed WindManager::GetSpeed() {
    return this->speed;
}

std::string WindManager::translate(WindDirection direction) {
    return windDirectionLang[direction];
}

std::string WindManager::translate(WindSpeed speed) {
    return windSpeedLang[speed];
}

std::string WindManager::GetDirectionString() {
    return translate(GetDirection());
}

std::string WindManager::GetSpeedString() {
    return translate(GetSpeed());
}
