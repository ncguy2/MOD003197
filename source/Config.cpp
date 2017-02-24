//
// Created by Guy on 24/02/2017.
//

#include <Config.h>

bool Configuration::UseBlockRenderer() {
    return this->useBlockRenderer;
}

bool Configuration::ToggleUseBlockRenderer() {
    return SetUseBlockRenderer(!this->useBlockRenderer);
}

bool Configuration::SetUseBlockRenderer(bool newValue) {
    this->useBlockRenderer = newValue;
    return this->useBlockRenderer;
}
