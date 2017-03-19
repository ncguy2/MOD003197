//
// Created by nick on 13/03/17.
//


#include <fstream>
#include <iostream>
#include <algorithm>
#include "../include/PropertiesParser.h"
#include "../include/PropertiesUtils.h"

    PropertiesParser::PropertiesParser() {}
    PropertiesParser::~PropertiesParser() {}

    Properties PropertiesParser::Read(const std::string &file) {
        std::ifstream is;
        is.open(file.c_str());
        if(!is.is_open()) {
            std::string msg = "Unable to read " + file;
            throw PropertiesException(msg.c_str());
        }

        try{
            std::string line;
            while(getline(is, line)) {
                if(PropertiesUtils::IsEmptyLine(line) || PropertiesUtils::IsComment(line)) {

                }else if(PropertiesUtils::IsProperty(line)) {
                    std::pair<std::string, std::string> prop = PropertiesUtils::ParseProperty(line);
                    std::transform(prop.first.begin(), prop.first.end(), prop.first.begin(), ::tolower);
                    Properties::Instance().AddProperty(prop.first, prop.second);
                }else{
                    std::string msg = "Invalid line: " + line;
                    throw PropertiesException(msg.c_str());
                }
            }
            is.close();
        }catch(...) {
            is.close();
            throw;
        }

        return Properties::Instance();
    }

    void PropertiesParser::Write(const std::string &file, const Properties &props) {
        std::ofstream os;
        os.open(file.c_str());
        if(!os.is_open()) {
            std::string msg = "Unable to write " + file;
            throw PropertiesException(msg.c_str());
        }

        try{
            std::vector<std::string> keys = props.GetPropertyNames();
            for(std::vector<std::string>::iterator i = keys.begin();
                    i != keys.end(); ++i) {
                os << *i << " = " << props.GetProperty(*i) << std::endl;
            }
            os.close();
        }catch(...) {
            os.close();
            throw;
        }

    }
