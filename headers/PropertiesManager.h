//
// Created by Guy on 28/02/2017.
//

#ifndef FIRESIM_PROPERTIESMANAGER_H
#define FIRESIM_PROPERTIESMANAGER_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class PropertiesManager {
public:
    static PropertiesManager& GetInstance() {
        static PropertiesManager instance;
        return instance;
    }

    template <typename T> T Get(std::string category, std::string name, T defaultValue);



protected:

    vector<string> GetCategories();
    vector<string> GetProperties(std::string category);
    string GetValue(std::string category, std::string name);

    map<string, map<string, string>> properties;

private:
    PropertiesManager();
    ~PropertiesManager();

};

#endif //FIRESIM_PROPERTIESMANAGER_H
