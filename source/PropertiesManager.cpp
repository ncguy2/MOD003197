//
// Created by Guy on 28/02/2017.
//

#include <vector>
#include <PropertiesManager.h>
#include <algorithm>
#include <file/FileHandler.h>
#include <Utils.h>

struct RetrieveKey {
    template <typename T>
    typename T::first_type operator()(T keyValuePair) const {
        return keyValuePair.first;
    }
};

vector<string> PropertiesManager::GetCategories() {
    vector<string> cats;
    //transform(properties.begin(), properties.end(), back_inserter(cats), RetrieveKey());
    return cats;
}

vector<string> PropertiesManager::GetProperties(std::string category) {
    vector<string> props;
    if(properties.count(category) != 1) return props; // category not found
    for(pair<string, string> p : properties[category])
        props.push_back(p.first);
    return props;
}

string PropertiesManager::GetValue(std::string category, std::string name) {
    if(properties.count(category) != 1) return ""; // category not found
    if(properties[category].count(name) != 1) return "";
    return properties[category][name];
}

template<typename T> T PropertiesManager::Get(std::string category, std::string name, T def) {
    string val = GetValue(category, name);
    if(val.empty()) {
        if(properties.count(category) != 1)
            properties[category] = map<string, string>();
        properties[category][name] = def;
        return def;
    }
    try {
        T t = static_cast<T>(val);
        return t;
    }catch(...) {}
    return def;
}

PropertiesManager::PropertiesManager() {
    vector<string> lines = files::ReadLines("properties.ini");
    map<string, string>* cat = nullptr;
    string catName;
    for(string line : lines) {
        //line.erase(std::find_if(line.rbegin(), line.rend(),
          //                   std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
        if(line.find_first_of('[') == 0 && line.find_last_of(']') == line.length()-1) {
            if(cat != nullptr)
                properties[catName] = *cat;
            catName = line.substr(1, line.length()-2);
            map<string, string> c;
            cat = &c;
        }else{
            vector<string> var = utils::Split(line, '=');
            string key = var[0];
            string val = var[1];
            (*cat)[key] = val;
        }
    }
}

PropertiesManager::~PropertiesManager() {
    std::ofstream f;
    f.open("properties.ini");
    for(pair<string, map<string, string>> cat : properties) {
        f << "[" << cat.first << "]" << std::endl;
        for(pair<string, string> prop : cat.second) {
            f << prop.first << "=" << prop.second << std::endl;
        }
    }
    f.close();
}
