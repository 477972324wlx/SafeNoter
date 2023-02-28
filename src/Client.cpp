#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
using namespace std;

int genSocketPair(){
    srand(time(0));
    int port = rand() % 10000 + 5000;
    return port;
}

int main(int args, char **argv){
    const string addr = "127.0.0.1";
    unsigned short port = 4396;

    string oper;
    string userfilename;
    
    switch (args){
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

    ClientSocketHandler client = ClientSocketHandler(port, addr);
    client.trySend(oper);
    

    if(oper == "read"){
        

    } else if (oper == "write"){


    } else {
        perror("Unknown operation!\n");
        return 0;
    }
    

    return 0;
}