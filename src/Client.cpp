#include <bits/stdc++.h>
#include "header/FileHandler.hpp"
using namespace std;

int main(){
    FileHandlerReply reply = FileHandler::readFile("Server.cpp");
    cout << reply.response << endl;
    reply = FileHandler::appendFile("Server.cpp", "// ';echo 3");



    
    return 0;
}