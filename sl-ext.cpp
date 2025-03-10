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
#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#endif
#define SL_EXT_EXPORTS
#include "sl-ext.hpp"
extern "C" {
    bool isInitialized = false;
    int vsh_instances=0;
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
    SL_EXT_API std::string get_os_name() {
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
    SL_EXT_API std::string get_referring_shell() {
        #ifdef _WIN32
        char* psModulePath = nullptr;
        size_t size = 0;
        _dupenv_s(&psModulePath, &size, "PSModulePath");
        if (psModulePath) {
            free(psModulePath);
            return "WinPS";
        } else {
            char* comspec = nullptr;
            size = 0;
            _dupenv_s(&comspec, &size, "COMSPEC");
            if (comspec && std::string(comspec).find("cmd.exe") != std::string::npos) {
                free(comspec);
                return "WinCMD";
            } else {
                if (comspec) {
                    free(comspec);
                };
                return "unknown";
            };
        };
        #elif __linux__
        // Step 1: Get the PPID from /proc/self/status
        std::ifstream statusFile("/proc/self/status");
        std::string line;
        pid_t ppid = -1;
        while (std::getline(statusFile, line)) {
            if (line.substr(0, 5) == "PPid:") {
                ppid = std::stoi(line.substr(5));
                break;
            };
        };
        statusFile.close();
        if (ppid == -1) {
            return "Error: Unable to find PPID";
        };
        // Step 2: Read the executable link from /proc/[PPID]/exe
        std::string exePath = "/proc/" + std::to_string(ppid) + "/exe";
        char parentExePath[PATH_MAX];
        ssize_t len = readlink(exePath.c_str(), parentExePath, sizeof(parentExePath) - 1);
        if (len == -1) {
            return "Error reading " + exePath;
        };
        parentExePath[len] = '\0'; // Null-terminate the string
        std::string parentExe(parentExePath);
        if (parentExe.find("bash") != std::string::npos) {
            return "bash";
        } else if (parentExe.find("zsh") != std::string::npos) {
            return "zsh";
        } else if (parentExe.find("sh") != std::string::npos) {
            return "sh";
        } else {
            return "unix-shell";
        };
        #endif
    };
    SL_EXT_API SL_VisualShell create_visual_shell(FILE* conn){
        SL_VisualShell vs=SL_VisualShell(conn);
        return vs;
    };
    SL_EXT_API void init_visual_shell(SL_VisualShell vs){
        vs.init();
    };
    SL_EXT_API void kill_visual_shell(SL_VisualShell vs){
        vs.kill();
    };
    SL_EXT_API std::string get_last_command_of_shell(SL_VisualShell vs){
        return vs.get_last_command();
    };
};
SL_VisualShell::SL_VisualShell(FILE* conn) {
    this->connection = conn;
    this->shell_child_active = false;
    this->isInitialized = false;
    vsh_instances++;
    this->instanceNumber = vsh_instances;
    this->last_command="";
};
SL_VisualShell::~SL_VisualShell() {
    if (this->isInitialized) {
        printf("VSL: Cleaning up VisualShell(TM) Instance %d \n", this->instanceNumber);
        vsh_instances--;
    };
};
void SL_VisualShell::init() {
    if (!this->isInitialized) {
        printf("VSL: Loading VisualShell(TM) Instance %d \n", this->instanceNumber);
        this->isInitialized = true;
        this->shell_child_active = true;
        // We will create a  shell child from the ssh connection
    };
};
void SL_VisualShell::kill() {
    if (this->isInitialized) {
        printf("VSL: Shutting down VisualShell(TM) Instance %d \n", this->instanceNumber);
        this->isInitialized = false;
        this->shell_child_active = false;
    };
};
std::string SL_VisualShell::get_last_command() {
    if (this->isInitialized) {
        return this->last_command;
    }else{
        return "";
    };
};