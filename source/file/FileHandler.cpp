//
// Created by Guy on 24/02/2017.
//

#include <string>
#include <file/FileHandler.h>

void files::WriteStringToFile(std::string file, std::string str) {
    std::ofstream f;
    f.open(file);
    f << str;
    f.close();
}

template<typename T>
void files::WriteVectorToFile(std::string file, std::vector<T> vec, bool newLine) {
    std::ofstream f;
    f.open(file);
    for(T t : vec) {
        f << t;
        if(newLine) f << std::endl;
    }
    f.close();
}

template<typename T>
void files::Write2DVectorToFile(std::string file, std::vector<std::vector<T>> vec) {
    std::ofstream f;
    f.open(file);
    for(std::vector<T> v : vec) {
        for(T t : v) {
            f << t;
        }
        f << std::endl;
    }
    f.close();
}
