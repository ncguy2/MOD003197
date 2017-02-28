//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_FILEHANDLER_H
#define FIRESIM_FILEHANDLER_H

#include <string>
#include <fstream>
#include <vector>

namespace files {

    void WriteStringToFile(std::string file, std::string str);

    template <typename T>
    void WriteVectorToFile(std::string file, std::vector<T> vec, bool newLine = true);

    template <typename T>
    void Write2DVectorToFile(std::string file, std::vector<std::vector<T>> vec);

    std::vector<std::string> ReadLines(std::string file);

}

#endif //FIRESIM_FILEHANDLER_H
