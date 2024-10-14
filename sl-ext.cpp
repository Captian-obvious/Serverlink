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
    void initialize(){
        printf("Initializing extension library...");
    };
    class SL_VisualShell{
        public:
        FILE* connection;
        SL_VisualShell(FILE* conn){
            this->connection=conn;
        };
    };
};