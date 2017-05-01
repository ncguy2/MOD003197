//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_CONFIG_H
#define FIRESIM_CONFIG_H


/**
 * Small configuration manager for properties that can be changed in real-time
 */
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
