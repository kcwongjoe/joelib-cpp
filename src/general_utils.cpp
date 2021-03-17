#include "general_utils.h"

namespace Utils
{
#pragma region String

    /**
     * @brief Split string by delimiter
     * 
     * @param str String to be splitted
     * @param delimiter Delimiter
     * @return std::vector<std::string> Splitted strings
     * @date 2021-03-17
     */
    std::vector<std::string> splitStr(std::string str, std::string delimiter)
    {
        std::regex re("[" + std::string(delimiter) + "]");

        std::sregex_token_iterator it{ str.begin(), str.end(), re, -1 };
        std::vector<std::string> tokenized{ it, {} };

        // Additional check to remove empty strings 
        tokenized.erase(
            std::remove_if(tokenized.begin(), tokenized.end(),
                [](std::string const& s) {
                    return s.size() == 0;
                }
            ),
            tokenized.end()
                    );

        return tokenized;
    }

    /**
     * @brief Convert double in specific precision
     * @param value Value to be converted
     * @param precision Precision
     * @return Return string
     * @date 2021-03-17
    */
    std::string to_string(double value, int precision)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        return ss.str();
    }

#pragma endregion String

#pragma region Time

    /**
     * @brief Convert time_t to string
     * 
     * @param[in] time time to be conveted
     * @param[in] format (Option) time format. Default as "%Y-%m-%d %H:%M:%S"
     * @return std::string
     * @date 2021-03-17
     */
    std::string to_string(time_t time, std::string format)
    {
        // Initialize
        struct tm* currentTimeInfo;
        char buffer[80];

#pragma warning(suppress : 4996)
        currentTimeInfo = localtime(&time);

        // Convert to string
        strftime(buffer, sizeof(buffer), format.c_str(), currentTimeInfo);
        std::string result(buffer);

        return result;
    }

    /**
     * @brief Get the Current Time String 
     * 
     * @param format (Option) Time string format. Default as "%Y-%m-%d %H:%M:%S"
     * @return Return current time string
     * @date 2021-03-17
     */
    std::string getCurrentTimeString(std::string format)
    {
        time_t currentTime;

        // Get current time
        time(&currentTime);

        // Convert to string
        std::string result = to_string(currentTime, format);

        return result;
    }

    /**
     * @brief Get the part of millisecond from time
     * @param time 
     * @return Return the part of millisecond
     * @date 2021-03-17
    */
    int getMilliseconds(std::chrono::system_clock::time_point time)
    {
        auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(time);
        auto fraction = time - seconds;
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(fraction).count();
        return ms;
    }

#pragma endregion Time

#pragma region File

    /**
     * @brief Check whether file existed.
     * 
     * @param path Path
     * @return bool Return true if file existed.
     * @date 2021-03-17
     */
    bool isFileExist(const std::string& path) {
        std::ifstream f(path.c_str());
        return f.good();
    }

#pragma endregion File
}

