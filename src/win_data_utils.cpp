#include "win_data_utils.h"

namespace Utils
{
    /**
     * @brief Convert LTPSTR/LPWSTR to string
     *
     * @param str LPTSTR
     * @return std::string Return string
     * @date 2021-03-17
     */
    std::string to_string(LPTSTR str)
    {
        std::string result;

#ifdef _UNICODE
        std::wstring wstr(str);
        result = std::string(wstr.begin(), wstr.end());
#else
        result = str;
#endif

        return result;
    }

    /**
     * @brief Convert LPWSTR to string
     *
     * @param str LPWSTR
     * @return std::string Return string
     * @date 2021-03-17
     */
    std::string to_string(LPWSTR str)
    {
        std::string result;
        std::wstring wstr(str);
        result = std::string(wstr.begin(), wstr.end());
        return result;
    }

#pragma region To_LPCSTR

    /**
     * @brief Convert string to LPCSTR/LPCWSTR
     *
     * @code{.cpp}
     * auto var = to_lpcstr("some string");
     * @endcode
     *
     * @param str string
     * @return LPCSTR Return LPCSTR or LPCWSTR
     * @date 2021-03-17
     */
#ifdef _UNICODE
    LPCWSTR Utils::toLpcstrAuto(std::string str)
    {
        return toLpwstr(str);
    }
#else
    LPCSTR toLpcstrAuto(std::string str)
    {
        return toLpcstr(str);
    }
#endif

    /**
     * @brief Convert string to LPCSTR
     *
     * @param str string
     * @return LPCSTR Return LPCSTR or LPCWSTR
     * @date 2021-03-17
     */
    LPCSTR toLpcstr(std::string str)
    {
        LPCSTR result = str.c_str();
        return result;

    }

    /**
     * @brief Convert string to LPCWSTR
     *
     * @param str string
     * @return LPCWSTR Return LPCWSTR
     * @date 2021-03-17
     */
    LPCWSTR toLpwstr(std::string str)
    {
        LPCWSTR result = std::wstring(str.begin(), str.end()).c_str();
        return result;
    }

#pragma endregion To_LPCSTR

    /**
     * @brief Convert BSTR to string
     * @param[in] bstr BSTR
     * @param[in] cp (Option) CodePage, Default as CP_UTF8
     * @return Return string
     * @date 2021-03-17
    */
    std::string bstrToString(BSTR bstr, int cp)
    {
        std::string result = "";

        if (bstr)
        {
            // request content length in single-chars through a terminating nullchar in the BSTR.
            // note: BSTR's support imbedded nullchars, so this will only convert through the first nullchar.
            int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
            if (res > 0)
            {
                result.resize(res);
                WideCharToMultiByte(cp, 0, bstr, -1, &result[0], res, NULL, NULL);
            }
            else
            {   // no content. clear target
                result.clear();
            }
        }

        return result;
    }
}