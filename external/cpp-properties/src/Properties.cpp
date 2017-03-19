//
// Created by nick on 13/03/17
//

#include <algorithm>
#include "../include/Properties.h"
#include "../../../headers/Utils.h"

Properties::Properties() {}

std::string Properties::GetProperty(std::string key) const {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if(properties.find(key) == properties.end()) {
        std::string msg = key + " does not exist";
        throw PropertyNotFoundException(msg.c_str());
    }
    return properties.at(key);
}

std::string Properties::GetProperty(std::string key, const std::string& defVal) const {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if(properties.find(key) == properties.end())
        return defVal;
    return properties.at(key);
}

std::vector<std::string> Properties::GetPropertyNames() const {
    return keys;
}

void Properties::AddProperty(std::string key, const std::string &value) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if(properties.find(key) == properties.end())
        keys.push_back(key);
    properties[key] = value;
}

void Properties::RemoveProperty(std::string key) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if(properties.find(key) == properties.end()) {
        std::string msg = key + " does not exist";
        throw PropertyNotFoundException(msg.c_str());
    }
    keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
    properties.erase(key);
}

int Properties::GetIntProperty(std::string key) {
    std::string str = GetProperty(key);
    return std::stoi(str);
}
int Properties::GetIntProperty(std::string key, int defVal) {
    std::string str = GetProperty(key, "");
    return str.empty() ? defVal : std::stoi(str);
}

float Properties::GetFloatProperty(std::string key) {
    std::string str = GetProperty(key);
    return std::stof(str);
}
float Properties::GetFloatProperty(std::string key, float defVal) {
    std::string str = GetProperty(key, "");
    return str.empty() ? defVal : std::stof(str);
}

bool Properties::GetBoolProperty(std::string key) {
    std::string str = GetProperty(key);
    return utils::stob(str);
}
bool Properties::GetBoolProperty(std::string key, bool defVal) {
    std::string str = GetProperty(key, "");
    return str.empty() ? defVal : utils::stob(str);
}
