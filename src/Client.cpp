#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
using namespace std;


void ReadNote(ClientSocketHandler*, string);
void WriteNote(ClientSocketHandler*, string);
void RemoveNote(ClientSocketHandler*, string);


static int exec_prog(const char **argv)
{
    pid_t   my_pid;
    int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;

    if (0 == (my_pid = fork())) {
            if (-1 == execve(argv[0], (char **)argv , NULL)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }
    return 0;
}

int main(int args, char **argv){

    // Address & port for sockets
    const string addr = "127.0.0.1";
    unsigned short port = 4396;


    // Get operation & filename from cmd line
    string oper;
    string userfilename;
    
    switch (args){
        // must contain 3 arguments
        case 3 :{
            oper = string(argv[1]);
            userfilename = string(argv[2]);
            break;
        }
        default:{
            perror("Insufficent arguments!\n");
            return 0;
        }
    }

    
    const char *my_argv[64] = {"./bin/Server", "read",argv[2],NULL};
   
    exec_prog(my_argv);
    usleep(1000);

    //store error messages;
    string err;

    
    //connect to server
    ClientSocketHandler client = ClientSocketHandler(port, addr);
    cout << "Connected" << endl;
    
    if(oper == "read"){
        ReadNote(&client, argv[2]);
    } else if(oper == "write"){
        WriteNote(&client,argv[2]);
    } else if (oper ==  "remove"){
        RemoveNote(&client,argv[2]);
    }

    /*
    client.tryReceive(err);

    cout << client.getBufferString() << endl;

    string str;
    while(cin >> str){
        client.trySend(str);
        client.tryReceive(err);
        cout << client.getBufferString() <<endl;
    }
    */
    
  
    return 0;
}

void ReadNote(ClientSocketHandler* client, string filename){
    string err;
    while(1){
        client->tryReceive(err);
        if (err != ""){
            break;
        }
        cout << client->getBufferString() << endl;
        
    }   
}
void WriteNote(ClientSocketHandler*, string filename){

}
void RemoveNote(ClientSocketHandler*, string filename){
    
}