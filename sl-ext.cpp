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
#include "sl-ext.hpp"

extern "C++" {
    bool isInitialized = false;

    SL_EXT_API void initialize() {
        if (!isInitialized) {
            isInitialized = true;
        };
    };

    SL_EXT_API const char* get_arch() {
        #if defined(__x86_64__) || defined(_M_X64)
        return "x86_64";
        #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
        return "x86_32";
        #elif defined(__ARM_ARCH_2__)
        return "ARM2";
        #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        return "ARM3";
        #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
        return "ARM4T";
        #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
        return "ARM5";
        #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
        return "ARM6T2";
        #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
        return "ARM6";
        #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7";
        #elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7A";
        #elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7R";
        #elif defined(__ARM_ARCH_7M__)
        return "ARM7M";
        #elif defined(__ARM_ARCH_7S__)
        return "ARM7S";
        #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64";
        #elif defined(mips) || defined(__mips__) || defined(__mips)
        return "MIPS";
        #elif defined(__sh__)
        return "SUPERH";
        #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        return "POWERPC";
        #elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
        return "POWERPC64";
        #elif defined(__sparc__) || defined(__sparc)
        return "SPARC";
        #elif defined(__m68k__)
        return "M68K";
        #else
        return "UNKNOWN";
        #endif
    };
    SL_EXT_API string get_os_name() {
        #ifdef _WIN32
        return "win32";
        #elif _WIN64
        return "win64";
        #elif __APPLE__ || __MACH__
        return "Mac OSX";
        #elif __linux__
        return "linux";
        #elif __FreeBSD__
        return "FreeBSD";
        #elif __unix || __unix__
        return "unix";
        #else
        return "unknown";
        #endif
    };
    SL_EXT_API string get_referring_shell() {
        #ifdef _WIN32
        const char* psModulePath = std::getenv("PSModulePath");
        if (psModulePath) {
            return "WinPS";
        } else {
            const char* comspec = std::getenv("COMSPEC");
            if (comspec && std::string(comspec).find("cmd.exe") != std::string::npos) {
                return "WinCMD";
            } else {
                return "unknown";
            }
        }
        #elif __linux__
        const char* shellPath = std::getenv("SHELL");
        if (shellPath == nullptr) {
            return "Unix-like";
        }
        std::string shell(shellPath);
        if (shell.find("bash") != std::string::npos){
            return "bash";
        }else if(shell.find("zsh") != std::string::npos){
            return "zsh";
        }else if(shell.find("sh") != std::string::npos){
            return "sh";
        }else{
            return "unix-shell";
        };
        return "No shell binary found";
        #endif
    };
    class SL_VisualShell {
    public:
        FILE* connection;
        SL_VisualShell(FILE* conn) {
            this->connection = conn;
        };
    };
};