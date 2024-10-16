#include <chrono>
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include "sl-ext.hpp"
#ifdef _WIN32
#include "winsock.h"
#elif __linux__
#include <netdb.h>
#include <arpa/inet.h>
#endif

using namespace std;
string ver="4.18.0";
#ifdef _WIN32
string cmd_prefix="> ";
string shell_type="WinCMD";
string user_agent_stub="SL/Windows";
#elif __linux__
string cmd_prefix="$ ";
string shell_type="Unix-like";
string user_agent_stub="SL/Linux";
#endif
class SL_Client {
    public: 
    bool isConnecting;
    bool isConnected;
    bool isInitialized;
    bool isCLIInitialized;
    string currentUrl;
    FILE* sshConnection;
    bool isSSHConnected;
    bool isVisualMode;
    string curr_path;
    string curr_user;
    string credentials;
    string sl_user_agent="SL-Client/1.0 Mozilla/5.0 ("+user_agent_stub+") AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36";
    unordered_map<string,unordered_map<string,string>>commands={
        {"connect", {
            {"desc", "initiates a SSH connection to provided hostname"},
            {"valid", "connect"},
            {"arguments", "[hostname] [port (opt)] <auth (-cred)(opt)>"}
        }},
        {"config", {
            {"desc", "open the configuration menu"},
            {"valid", "cfg,config,configuration"},
            {"arguments", "None"}
        }},
        {"cinfo",{
            {"desc", "displays information about the current connection"},
            {"valid", "cinfo"},
            {"arguments", "None"}
        }},
        {"disconnect",{
            {"desc", "disconnects from the current connection"},
            {"valid", "disconnect,disconn,discon"},
            {"arguments", "None"}
        }},
        {"info", {
            {"desc", "displays information about the client"},
            {"valid", "info"},
            {"arguments", "None"}
        }},
        {"visual", {
            {"desc", "enters visual mode (not fully implemented)"},
            {"valid", "visual,gui,vis"},
            {"arguments", "None"}
        }},
        {"exit", {
            {"desc", "exits the CLI"},
            {"valid", "exit,quit,leave"},
            {"arguments", "None"}
        }}
    };
    unordered_map<string,string>info={
        {"version",ver},
        {"sshVer","openSSH-2"},
        {"shell",shell_type},
    };
    string ip;
    int port;
    SL_Client() {
        this->isConnecting=false;
        this->isConnected=false;
        this->isInitialized=false;
        this->isCLIInitialized=false;
        this->currentUrl="";
        this->isSSHConnected=false;
        this->isVisualMode=false;
    };
    void get_connection_info(){
        if (!this->isConnected){
            this->print_err("Not connected to a server, no connection info available.");
        }else{
            unordered_map<string,string>conn_info={
                {"ip",this->ip},
                {"port",to_string(this->port)},
                {"url",this->currentUrl},
                {"current_path",this->curr_path},
                {"user",this->curr_user},
                {"user-agent","'"+this->sl_user_agent+"'"},
            };
            for(auto i=conn_info.begin();i!=conn_info.end();i++){
                cout << "  " << i->first << ":" << i->second << endl;
            };
        };
    };
    void connect(string hostname,int port,string credentials){
        string usr="";
        if(isConnected){
            this->print_err("Already connected to a server");
        };
        if(!isInitialized){
            this->print_err("Serverlink not initialized");
        };
        if (!isConnecting){
            this->isConnecting=true;
            this->print_info("Connecting to https://"+hostname+" on port "+to_string(port)+". Please wait...");
            if (hostname.find("@")!=string::npos){
                usr=hostname.substr(0,hostname.find("@"));
                hostname=hostname.substr(hostname.find("@")+1);
            };
            this->curr_user=usr;
            this->credentials=credentials;
            this_thread::sleep_for(chrono::milliseconds(1000));
            if (this->hostname_resolves(hostname)){
                this->print_info("Connected to https://"+hostname+".");
                this->isConnecting=false;
                this->isConnected=true;
                this->ip=hostname;
                this->port=port;
                this->currentUrl="https://"+hostname+":"+to_string(port);
                this->initialize_cli(hostname);
            }else{
                this->print_err("Failed to connect to https://"+hostname+".\nRequest failed or Hostname does not resolve.");
                this->isConnecting=false;
            };
        }else{
            this->print_err("Already connecting to a server");
        };
    };
    int initialize_ssh(string hostname,int port,string usr){
        string precmd;
        if (usr!=""){
            precmd="ssh "+usr+"@"+hostname;
        }else{
            precmd="ssh "+hostname;
        };
        const char* cmd=precmd.c_str();
        try{
            this->sshConnection=popen(cmd,"w");

        }catch(system_error& e){
            this->print_err("Failed to initialize SSH connection");
            return 1;
        };
        return 0;
    };
    void ssh_communicate(string command){
        if(!isConnected){
            this->print_err("Not connected to a server");
        }else if (this->isSSHConnected){
            fprintf(this->sshConnection,"%s",command.c_str());
        };
    };
    void ssh_close(){
        if(!isConnected){
            this->print_err("Not connected to a server");
        }else if(this->isSSHConnected){
            this->isSSHConnected=false;
            pclose(this->sshConnection);
            this->print_info("SSH connection closed");
        };
    };
    void initialize_cli(string hostname){
        if (!isCLIInitialized){
            this->print_info("Serverlink CLI initializing. Please wait...");
            this->isCLIInitialized=true;
            this_thread::sleep_for(chrono::milliseconds(100));
            this->print_info("<SHELL STARTING>");
            this_thread::sleep_for(chrono::milliseconds(900));
        };
    };
    void disconnect(){
        if(isConnected){
            this->print_info("Saving and disconnecting from https://"+this->ip+".");
            this_thread::sleep_for(chrono::milliseconds(1000));
            this->print_info("Disconnected from https://"+this->ip+".");
            this->isConnected=false;
        }else{
            this->print_err("Not connected to a server");
        };
    };
    int begin_shell(string hostname,int port,string credentials){
        return 0;
    };
    auto get_command(string cmd){
        string result="";
        // Include the <string> header to use split
        #include <string>
        for(auto i=this->commands.begin();i!=this->commands.end();i++){
            string cmd_name=i->first;
            // Split the valid names by comma
            vector<string> validNames=this->split(i->second["valid"],',');
            // Check if cmd is in the valid names list
            if (find(validNames.begin(),validNames.end(),cmd) != validNames.end()){
                result=cmd_name;
                break;
            };
        };
        return result;
    };
    void initialize(){
        if(!isInitialized){
            this->print_info("Serverlink v"+ver+" ("+get_arch()+") on "+get_os_name());
            this->isInitialized=true;
            this->isConnected=false;
            this->isConnecting=false;
            this->currentUrl="";
        }else{
            this->print_err("Serverlink already initialized.");
        };
    };
    bool hostname_resolves(const std::string& hostname) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            cerr << "\033[1;31mWSAStartup failed.\033[0m" << endl;
            return false;
        };
        #endif
        struct addrinfo hints, *res;
        int status;
        memset(&hints,0,sizeof hints);
        hints.ai_family=AF_UNSPEC; // AF_INET or AF_INET6 to force version
        hints.ai_socktype=SOCK_STREAM;    
        if ((status=getaddrinfo(hostname.c_str(), NULL, &hints, &res))!=0){
            #ifdef _WIN32
            cerr << "\033[1;31mSL: Unable to get address info: " << WSAGetLastError() << "\033[0m" << endl;
            #else
            cerr << "\033[1;31mSL: Unable to get address info: " << gai_strerror(status) << "\033[0m" << endl;
            #endif
            #ifdef _WIN32
            WSACleanup();
            #endif
            return false;
        };
        freeaddrinfo(res); // free the linked list
        #ifdef _WIN32
        WSACleanup ();
        #endif
        return true;
    };
    vector<string> split(const string &s,const char separator=' '){
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream,token,separator)){
            tokens.push_back(token);
        };
        return tokens;
    };
    void run_cmd(string cmd, vector<string> args){
        if (cmd=="connect"){
            if(args.size()<1){
                this->print_err("No hostname provided, process exited.");
            }else if(args.size()<2){
                this->connect(args[0],22,"");
            }else if(args.size()<3){
                this->connect(args[0],stoi(args[1]),"");
            }else if (args.size()==3){
                if (args[2].find("-cred=")!=string::npos){
                    this->connect(args[0],stoi(args[1]),args[2].substr(6));
                }else{
                    this->print_err("Invalid credentials provided, process exited.");
                };
            }else{
                this->print_err("Too many arguments provided, process exited.");
            };
        }else if(cmd=="info"){
            if (args.size()<1){
                for(auto i=this->info.begin();i!=this->info.end();i++){
                    cout << "  " << i->first << ":" << i->second << endl;
                };
            };
        }else if(cmd=="cinfo"){
            if (args.size()<1){
                this->get_connection_info();
            };
        }else if(cmd=="disconnect"){
            if (args.size()<1){ 
                this->disconnect();
            }else{
                this->print_err("Too many arguments provided.");
            };
        }else if(cmd=="visual"){
            if (args.size()<1){
                this->isVisualMode=true;
                this->print_info("Press <ENTER> to enter visual mode.");
            }else if(args.size()<2){
                this->isVisualMode=true;
                this->print_info("Press <ENTER> to enter visual mode.");
            }else{
                this->print_err("Too many arguments provided!");
            };
        };
    };
    void print_info(string output){
        cout << "SL: " << output << endl;
    };
    void print_err(string output){
        cout << "\033[1;31mSL: " << output << "\033[0m" << endl;
    };
};
class SL_GUI {
    public:
    SL_Client client;
    string hostname;
    int port;
    string credentials;
    string username;
    bool window_exists=false;
    void init(SL_Client sl){
        this->client=sl;
        this->update_vars();
    };
    void update_vars(){
        this->hostname=this->client.ip;
        this->port=this->client.port;
        this->credentials=this->client.credentials;
        this->username=this->client.curr_user;
    };
    void start_ui(string page){
        if (!this->client.isInitialized){
            cout << "SL: Initializing Client..." << endl;
            this->client.initialize();
        };
        this->client.print_info("Preparing to enter visual mode...");
        this->print_info("Initializing application...");
        this_thread::sleep_for(chrono::milliseconds(1000));
        this->print_info("Loading window...");
        if (!this->window_exists){
            this->make_window();
        };
    };
    void make_window(){
        this->window_exists=true;
        this->print_info("Starting <main>...");
        this_thread::sleep_for(chrono::milliseconds(100));
        this->client.print_info("Exiting to visual mode...");
        this->print_info("Welcome to Serverlink!");
    };
    void print_info(string output){
        cout << "VSL: " << output << endl;
    };
    void print_err(string output){
        cout << "\033[1;31mVSL: " << output << "\033[0m" << endl;
    };
};

vector<string> split(const string &s,const char separator=' '){
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream,token,separator)){
        tokens.push_back(token);
    };
    return tokens;
};
#ifdef _WIN32
#include "windows.h"

typedef void (*InitializeFunc)();
typedef const char* (*GetArchFunc)();
typedef const char* (*GetOSNameFunc)();
typedef const char* (*GetReferringShellFunc)();

InitializeFunc initialize = nullptr;
GetArchFunc get_arch = nullptr;
GetOSNameFunc get_os_name = nullptr;
GetReferringShellFunc get_referring_shell = nullptr;

void loadSL_ExtDLL() {
    static bool isLoaded = false;
    if (!isLoaded) {
        HINSTANCE hDLL = LoadLibrary("sl-ext.dll");
        if (hDLL != NULL) {
            initialize = (InitializeFunc)GetProcAddress(hDLL, "initialize");
            get_arch = (GetArchFunc)GetProcAddress(hDLL, "get_arch");
            get_os_name = (GetOSNameFunc)GetProcAddress(hDLL, "get_os_name");
            get_referring_shell = (GetReferringShellFunc)GetProcAddress(hDLL, "get_referring_shell");
            if (!initialize || !get_arch || !get_os_name || !get_referring_shell) {
                cerr << "Failed to get one or more function addresses." << endl;
                MessageBox(NULL,"Failed to get one or more function addresses.","AddrLoadFail",MB_OK | MB_ICONERROR);
            };
            isLoaded = true;
        }else{
            cerr << "Failed to load sl-ext.dll . Is it in the current directory?" << endl;
            MessageBox(NULL,"Failed to load 'sl-ext.dll'. Is in in the current directory?","Failed to load dll",MB_OK | MB_ICONERROR);
        };
    };
};
#endif
int main(int argc, char** argv){
    #ifdef _WIN32
    loadSL_ExtDLL();
    #endif
    shell_type=get_referring_shell();
    SL_Client slc=SL_Client();
    slc.initialize();
    SL_GUI gui=SL_GUI();
    gui.init(slc);
    bool magic_exit_code=false;
    if (argc<2){
        cout << "Welcome to Serverlink, what would you like to do?" << endl;
        cout << "Type 'help' for a list of commands" << endl;
        while(!magic_exit_code){
            string input;
            cout << cmd_prefix;
            getline(cin,input);
            vector<string> args=split(input);
            if(input==""){
                continue;
            }else if(args[0]=="help"){
                cout << "Commands:" << endl;
                for(auto i=slc.commands.begin();i!=slc.commands.end();i++){
                    cout << "  " << i->first << " - " << i->second["desc"] << endl;
                    cout << "  Arguments: " << i->second["arguments"] << endl;
                };
            }else if(slc.get_command(args[0])!=""){
                string cmdname=slc.get_command(args[0]);
                if (cmdname=="exit"){
                    // Close the CLI if the user types exit
                    magic_exit_code=true;
                }else{
                    // Run the command
                    args.erase(args.begin());
                    slc.run_cmd(cmdname,args);
                    if (slc.isVisualMode){
                        gui.start_ui("default");
                    };
                };
            }else{
                cout << "\033[1;31mInvalid command.\033[0m" << endl;
            };
        };
    }else if (argc>1){
        vector<string> args(argv,argv+argc);
        args.erase(args.begin());
        string cmd=args[0];
        if(cmd=="help"){
            cout << "Commands:" << endl;
            for(auto i=slc.commands.begin();i!=slc.commands.end();i++){
                cout << "  " << i->first << " - " << i->second["desc"] << endl;
                cout << "  Arguments: " << i->second["arguments"] << endl;
            };
        }else if(slc.get_command(cmd)!=""){
            string cmdname=slc.get_command(cmd);
            // Run the command
            args.erase(args.begin());
            slc.run_cmd(cmdname,args);
        }else{
            cout << "\033[1;31mInvalid command.\033[0m" << endl;
        };
    };
    return 0;
};
