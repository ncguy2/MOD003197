//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_CONFIG_H
#define FIRESIM_CONFIG_H


class Configuration {
public:
    static Configuration& Instance() {
        static Configuration instance;
        return instance;
    }

    bool UseBlockRenderer();
    bool ToggleUseBlockRenderer();
    bool SetUseBlockRenderer(bool newValue);

private:
    Configuration() : useBlockRenderer(false) {}
    bool useBlockRenderer;

};

#endif //FIRESIM_CONFIG_H
