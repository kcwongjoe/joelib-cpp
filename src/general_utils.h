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

#ifndef JW_UTILS_IS_NUMERICAL_H
#define JW_UTILS_IS_NUMERICAL_H
	/**
	 * @brief Check whether typename is numerical type
	 * @tparam T Type to be checked.
     * @date 2021-04-09
	*/
	template <typename T>
	constexpr bool is_numerical = !std::is_same_v<T, bool> && std::is_arithmetic_v<T>;
#endif

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

    /**
	 * @brief Find the index of the closest value in the array
	 * @tparam T Numerical variable
	 * @param[in] value Value to search
	 * @param[in] values The array
	 * @param[in] sorted (option) Set as true if the array was sorted. Default as false.
	 * @return Return the index of the closest value in the array. Return -1 if something wrong.
     * @date 2021-04-09
	*/
	template<typename T>
	int findClosestIndex(T value, std::vector<T> values, bool sorted = false)
	{
		// Exception
		if constexpr (!is_numerical<T>)
			throw "This funciton only support numerical type.";

		int result = -1;
		if (sorted)
		{
			int low = std::lower_bound(values.begin(), values.end(), value) - values.begin();
			int distance1 = std::abs(values[low - 1] - value);
			int distance2 = std::abs(values[low] - value);
			int distance3 = std::abs(values[low + 1] - value);
			if (distance1 < distance2 && distance1 < distance3)
			{
				result = low - 1;
			}
			else if (distance2 < distance1 && distance2 < distance3)
			{
				result = low;
			}
			else
			{
				result = low + 1;
			}
		}
		else
		{
			int minDistance = std::abs(values[0] - value);
			int minIndex = 0;
			for (int i = 1; i < values.size(); i++)
			{
				int distance = std::abs(values[i] - value);
				if (distance < minDistance)
				{
					minIndex = i;
					minDistance = distance;
				}
			}

			result = minIndex;
		}

		return result;
	}
}


//*******************************

#endif