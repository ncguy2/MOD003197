//
// Created by nick on 13/03/17.
//

#include "../include/PropertiesUtils.h"

    namespace PropertiesUtils {
        namespace {
            const std::string TRIM_DELIMITERS = " \f\n\r\t\v";
        }

        std::string RightTrim(const std::string& str) {
            std::string::size_type s = str.find_last_not_of(TRIM_DELIMITERS);
            if(s == std::string::npos)
                return "";
            return str.substr(0, s+1);
        }


        std::string LeftTrim(const std::string& str) {
            std::string::size_type s = str.find_first_not_of(TRIM_DELIMITERS);
            if(s == std::string::npos)
                return "";
            return str.substr(s);
        }

        std::string Trim(const std::string& str) {
            return RightTrim(LeftTrim(str));
        }

        bool IsProperty(const std::string& str) {
            std::string trimmedStr = Trim(str);
            std::string::size_type s = trimmedStr.find_first_of("=");
            if(s == std::string::npos) return false;
            std::string key = Trim(trimmedStr.substr(0, s));
            if(key == "") return false;
            return true;
        }

        std::pair<std::string, std::string> ParseProperty(const std::string& str) {
            std::string trimmedStr = Trim(str);
            std::string::size_type s = trimmedStr.find_first_of("=");
            std::string key = Trim(trimmedStr.substr(0, s));
            std::string value = Trim(trimmedStr.substr(s+1));
            return std::pair<std::string, std::string>(key, value);
        };

        bool IsComment(const std::string& str) {
            std::string trimmedStr = LeftTrim(str);
            return trimmedStr[0] == '#';
        }

        bool IsEmptyLine(const std::string& str) {
            std::string trimmedStr = LeftTrim(str);
            return trimmedStr.empty();
        }

    }