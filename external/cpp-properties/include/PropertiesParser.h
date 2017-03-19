//
// Created by nick on 13/03/17.
//

#ifndef FIRESIM_PROPERTIESPARSER_H
#define FIRESIM_PROPERTIESPARSER_H

#include <string>
#include <exception>
#include "Properties.h"

    class PropertiesParser {
    public:
        PropertiesParser();
        virtual ~PropertiesParser();
        static Properties Read(const std::string& file);
        static void Write(const std::string& file, const Properties& props);
    };

#endif //FIRESIM_PROPERTIESPARSER_H
