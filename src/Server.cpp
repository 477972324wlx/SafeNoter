#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
#include "header/ThreadWorker.hpp"
using namespace std;


//void ReadNote(ServerSocketHandler*, string);
//void WriteNote(ServerSocketHandler*,string);
//void RemoveNote(ServerSocketHandler*,string);

const string addr = "127.0.0.1";
unsigned short port = 4396;




int main(int args, char ** argv){




    ServerSocketHandler server = ServerSocketHandler(port, addr);
    
     
    while(1){
        static int id;
        int clientfd = server.tryAccept();

        ThreadArgs * newArg = new ThreadArgs();
        newArg->pid = ++id;
        newArg->clientfd = clientfd;

        thread newThread = thread(ThreadMain,newArg);
        newThread.detach();
    }
   

/*
    if(op == "read"){
        //cout << "Start reading" << endl;
        ReadNote(&server, argv[2]);
    } else if(op == "write"){
        WriteNote(&server, argv[2]);
    } else if(op == "remove"){
        RemoveNote(&server, argv[2]);
    }
*/


   // server.closeSocket();
    return 0;
}

/*
void ReadNote(ServerSocketHandler * server, string filename){
    const int MAX_BUFFER  = 1024;
    char buffer[MAX_BUFFER];
    //cout << "Start Reading" << endl;
    string execute_command = "cat *"+filename+"*";

    //cout << execute_command << endl;

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

    filename = reply.response;
    server->trySend(filename);

    ofstream fout(filename, ios::app);
    while(server->tryReceive()){
        string str = server->getBufferString();
        //reply = FileHandler::appendFile(filename, str);
        fout << str << endl;
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
*/