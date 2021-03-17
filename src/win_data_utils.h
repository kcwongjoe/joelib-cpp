#pragma once
#ifndef JW_WIN_DATA_UTILS_H
#define JW_WIN_DATA_UTILS_H

//************Content************
#include <windows.h>
#include <windef.h>
#include <string>

namespace Utils
{
    // ******String******

    std::string to_string(LPTSTR str);
    std::string to_string(LPWSTR str);

    std::string bstrToString(BSTR bstr, int cp = CP_UTF8);

    // To_LPCSTR

#ifdef _UNICODE
    LPCWSTR toLpcstrAuto(std::string str);
#else
    LPCSTR toLpcstrAuto(std::string str);
#endif
    LPCSTR toLpcstr(std::string str);
    LPCWSTR toLpwstr(std::string str);
}

//*******************************

#endif