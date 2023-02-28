#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
using namespace std;


int main(int args, char ** argv){
    const string addr = "127.0.0.1";
    unsigned short port = 4396;

    ServerSocketHandler server = ServerSocketHandler(port, addr);
    server.tryAccept();
    
    
    return 0;
}