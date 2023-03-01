#include <bits/stdc++.h>
#include "header/SocketHandler.hpp"
using namespace std;






int main(int args, char ** argv){
    const string addr = "127.0.0.1";
    unsigned short port = 4396;
    cout << "Hello here" << endl;
    if(args != 3){
        return 0;
    }


    ServerSocketHandler server = ServerSocketHandler(port, addr);
    
    // block until connection
    server.tryAccept();

    
    // Get operation type : Read/Write/Remove
    // server.tryReceive();
    string operation = server.getBufferString();

    cout << argv[0] <<' ' << argv[1] << endl;
    
    cout << operation << endl;

    server.trySend(operation + string(argv[0]) + string(argv[1]));
    
    server.closeSocket();
    return 0;
}