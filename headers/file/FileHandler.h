//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_FILEHANDLER_H
#define FIRESIM_FILEHANDLER_H

#include <string>
#include <fstream>
#include <vector>

/**
 * Primarily used for debugging
 */
namespace files {

    /**
     * Writes a single string to a file
     * @param file The file to write to, will create if none exists
     * @param str The string to write
     */
    void WriteStringToFile(std::string file, std::string str);

    /**
     * Writes the contents of a vector to a file
     * @tparam T The vector type, may be inferred from {vec} if the compiler supports it
     * @param file The file to write to, will create if none exists
     * @param vec The vector to write from
     * @param newLine Whether each entry should have it's own line
     */
    template <typename T>
    void WriteVectorToFile(std::string file, std::vector<T> vec, bool newLine = true);

    /**
     * Helper method for writing nested vectors (up to 2D)
     * @tparam T The vector type, may be inferred from {vec} if the compiler supports it
     * @param file The file to write to, will create if none exists
     * @param vec The nested vector to write from
     */
    template <typename T>
    void Write2DVectorToFile(std::string file, std::vector<std::vector<T>> vec);

    /**
     * Reads a file directly into a string vector, doesn't support binary files
     * @param file The file to read from
     * @return The lines of the file in a vector
     */
    std::vector<std::string> ReadLines(std::string file);

}

#endif //FIRESIM_FILEHANDLER_H
