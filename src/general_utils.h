#pragma once
#ifndef JW_GENERAL_UTILS_H
#define JW_GENERAL_UTILS_H

//************Content************
#include <regex> 
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <numeric>
//#include <fileapi.h>

namespace Utils
{
    // ******String******
    std::vector<std::string> splitStr(std::string str, std::string delimiter);
    std::string to_string(double value, int precision);

    // ******Time******
    std::string to_string(time_t time, std::string format = "%Y-%m-%d %H:%M:%S");
    std::string getCurrentTimeString(std::string format = "%Y-%m-%d %H:%M:%S");
    int getMilliseconds(std::chrono::system_clock::time_point time);

    // ******File******
    bool isFileExist(const std::string& name);

    /**
     * @brief Write data into file.
     * 
     * @tparam T Any type which can parsed to string by std::to_string()
     * @param path File path
     * @param data Data to be written
     * @date 2021-03-17
     */
    template <typename T>
    void writeFile(std::string path, std::vector<T> data)
    {
        std::ofstream file;
        file.open(path, std::fstream::out);
        for (int i = 0; i < data.size(); i++)
        {
            file << std::to_string(data[i]) + "\n";
        }
        file.close();
    }

    // ******Vector******

    /**
     * @brief A efficient function to remove elements by indices
     * 
     * @code{.cpp}
     * std::vector<std::string> values = { "0", "1", "2", "3", "4", "5" };
     * Utils::removeByIndices(&values, { 0, 2, 4, 5});
     * 
     * // values will be = { "1", "3"}
     * @endcode
     * 
     * @tparam T 
     * @param[in, out] processVector vector to be processed
     * @param[out] indices Value of indices to be removed.
     * @date 2021-03-17
    */
    template<typename T>
    void removeByIndices(std::vector<T>* processVector, std::vector<int> indices)
    {
        size_t removeIndicesSize = indices.size();
        if (removeIndicesSize > 0)
        {
            // Remove

            if (removeIndicesSize < 10)
            {
                // Use erase
                for (int i = removeIndicesSize - 1; i >= 0; i--)
                {
                    processVector->erase(processVector->begin() + indices[i]);
                }
            }
            else
            {
                // Use move
                auto startElement = processVector->begin();
                int moved = 0;

                for (int i = 0; i < removeIndicesSize; i++)
                {
                    int indexAfterRemove = (i + 1 >= removeIndicesSize) ? processVector->size() : indices[i + 1];

                    std::move(startElement + indices[i] + 1,
                        startElement + indexAfterRemove,
                        startElement + indices[i] - moved);

                    moved++;
                }

                processVector->resize(processVector->size() - removeIndicesSize);
            }
        }
    }
}


//*******************************

#endif