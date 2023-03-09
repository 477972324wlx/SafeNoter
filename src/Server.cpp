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
    printf("Server launched!\n");
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
}
