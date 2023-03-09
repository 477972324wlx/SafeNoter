#ifndef _UTIL
#include "util.hpp"
#define _UTIL
#endif


#include <bits/stdc++.h>
using std::string;
using std::cout;
using std::endl;
using std::ofstream;
struct FileHandlerReply;
class FileHandler;


/*
    A Static Class used to get input/output From bash.  

    Using bash command : cat/rm/print to do the file stuff
    
    Linux file command(components) should be safe enough?

*/
class FileHandler{

public:
    //Start a pipe executing bash command, get the file descripter
    //The content may be huge, cannot read whole at one time
    static FILE*  GetPipeFromCommand(string);

    // execute bash commands, get the command line output
    // the result of the command should be considered not too large
    // For large output , Use GetPipeFromCommand 
    static string GetStdoutFromCommand(string);     


    /*
        Dealing with files using bash
        Arguments should be checked or Shell injection may work.
    */

    static FileHandlerReply createFile(string);
    static FileHandlerReply removeFile(string);
    static FileHandlerReply testFile(string);   //whether a file exists or not 
    static FileHandlerReply readFile(string);   
    static FileHandlerReply appendFile(string,string);      
    
};

// Response for FileHandler
struct FileHandlerReply{
    string response;
    int error_code;
};



FILE * FileHandler::GetPipeFromCommand(string cmd){
    FILE* stream;
    cmd.append(" 2>&1"); 
    stream = popen(cmd.c_str(), "r");

    return stream;
}

string FileHandler::GetStdoutFromCommand(string cmd) {
    string data;
   
    const int max_buffer = 1024;
    char buffer[max_buffer];
    
    FILE *stream  = FileHandler::GetPipeFromCommand(cmd);
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}


FileHandlerReply FileHandler::readFile(string path){
    if(path.size() > 100){
        return FileHandlerReply{"string too long", 1};
    }
    string execute_command = "cat " + path; 
    string ret = FileHandler::GetStdoutFromCommand(execute_command);

    if(ret.find("No such file or directory") != string::npos || ret.find("Permission deni") != string::npos){
        return FileHandlerReply{ret, 1};
    }   
    return FileHandlerReply{ret, 0};

}   


FileHandlerReply FileHandler::appendFile(string filename, string content){
    FileHandlerReply reply;

    if(content.size() > 1000){
       return FileHandlerReply{filename, 1};
    }
    ofstream fout(filename, std::ios::app);
    
    fout << content;
    fout.close();
    return FileHandlerReply{"", 0};
}

FileHandlerReply FileHandler::createFile(string filename){
    string execute_command = "touch " + filename;
    string ret = FileHandler::GetStdoutFromCommand(execute_command);
    if(ret.find("Permission denied") != string::npos){
        return FileHandlerReply{ret, 1};
    }
    return FileHandlerReply{filename, 0};
}

FileHandlerReply FileHandler::testFile(string filename){
    string execute_command = "file " + filename;
    string ret = FileHandler::GetStdoutFromCommand(execute_command);
    if(ret.find("(No such file or directory)") != string::npos){
        return FileHandlerReply{ret,1};
    }
    if(ret.find("Permission denied") != string::npos){
        return FileHandlerReply{ret, 1};
    }
    if(util::isOneLine(ret)){
        return FileHandlerReply{"Multiple choices",1};
    }
    return FileHandlerReply{filename, 0};
}

FileHandlerReply FileHandler::removeFile(string filename){
    string execute_command= "rm " + filename;
    cout << execute_command << endl;
    string reply = FileHandler::GetStdoutFromCommand(execute_command);
    if(reply.find("Permission Denied") != string::npos || reply.find("No such") != string::npos){
        return FileHandlerReply{reply,1};
    }

    return FileHandlerReply{reply, 0};
}