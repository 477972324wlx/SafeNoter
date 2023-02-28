#include <bits/stdc++.h>
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

struct FileHandlerReply;
class FileHandler;


/*
    A Static Class used to get input/output From bash.  No instance required 
    
    Linux file command(components) should be safer than my c++ version (I hope ?)

*/
class FileHandler{

    static string GetStdoutFromCommand(string);     // execute bash commands, get the output
    static bool checkFilename(string);     // check illegal symbols in filename
public:
    static FileHandlerReply readFile(string);       // read noteFiles
    static FileHandlerReply appendFile(string,string);     // append content to files
    static FileHandlerReply createFile(string);
    
};

struct FileHandlerReply{
    string response;
    int error_code;
};

static bool checkFilename(string str){
    for (char &ch : str){
        if (ch == '\'' || ch == '"' || ch =='\\'){
            return false;
        }
    }
    return true;
}



string FileHandler::GetStdoutFromCommand(string cmd) {
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1"); // Do we want STDERR?

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

FileHandlerReply FileHandler::readFile(string path){
 
    if(FileHandler::checkFilename(path)){
        return FileHandlerReply{"Illegal filename", 1};
    }
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
    
    if (!fout){
        reply.error_code = 1;
        reply.response = "Cannot open file";
    }
    fout << content << std::endl;
}

FileHandlerReply FileHandler::createFile(string user_filename){
  

}