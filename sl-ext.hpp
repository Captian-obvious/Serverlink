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
#include <fstream>
#include <unistd.h>
#include <limits.h>

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
class SL_EXT_API SL_VisualShell {
public:
    FILE* connection;
    bool isInitialized;
    bool shell_child_active;
    int instanceNumber;

    SL_VisualShell(FILE* conn);
    ~SL_VisualShell();

    void init();
    void kill();
};
extern "C" {
    SL_EXT_API void initialize();
    SL_EXT_API const char* get_arch();
    SL_EXT_API string get_os_name();
    SL_EXT_API string get_referring_shell();
    SL_EXT_API SL_VisualShell create_visual_shell(FILE* conn);
    SL_EXT_API void init_visual_shell(SL_VisualShell vs);
    SL_EXT_API void kill_visual_shell(SL_VisualShell vs);
};
#endif