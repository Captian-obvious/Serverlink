#ifndef SL_EXT_HPP
#define SL_EXT_HPP

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#ifdef _WIN32
    #ifdef SL_EXT_EXPORTS
        #define SL_EXT_API __declspec(dllexport)
    #else
        #define SL_EXT_API __declspec(dllimport)
    #endif
#else
    #define SL_EXT_API
#endif

extern "C++" {
    SL_EXT_API void initialize();
    SL_EXT_API const char* get_arch();
    SL_EXT_API string get_os_name();
    SL_EXT_API string get_referring_shell();
    class SL_EXT_API SL_VisualShell;
};
#endif