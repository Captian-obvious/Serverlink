#ifndef sl_ext
#define sl_ext

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

extern "C++" {
    void initialize();
    const string get_arch();
    string get_os_name();
    string get_referring_shell();
    class SL_VisualShell;
};

#endif // sl_ext
