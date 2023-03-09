#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
using namespace std;

const string addr = "127.0.0.1";
unsigned short port = 4396;



void ReadNote(ClientSocketHandler*);
void WriteNote(ClientSocketHandler*);
void RemoveNote(ClientSocketHandler*);




int main(int args, char **argv){

    // Address & port for sockets
    

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


    if(strlen(argv[1]) + strlen(argv[2]) > 1000){
        perror("Arguments are too long!\n");
        exit(1);
    }

    char requestArgs[1024] = {0};
    sprintf(requestArgs,"%s;%s", argv[1], argv[2]);
    //store error messages;
    
    //connect to server
    ClientSocketHandler client = ClientSocketHandler(port, addr);
    //cout << "Connected" << endl;


    client.trySend(string(requestArgs));



    if(strcmp(argv[1], "read") == 0){
        ReadNote(&client);
    } else if(strcmp(argv[1], "write") == 0){
        WriteNote(&client);
    } else if(strcmp(argv[1], "remove") == 0){
        RemoveNote(&client);
    } else {
        perror("Unknown Operation\n");
        exit(1);
    }
  
    return 0;
}

void ReadNote(ClientSocketHandler* client){
    string err;
    while(1){
        client->tryReceive(err);
        if (err != ""){
            break;
        }
        write(1, client->getBuffer(), strlen(client->getBuffer()));
        
    }   
}
void WriteNote(ClientSocketHandler* client) {
    string str, err, filename;

    client->tryReceive(err);
    filename = client->getBufferString();

    cout << filename;
    while(fgets(client->buf,1024, stdin)){
        client->trySend(string(client->buf));
    }
}

void RemoveNote(ClientSocketHandler* client){
    string err;
    string str;
    client->tryReceive(err);
    cout << client->getBufferString();

    getline(cin,str);
    client->trySend(str);

    client->tryReceive(err);
    cout << client->getBufferString() << endl;

}
