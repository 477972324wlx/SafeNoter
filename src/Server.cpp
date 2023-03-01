#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
#include "header/FileHandler.hpp"
using namespace std;


void ReadNote(ServerSocketHandler*, string);
void WriteNote(ServerSocketHandler*,string);
void RemoveNote(ServerSocketHandler*,string);



int main(int args, char ** argv){
    const string addr = "127.0.0.1";
    unsigned short port = 4396;
    
    if(args != 3){
        return 0;
    }


    ServerSocketHandler server = ServerSocketHandler(port, addr);
    
    // block until connection
    server.tryAccept();
    string op = argv[1];

    if(op == "read"){
        //cout << "Start reading" << endl;
        ReadNote(&server, argv[2]);
    } else if(op == "write"){
        WriteNote(&server, argv[2]);
    } else if(op == "remove"){
        RemoveNote(&server, argv[2]);
    }

    // Get operation type : Read/Write/Remove
    // server.tryReceive();
    /*
    
    server.trySend(string(argv[0]) + string(argv[1]));
    
    while(server.tryReceive()){
        server.trySend("OK! Message is" + server.getBufferString());
    }
    */


    server.closeSocket();
    return 0;
}


void ReadNote(ServerSocketHandler * server, string filename){
    const int MAX_BUFFER  = 1024;
    char buffer[MAX_BUFFER];
    cout << "Start Reading" << endl;
    string execute_command = "cat *"+filename+"*";

    cout << execute_command << endl;

    FILE * stream = FileHandler::GetPipeFromCommand(execute_command);
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, MAX_BUFFER, stream) != NULL) {
               // cout << buffer << endl;
                //cout << "Server!" << endl;
                server->trySend(buffer);
                
            }
        pclose(stream);
    } else {
        server->trySend("Read Error!\n");
    }
}

void WriteNote(ServerSocketHandler* server,string filename){
    //First Create File
    FileHandlerReply reply = FileHandler::createFile(filename);

    if(reply.error_code){
        exit(0);
    }

    string filename = reply.response;

    while(server->tryReceive()){
        string str = server->getBufferString();
        reply = FileHandler::appendFile(filename, str);
        if(reply.error_code){
            exit(0);
        }
    }
}

void RemoveNote(ServerSocketHandler* server,string filename){
    string execute_command = "rm *" + filename + "*";
    string ret = FileHandler::GetStdoutFromCommand(execute_command);
    if (ret == ""){
        ret = "Success!";
    }
    server->trySend(ret);
}