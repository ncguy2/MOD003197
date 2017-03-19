//
// Created by nick on 13/03/17.
//

#ifndef FIRESIM_PROPERTIESUTILS_H
#define FIRESIM_PROPERTIESUTILS_H

#include <string>
#include <utility>

    namespace PropertiesUtils {
        std::string LeftTrim(const std::string& str);
        std::string RightTrim(const std::string& str);
        std::string Trim(const std::string& str);
        bool IsProperty(const std::string& str);
        std::pair<std::string, std::string> ParseProperty(const std::string& str);
        bool IsComment(const std::string& str);
        bool IsEmptyLine(const std::string& str);
    }

#endif //FIRESIM_PROPERTIESUTILS_H
