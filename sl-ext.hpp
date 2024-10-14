#ifndef SL_EXT
#define SL_EXT

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

extern "C++" {
    void initialize();

    class SL_VisualShell {
    public:
        FILE* connection;
        SL_VisualShell(FILE* conn);
    };
}

#endif // SL_EXT